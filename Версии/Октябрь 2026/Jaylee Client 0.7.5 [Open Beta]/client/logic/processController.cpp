#include "pch.h"
#include "processController.h"

#include <windows.h>
#include <tlhelp32.h>
#include <algorithm>
#include <string>
#include <vector>

namespace processcontroller
{
    namespace
    {
        static const wchar_t* kSingleInstanceMutexName = L"JayleeClient_SingleInstance_Mutex";
        static HANDLE g_singleInstanceMutex = nullptr;

        void ShowWarningDialog(const std::wstring& message)
        {
            MessageBoxW(nullptr, message.c_str(), L"Jaylee Client", MB_ICONWARNING | MB_OK);
        }
    }

    bool IsAnotherInstanceRunning()
    {
        if (g_singleInstanceMutex != nullptr)
        {
            return false;
        }

        g_singleInstanceMutex = CreateMutexW(nullptr, TRUE, kSingleInstanceMutexName);
        if (!g_singleInstanceMutex) {
            return true;
        }

        DWORD err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS) {
            CloseHandle(g_singleInstanceMutex);
            g_singleInstanceMutex = nullptr;
            return true;
        }

        return false;
    }

    bool EnsureSingleInstance()
    {
        if (IsAnotherInstanceRunning())
        {
            ShowDuplicateInstanceWarningAndExit();
        }

        return true;
    }

    void TerminateChildProcesses()
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return;
        }

        std::vector<PROCESSENTRY32W> processes;
        PROCESSENTRY32W entry{};
        entry.dwSize = sizeof(entry);
        if (Process32FirstW(snapshot, &entry))
        {
            do
            {
                processes.push_back(entry);
            } while (Process32NextW(snapshot, &entry));
        }
        CloseHandle(snapshot);

        const DWORD currentPid = GetCurrentProcessId();
        std::vector<DWORD> pending{ currentPid };
        std::vector<DWORD> descendants;
        for (size_t parentIndex = 0; parentIndex < pending.size(); ++parentIndex)
        {
            for (const auto& process : processes)
            {
                if (process.th32ParentProcessID == pending[parentIndex] &&
                    std::find(pending.begin(), pending.end(), process.th32ProcessID) == pending.end())
                {
                    pending.push_back(process.th32ProcessID);
                    descendants.push_back(process.th32ProcessID);
                }
            }
        }

        for (auto processId = descendants.rbegin(); processId != descendants.rend(); ++processId)
        {
            HANDLE process = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, *processId);
            if (process)
            {
                TerminateProcess(process, 0);
                WaitForSingleObject(process, 1000);
                CloseHandle(process);
            }
        }
    }

    void ShowDuplicateInstanceWarningAndExit()
    {
        const std::wstring message = L"\u0414\u043B\u044F \u0437\u0430\u043F\u0443\u0441\u043A\u0430 \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u044B \u0442\u0440\u0435\u0431\u0443\u0435\u0442\u0441\u044F \u0437\u0430\u0432\u0435\u0440\u0448\u0438\u0442\u044C \u043F\u0440\u0435\u0434\u0432\u0430\u0440\u0438\u0442\u0435\u043B\u044C\u043D\u043E \u043E\u0442\u043A\u0440\u044B\u0442\u044B\u0439 \u044D\u043A\u0437\u0435\u043C\u043F\u043B\u044F\u0440 \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u044B!";
        ShowWarningDialog(message);
        ExitProcess(0);
    }
}

