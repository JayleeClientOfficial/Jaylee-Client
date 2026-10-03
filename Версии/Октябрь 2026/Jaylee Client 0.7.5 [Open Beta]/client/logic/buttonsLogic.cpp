#include "pch.h"
#include "winrt/base.h"
#include "buttonsLogic.h"

#include "../visual/settings.h"
#include "../visual/aboutWindow.h"
#include "../visual/domainWindow.h"

#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <string>
#include <vector>
#include <functional>
#include <TlHelp32.h>
#include "autorun.h"
#include "workInTray.h"
#include "configSystem.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <process.h>
#include <shellapi.h>
#include <thread>
#include "hiddenWinwsStarter.h"
#include <fstream>
#include <mutex>
#include <algorithm>

static std::vector<winrt::Microsoft::UI::Xaml::Window> g_registeredWindows;
static std::mutex g_registeredWindowsMutex;
static std::vector<std::pair<::winrt::weak_ref<winrt::Microsoft::UI::Xaml::Window>, std::function<void(int)>>> g_themeListeners;
static std::mutex g_themeListenersMutex;


static std::wstring FindFileInZapret(const std::wstring &fileName) {
    std::vector<std::wstring> bases;
    wchar_t buffer[MAX_PATH] = {};
    std::wstring moduleDir = L".";
    if (GetModuleFileNameW(nullptr, buffer, MAX_PATH) > 0) {
        std::wstring path(buffer);
        size_t pos = path.find_last_of(L"\\/");
        if (pos != std::wstring::npos) moduleDir = path.substr(0, pos);
    }
    if (moduleDir.size() >= 2 && moduleDir[1] == L':') {
        std::wstring drive = moduleDir.substr(0, 2);
        bases.push_back(drive + L"\\Jaylee client\\client\\zapret\\");
        bases.push_back(drive + L"\\Jaylee client\\Jaylee client\\client\\zapret\\");
    }
    bases.push_back(moduleDir + L"\\client\\zapret\\");
    bases.push_back(moduleDir + L"\\..\\client\\zapret\\");
    bases.push_back(moduleDir + L"\\..\\..\\client\\zapret\\");
    bases.push_back(moduleDir + L"\\..\\..\\..\\client\\zapret\\");
    bases.push_back(moduleDir + L"\\zapret\\");
    wchar_t cwdBuf[MAX_PATH] = {};
    if (GetCurrentDirectoryW(MAX_PATH, cwdBuf) > 0) {
        bases.push_back(std::wstring(cwdBuf) + L"\\client\\zapret\\");
        bases.push_back(std::wstring(cwdBuf) + L"\\zapret\\");
    }
    bases.push_back(moduleDir + L"\\..\\..\\..\\..\\client\\zapret\\");

    for (auto &b : bases) {
        std::wstring p = b + fileName;
        if (GetFileAttributesW(p.c_str()) != INVALID_FILE_ATTRIBUTES) return p;
    }
    return L"";
}

namespace {
    std::wstring GetCurrentModuleDir() {
        wchar_t buffer[MAX_PATH] = {};
        DWORD len = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        if (len == 0 || len >= MAX_PATH) return L".";
        std::wstring path(buffer);
        size_t pos = path.find_last_of(L"\\/");
        if (pos == std::wstring::npos) return L".";
        return path.substr(0, pos);
    }

    std::wstring FindAltBatch(int altIndex) {
        if (altIndex <= 0) return L"";
    if (altIndex == 13) {
        auto p = FindFileInZapret(L"general (ALT13).bat");
        if (!p.empty()) return p;
    } else if (altIndex == 16) {
        auto p = FindFileInZapret(L"general (FAKE TLS AUTO ALT).bat");
        if (!p.empty()) return p;
    } else if (altIndex == 14) {
        auto p = FindFileInZapret(L"general (FAKE TLS AUTO ALT2).bat");
        if (!p.empty()) return p;
    } else if (altIndex == 15) {
        auto p = FindFileInZapret(L"general (FAKE TLS AUTO ALT3).bat");
        if (!p.empty()) return p;
    } else if (altIndex == 17) {
        auto p = FindFileInZapret(L"general (EXP).bat");
        if (!p.empty()) return p;
    } else if (altIndex == 18) {
        auto p = FindFileInZapret(L"general.bat");
        if (!p.empty()) return p;
    }
    std::vector<std::wstring> bases;
    std::wstring moduleDir = GetCurrentModuleDir();
    if (moduleDir.size() >= 2 && moduleDir[1] == L':') {
        std::wstring drive = moduleDir.substr(0, 2);
        bases.push_back(drive + L"\\Jaylee client\\client\\zapret\\");
        bases.push_back(drive + L"\\Jaylee client\\Jaylee client\\client\\zapret\\");
    }
    bases.push_back(moduleDir + L"\\client\\zapret\\");
    bases.push_back(GetCurrentModuleDir() + L"\\..\\client\\zapret\\");
    bases.push_back(GetCurrentModuleDir() + L"\\..\\..\\client\\zapret\\");
    bases.push_back(GetCurrentModuleDir() + L"\\..\\..\\..\\client\\zapret\\");
    bases.push_back(GetCurrentModuleDir() + L"\\zapret\\");
    wchar_t cwdBuf[MAX_PATH] = {};
    if (GetCurrentDirectoryW(MAX_PATH, cwdBuf) > 0) {
        bases.push_back(std::wstring(cwdBuf) + L"\\client\\zapret\\");
        bases.push_back(std::wstring(cwdBuf) + L"\\zapret\\");
    }
    bases.push_back(GetCurrentModuleDir() + L"\\..\\..\\..\\..\\client\\zapret\\");

    for (auto &dir : bases) {
        std::function<std::wstring(const std::wstring&, int)> searchDir;
        searchDir = [&](const std::wstring &searchDirPath, int depth) -> std::wstring {
            WIN32_FIND_DATAW fd;
            std::wstring fileSearch = searchDirPath + L"\\*.bat";
            HANDLE h = FindFirstFileW(fileSearch.c_str(), &fd);
            if (h != INVALID_HANDLE_VALUE) {
                do {
                    std::wstring name = fd.cFileName;
                    std::wstring up = name;
                    for (auto &ch : up) ch = towupper(ch);
                    size_t p = up.find(L"ALT");
                    if (p != std::wstring::npos) {
                        size_t q = p + 3;
                        while (q < up.size() && !iswdigit(up[q])) q++;
                        if (q < up.size() && iswdigit(up[q])) {
                            int val = 0;
                            size_t r = q;
                            while (r < up.size() && iswdigit(up[r])) {
                                val = val * 10 + (up[r] - L'0');
                                r++;
                            }
                            if (val == altIndex) {
                                FindClose(h);
                                return searchDirPath + name;
                            }
                        } else {
                            if (altIndex == 1) {
                                if (q >= up.size() || !iswalnum(up[q])) {
                                    FindClose(h);
                                    return searchDirPath + name;
                                }


                            }
                        }
                    }
                    std::wstring upName = up;
                    if (upName.find(L"GENERAL") != std::wstring::npos) {
                        for (size_t i = 0; i < upName.size(); ++i) {
                            if (iswdigit(upName[i])) {
                                int val = upName[i] - L'0';
                                size_t j = i + 1;
                                while (j < upName.size() && iswdigit(upName[j])) { val = val * 10 + (upName[j] - L'0'); j++; }
                                if (val == altIndex) {
                                    FindClose(h);
                                    return searchDirPath + name;
                                }
                                break;
                            }
                        }
                    }
                } while (FindNextFileW(h, &fd));
                FindClose(h);
            }

            if (depth > 0) {
                std::wstring dirSearch = searchDirPath + L"\\*";
                HANDLE dh = FindFirstFileW(dirSearch.c_str(), &fd);
                if (dh != INVALID_HANDLE_VALUE) {
                    do {
                        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..") != 0) {
                            std::wstring sub = searchDirPath + L"\\" + fd.cFileName;
                            std::wstring r = searchDir(sub, depth - 1);
                            if (!r.empty()) {
                                FindClose(dh);
                                return r;
                            }
                        }
                    } while (FindNextFileW(dh, &fd));
                    FindClose(dh);
                }
            }
            return L"";
        };

        wchar_t bufExact[MAX_PATH];
        swprintf_s(bufExact, _countof(bufExact), L"%lsgeneral (ALT%d).bat", dir.c_str(), altIndex);
        if (GetFileAttributesW(bufExact) != INVALID_FILE_ATTRIBUTES) return std::wstring(bufExact);
        swprintf_s(bufExact, _countof(bufExact), L"%lsgeneral (ALT %d).bat", dir.c_str(), altIndex);
        if (GetFileAttributesW(bufExact) != INVALID_FILE_ATTRIBUTES) return std::wstring(bufExact);
        if (altIndex == 1) {
            swprintf_s(bufExact, _countof(bufExact), L"%lsgeneral (ALT).bat", dir.c_str());
            if (GetFileAttributesW(bufExact) != INVALID_FILE_ATTRIBUTES) return std::wstring(bufExact);
        }

        std::wstring rec = searchDir(dir, 2);
        if (!rec.empty()) return rec;
        WIN32_FIND_DATAW fd;
        std::wstring search = dir + L"*.bat";
        HANDLE h = FindFirstFileW(search.c_str(), &fd);
        if (h == INVALID_HANDLE_VALUE) {
            continue;
        }
        do {
            std::wstring name = fd.cFileName;
            size_t p = name.find(L"ALT");
            if (p != std::wstring::npos) {
                size_t q = p + 3;
                while (q < name.size() && !iswdigit(name[q])) q++;
                if (q < name.size() && iswdigit(name[q])) {
                    int val = 0;
                    size_t r = q;
                    while (r < name.size() && iswdigit(name[r])) {
                        val = val * 10 + (name[r] - L'0');
                        r++;
                    }
                    if (val == altIndex) {
                        FindClose(h);
                        return dir + name;
                    }
                } else {
                    if (altIndex == 1) {
                        if (q < name.size()) {
                            if (!iswalnum(name[q])) {
                                FindClose(h);
                                return dir + name;
                            }
                        } else {
                            FindClose(h);
                            return dir + name;
                        }
                    }
                }
            }
        } while (FindNextFileW(h, &fd));
        FindClose(h);
    }

    return L"";
    }

    void KillWinwsProcesses() {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return;
        PROCESSENTRY32W pe{};
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(snap, &pe)) {
            do {
                std::wstring exe = pe.szExeFile;
                for (auto &c : exe) c = towlower(c);
                if (exe == L"winws.exe") {
                    HANDLE ph = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                    if (ph) {
                        TerminateProcess(ph, 0);
                        CloseHandle(ph);
                    }
                }
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
    }
}

static int g_selectedAlt = 0;
static PROCESS_INFORMATION g_batchPi = {};
static bool g_running = false;
static bool g_uiBypassRunning = false;

static DWORD g_lastCreateProcessError = 0;

void OpenSettingsFromLogic()
{
    ShowSettingsWindow();
}

void OpenAboutFromLogic()
{
    ShowAboutWindow();
}

void OpenServiceBatchFromLogic()
{
    std::wstring found = FindFileInZapret(L"service.bat");
    if (found.empty()) {
        MessageBoxW(NULL, L"Файл service.bat не найден в папке client\\zapret. Проверьте расположение проекта.", L"Файл не найден", MB_OK | MB_ICONERROR);
        return;
    }

    HINSTANCE h = ShellExecuteW(NULL, L"open", found.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if ((INT_PTR)h <= 32) {
        std::wstring cmd = L"/c \"" + found + L"\"";
        SHELLEXECUTEINFOW sei{};
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = NULL;
        sei.lpVerb = L"open";
        sei.lpFile = L"cmd.exe";
        sei.lpParameters = cmd.c_str();
        sei.nShow = SW_SHOW;
        ShellExecuteExW(&sei);
    }
}

void OpenListFileFromLogic()
{
    ShowDomainWindow();
}

void OpenDomainListFromLogic(std::wstring const& fileName)
{
    std::wstring found = FindFileInZapret(L"lists\\" + fileName);
    if (found.empty()) {
        std::wstring message = L"Файл списка не найден: " + fileName;
        MessageBoxW(nullptr, message.c_str(), L"Файл не найден", MB_OK | MB_ICONERROR);
        return;
    }

    std::wstring parameters = L"\"" + found + L"\"";
    HINSTANCE result = ShellExecuteW(nullptr, L"open", L"notepad.exe", parameters.c_str(), nullptr, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) <= 32) {
        MessageBoxW(nullptr, L"Не удалось открыть список в Блокноте.", L"Ошибка открытия файла", MB_OK | MB_ICONERROR);
    }
}

void SelectStrategy(int altIndex)
{
    bool wasRunning = IsBypassRunning();
    g_selectedAlt = altIndex;
    SaveSettingsToConfig();

    if (wasRunning) {
        if (altIndex != 0) {
            StartSelectedBypass();
        } else {
            StopBypass();
        }
    }
}

int GetSelectedStrategy()
{
    return g_selectedAlt;
}

bool StartSelectedBypass()
{
    if (g_selectedAlt == 0) return false;

    if (IsBypassRunning()) {
        StopBypass();
        Sleep(200);
    }

    std::wstring bat = FindAltBatch(g_selectedAlt);
    if (bat.empty()) return false;

    bool started = false;
    std::wstring exePath;
    std::wstring args;
    std::wstring batDir;

    {
        std::wstring joined;
        std::wifstream ifs(bat);
        if (ifs.good()) {
            std::wstring line;
            while (std::getline(ifs, line)) {
                if (!line.empty() && line.back() == L'\r') line.pop_back();
                size_t i = line.find_last_not_of(L" \t");
                if (i != std::wstring::npos && line[i] == L'^') {
                    line.resize(i);
                    joined += line;
                } else {
                    joined += line + L' ';
                }
            }
        }
        if (!joined.empty()) {
            size_t pos = joined.find(L"winws.exe");
            if (pos != std::wstring::npos) {
                size_t left = joined.rfind(L'"', pos);
                size_t right = (left != std::wstring::npos) ? joined.find(L'"', left + 1) : std::wstring::npos;
                std::wstring exeToken;
                if (left != std::wstring::npos && right != std::wstring::npos && right > left) {
                    exeToken = joined.substr(left + 1, right - left - 1);
                }

                size_t p = bat.find_last_of(L"\\/");
                batDir = (p == std::wstring::npos) ? L"." : bat.substr(0, p + 1);

                std::wstring listsPath = batDir + L"lists\\";
                auto ensureFile = [](const std::wstring& path, const std::string& defaultContent) {
                    if (GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
                        std::ofstream ofs(path);
                        if (ofs.is_open()) ofs << defaultContent;
                    }
                };
                ensureFile(listsPath + L"ipset-exclude-user.txt", "203.0.113.113/32\n");
                ensureFile(listsPath + L"list-general-user.txt", "# Never leave this file empty\ndomain.example.abc\n");
                ensureFile(listsPath + L"list-exclude-user.txt", "domain.example.abc\n");

                std::wstring gameFilterTCP = L"12";
                std::wstring gameFilterUDP = L"12";
                std::wstring gameFlagFile = batDir + L"utils\\game_filter.enabled";
                if (GetFileAttributesW(gameFlagFile.c_str()) != INVALID_FILE_ATTRIBUTES) {
                    std::wifstream gff(gameFlagFile);
                    if (gff.good()) {
                        std::wstring mode;
                        gff >> mode;
                        for (auto& c : mode) c = towlower(c);
                        if (mode == L"all") {
                            gameFilterTCP = L"1024-65535";
                            gameFilterUDP = L"1024-65535";
                        } else if (mode == L"tcp") {
                            gameFilterTCP = L"1024-65535";
                        } else if (mode == L"udp") {
                            gameFilterUDP = L"1024-65535";
                        }
                    }
                }

                auto replaceAll = [&](std::wstring &s, const std::wstring &from, const std::wstring &to) {
                    if (from.empty()) return;
                    size_t pos = 0;
                    while (pos <= s.length() && pos + from.length() <= s.length()) {
                        bool match = true;
                        for (size_t i = 0; i < from.length(); ++i) {
                            if (towupper(s[pos + i]) != towupper(from[i])) {
                                match = false;
                                break;
                            }
                        }
                        if (match) {
                            s.replace(pos, from.length(), to);
                            pos += to.length();
                        } else {
                            pos++;
                        }
                    }
                };
                std::wstring binPath = batDir + L"bin\\";

                if (!exeToken.empty()) {
                    replaceAll(exeToken, L"%BIN%", binPath);
                    replaceAll(exeToken, L"%~dp0", batDir);
                    replaceAll(exeToken, L"%LISTS%", listsPath);
                    exePath = exeToken;
                } else {
                    exePath = batDir + L"bin\\winws.exe";
                }

                if (right != std::wstring::npos) {
                    args = joined.substr(right + 1);
                } else if (!exeToken.empty()) {
                    size_t endPos = pos + wcslen(L"winws.exe");
                    if (endPos < joined.size()) args = joined.substr(endPos);
                }

                if (!args.empty()) {
                    replaceAll(args, L"%BIN%", binPath);
                    replaceAll(args, L"%~dp0", batDir);
                    replaceAll(args, L"%LISTS%", listsPath);
                    replaceAll(args, L"%GameFilterTCP%", gameFilterTCP);
                    replaceAll(args, L"%GameFilterUDP%", gameFilterUDP);
                }

                auto trim = [&](std::wstring &s) {
                    size_t a = s.find_first_not_of(L" \t\r\n");
                    size_t b = s.find_last_not_of(L" \t\r\n");
                    if (a == std::wstring::npos) { s.clear(); return; }
                    s = s.substr(a, b - a + 1);
                };
                trim(exePath);
                trim(args);

                if (!exePath.empty() && exePath.size() >= 2 && exePath[1] != L':') {
                    exePath = batDir + exePath;
                }

                if (!exePath.empty() && GetFileAttributesW(exePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                    std::wstring finalArgs = L"\"" + exePath + L"\" " + args;
                    auto res = HiddenWinwsStarter::Start(exePath, finalArgs);
                    if (res.success) {
                        g_batchPi.hProcess = res.processHandle;
                        g_batchPi.hThread = nullptr;
                        g_running = true;
                        g_uiBypassRunning = true;
                        started = true;
                    }
                }
            }
        }
    }

    if (started) return true;

    if (!exePath.empty() && GetFileAttributesW(exePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
        SHELLEXECUTEINFOW sei{};
        sei.cbSize = sizeof(sei);
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = nullptr;
        sei.lpVerb = L"runas";
        sei.lpFile = exePath.c_str();
        sei.lpParameters = args.c_str();
        sei.nShow = SW_HIDE;

        std::wstring workDir;
        size_t slashPos = exePath.find_last_of(L"\\/");
        if (slashPos != std::wstring::npos) {
            workDir = exePath.substr(0, slashPos);
            sei.lpDirectory = workDir.c_str();
        }

        if (ShellExecuteExW(&sei) && sei.hProcess) {
            g_batchPi.hProcess = sei.hProcess;
            g_batchPi.hThread = nullptr;
            g_running = true;
            g_uiBypassRunning = true;
            return true;
        }
    }

    std::wstring params = L"/C "" + bat + L""";
    SHELLEXECUTEINFOW sei{};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = nullptr;
    sei.lpVerb = L"open";
    sei.lpFile = L"cmd.exe";
    sei.lpParameters = params.c_str();
    sei.nShow = SW_HIDE;

    if (!ShellExecuteExW(&sei) || !sei.hProcess) {
        g_lastCreateProcessError = GetLastError();
        if (sei.hProcess) CloseHandle(sei.hProcess);
        g_uiBypassRunning = false;
        return false;
    }

    g_batchPi.hProcess = sei.hProcess;
    g_batchPi.hThread = nullptr;
    g_running = true;
    g_uiBypassRunning = true;
    return true;
}

void StopBypass()
{
    if (g_batchPi.hProcess) {
        DWORD code = 0;
        if (GetExitCodeProcess(g_batchPi.hProcess, &code) && code == STILL_ACTIVE) {
            TerminateProcess(g_batchPi.hProcess, 0);
        }
        CloseHandle(g_batchPi.hProcess);
        g_batchPi = {};
    }
    KillWinwsProcesses();
    g_lastCreateProcessError = 0;
    g_running = false;
    g_uiBypassRunning = false;
}

bool IsBypassRunning()
{
    if (g_running) return true;
    if (g_batchPi.hProcess) {
        DWORD code = 0;
        if (GetExitCodeProcess(g_batchPi.hProcess, &code) && code == STILL_ACTIVE) return true;
    }
    return false;
}

bool CheckBypassStatus(std::wstring &outMessage)
{
    if (IsBypassRunning()) {
        outMessage = L"OK: bypass is running.";
        return true;
    }
    if (g_selectedAlt == 0) {
        outMessage = L"Ошибка: стратегия не выбрана.";
        return false;
    }
    std::wstring bat = FindAltBatch(g_selectedAlt);
    if (bat.empty()) {
        outMessage = L"Ошибка: .bat для выбранной стратегии не найден. Попробуйте проверить папку client\\zapret относительно каталога приложения.";
        return false;
    }
    if (g_lastCreateProcessError != 0) {
        wchar_t buf[512];
        swprintf_s(buf, _countof(buf), L"Ошибка CreateProcess/ShellExecuteEx: код=%lu.", g_lastCreateProcessError);
        outMessage = buf;
        return false;
    }
    outMessage = L"Ошибка: .bat найден, но процесс не запущен.";
    return false;
}

void ReportBypassStatusIfNotRunning()
{
    std::wstring msg;
    if (!CheckBypassStatus(msg)) {
        wchar_t tmpPath[MAX_PATH] = {};
        if (GetTempPathW(MAX_PATH, tmpPath) > 0) {
            std::wstring filePath = std::wstring(tmpPath) + L"Jaylee_bypass_status.txt";
            HANDLE h = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (h != INVALID_HANDLE_VALUE) {
                DWORD written = 0;
                const unsigned char bomUtf8[] = { 0xEF, 0xBB, 0xBF };
                WriteFile(h, bomUtf8, sizeof(bomUtf8), &written, nullptr);
                int required = WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), static_cast<int>(msg.size()), nullptr, 0, nullptr, nullptr);
                if (required > 0) {
                    std::string utf8;
                    utf8.resize(required);
                    WideCharToMultiByte(CP_UTF8, 0, msg.c_str(), static_cast<int>(msg.size()), utf8.data(), required, nullptr, nullptr);
                    DWORD wrote2 = 0;
                    if (!utf8.empty()) WriteFile(h, utf8.data(), static_cast<DWORD>(utf8.size()), &wrote2, nullptr);
                }
                CloseHandle(h);
                std::wstring ansiPath = std::wstring(tmpPath) + L"Jaylee_bypass_status_ansi.txt";
                HANDLE h2 = CreateFileW(ansiPath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
                if (h2 != INVALID_HANDLE_VALUE) {
                    int reqA = WideCharToMultiByte(CP_ACP, 0, msg.c_str(), static_cast<int>(msg.size()), nullptr, 0, nullptr, nullptr);
                    if (reqA > 0) {
                        std::string ansi;
                        ansi.resize(reqA);
                        WideCharToMultiByte(CP_ACP, 0, msg.c_str(), static_cast<int>(msg.size()), ansi.data(), reqA, nullptr, nullptr);
                        DWORD wroteA = 0;
                        if (!ansi.empty()) WriteFile(h2, ansi.data(), static_cast<DWORD>(ansi.size()), &wroteA, nullptr);
                    }
                    CloseHandle(h2);
                    ShellExecuteW(NULL, L"open", L"notepad.exe", ansiPath.c_str(), nullptr, SW_SHOW);
                    return;
                }
            }
        }
        MessageBoxW(NULL, L"Диагностика не удалась. Проверьте права и попробуйте запустить программу от имени администратора.", L"Ошибка обхода блокировок", MB_OK | MB_ICONERROR);
    }
}

bool GetUIBypassRunningState()
{
    return g_uiBypassRunning;
}

static void ApplyNativeWindowFrameTheme(int theme)
{
    const wchar_t* titles[] = {
        L"Jaylee Client",
        L"Настройки"
    };

    for (const wchar_t* title : titles) {
        HWND hwnd = FindWindowW(nullptr, title);
        if (!hwnd) {
            continue;
        }

        BOOL darkMode = theme == 1 ? TRUE : FALSE;
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
    }
}

void ApplyTheme(int theme)
{
    SetSavedTheme(theme);
    ApplyNativeWindowFrameTheme(theme);
    try {
        auto app = winrt::Microsoft::UI::Xaml::Application::Current();
        if (app) {
            using winrt::Microsoft::UI::Xaml::ApplicationTheme;
            ApplicationTheme at = (theme == 1) ? ApplicationTheme::Dark : ApplicationTheme::Light;
            app.RequestedTheme(at);
        }
    } catch (...) {}

    try {
        std::vector<winrt::Microsoft::UI::Xaml::Window> windows;
        {
            std::lock_guard<std::mutex> lk(g_registeredWindowsMutex);
            windows = g_registeredWindows;
        }

        if (!windows.empty()) {
            for (auto const& w : windows) {
                try {
                    auto dq = w.DispatcherQueue();
                    if (dq) {
                        auto win = w;
                        dq.TryEnqueue([win, theme]() {
                            try {
                                auto content = win.Content();
                                if (content) {
                                    if (auto fe = content.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>()) {
                                        fe.RequestedTheme(theme == 1 ? winrt::Microsoft::UI::Xaml::ElementTheme::Dark : winrt::Microsoft::UI::Xaml::ElementTheme::Light);
                                    }
                                    if (auto panel = content.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
                                        winrt::Windows::UI::Color bg{};
                                        if (theme == 1) { bg.A = 0xFF; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12; }
                                        else { bg.A = 0xFF; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
                                        panel.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bg });
                                    }
                                }

                                try {
                                    std::vector<std::function<void(int)>> toCall;
                                    {
                                        std::lock_guard<std::mutex> lk(g_themeListenersMutex);
                                        for (auto const& p : g_themeListeners) {
                                            try {
                                                auto stored = p.first.get();
                                                if (stored && stored == win) toCall.push_back(p.second);
                                            } catch(...) {}
                                        }
                                    }
                                    for (auto const& l : toCall) {
                                        try { l(theme); } catch(...) {}
                                    }
                                } catch(...) {}

                            } catch(...) {}
                        });
                    }
                } catch(...) {}
            }
        } else {
            std::lock_guard<std::mutex> lk(g_themeListenersMutex);
            for (auto const& p : g_themeListeners) {
                try { p.second(theme); } catch (...) {}
            }
        }
    } catch (...) {}
}

void RegisterThemeListener(winrt::Microsoft::UI::Xaml::Window const& w, std::function<void(int)> listener)
{
    std::lock_guard<std::mutex> lk(g_themeListenersMutex);
    g_themeListeners.push_back(std::make_pair(::winrt::make_weak(w), std::move(listener)));
}

void RegisterAppWindow(winrt::Microsoft::UI::Xaml::Window const& w)
{
    std::lock_guard<std::mutex> lk(g_registeredWindowsMutex);
    for (auto const& ex : g_registeredWindows) {
        if (ex == w) return;
    }
    g_registeredWindows.push_back(w);
}

void UnregisterAppWindow(winrt::Microsoft::UI::Xaml::Window const& w)
{
    std::lock_guard<std::mutex> lk(g_registeredWindowsMutex);
    g_registeredWindows.erase(std::remove_if(g_registeredWindows.begin(), g_registeredWindows.end(), [&](auto const& x){ return x == w; }), g_registeredWindows.end());

    {
        std::lock_guard<std::mutex> lk2(g_themeListenersMutex);
        g_themeListeners.erase(std::remove_if(g_themeListeners.begin(), g_themeListeners.end(), [&](auto const& p){
            try {
                auto stored = p.first.get();
                return (!stored) || (stored == w);
            } catch(...) {
                return true;
            }
        }), g_themeListeners.end());
    }
}

bool CheckBypassStatusEnhanced(std::wstring& outMessage, DWORD* pidOut)
{
    if (g_batchPi.hProcess) {
        DWORD code = 0;
        if (GetExitCodeProcess(g_batchPi.hProcess, &code) && code == STILL_ACTIVE) {
            if (pidOut) *pidOut = GetProcessId(g_batchPi.hProcess);
            wchar_t msg[256];
            swprintf_s(msg, _countof(msg), L"OK: батник запущен (PID: %lu).", GetProcessId(g_batchPi.hProcess));
            outMessage = msg;
            return true;
        }
    }
    outMessage = L"Процесс батника не найден.";
    return false;
}