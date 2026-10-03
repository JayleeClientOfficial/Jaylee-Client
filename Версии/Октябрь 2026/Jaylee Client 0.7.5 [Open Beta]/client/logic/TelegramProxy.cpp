#include "pch.h"
#include "TelegramProxy.h"
#include <tlhelp32.h>
#include <vector>
#include <algorithm>
#include <shellapi.h>

TelegramProxy::TelegramProxy()
{
}

TelegramProxy::~TelegramProxy()
{
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		m_running = false;
	}
	if (m_monitorThread.joinable()) {
		m_monitorThread.join();
	}

	if (m_processHandle) {
		CloseHandle(m_processHandle);
		m_processHandle = nullptr;
	}
}

void TelegramProxy::StartAsync(const std::wstring& port, const std::wstring& domain)
{
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		m_port = port.empty() ? L"8080" : port;
		m_domain = domain;
	}

	std::thread t([this]() {
		DWORD existing = FindExistingProcess();
		if (existing != 0) {
			HANDLE ph = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, existing);
			if (ph) {
				std::lock_guard<std::mutex> lk(m_mutex);
				m_pid = existing;
				m_processHandle = ph;
				m_owned = false;
				m_running = true;
			}
		} else {
			std::wstring exe = BuildExePath();
			if (!exe.empty()) {
				std::wstring cmd = L"\"" + exe + L"\" --port " + m_port;
				if (!m_domain.empty()) {
					cmd += L" --domain ";
					if (m_domain.find(L' ') != std::wstring::npos) cmd += L"\"" + m_domain + L"\"";
					else cmd += m_domain;
				}

				bool ok = LaunchProcess(exe, cmd);
				if (ok) {
					std::lock_guard<std::mutex> lk(m_mutex);
					m_running = true;
					m_owned = true;
				}
			}
		}

		{
			std::lock_guard<std::mutex> lk(m_mutex);
			if (!m_monitorThread.joinable()) {
				m_monitorThread = std::thread(&TelegramProxy::MonitorLoop, this);
			}
		}
	});
	t.detach();
}

void TelegramProxy::Stop()
{
	std::lock_guard<std::mutex> lk(m_mutex);
	if (m_owned && m_processHandle) {
		TerminateProcess(m_processHandle, 0);
		CloseHandle(m_processHandle);
		m_processHandle = nullptr;
		m_pid = 0;
		m_running = false;
		m_owned = false;
	}
}

bool TelegramProxy::IsRunning() const
{
	return m_running.load();
}

DWORD TelegramProxy::GetPid() const
{
	return m_pid;
}

DWORD TelegramProxy::FindExistingProcess() const
{
	DWORD found = 0;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) return 0;

	PROCESSENTRY32W pe{};
	pe.dwSize = sizeof(pe);
	if (Process32FirstW(snap, &pe)) {
		do {
			if (_wcsicmp(pe.szExeFile, L"TgWsProxy_windows.exe") == 0 || _wcsicmp(pe.szExeFile, L"tgwsproxy.exe") == 0) {
				found = pe.th32ProcessID;
				break;
			}
		} while (Process32NextW(snap, &pe));
	}
	CloseHandle(snap);
	return found;
}

bool TelegramProxy::LaunchProcess(const std::wstring& exePath, const std::wstring& cmdLine)
{
	std::wstring cmd = cmdLine;
	STARTUPINFOW si{};
	PROCESS_INFORMATION pi{};
	si.cb = sizeof(si);

	BOOL ok = CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	if (!ok) return false;

	{
		std::lock_guard<std::mutex> lk(m_mutex);
		m_processHandle = pi.hProcess;
		m_pid = pi.dwProcessId;
	}
	CloseHandle(pi.hThread);
	return true;
}

void TelegramProxy::MonitorLoop()
{
	while (true) {
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			if (!m_running && m_processHandle == nullptr && !m_owned) {
			}
			if (!m_running && !m_owned && m_processHandle == nullptr) {
				DWORD pid = FindExistingProcess();
				if (pid != 0) {
					HANDLE ph = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
					if (ph) {
						m_processHandle = ph;
						m_pid = pid;
						m_running = true;
						m_owned = false;
					}
				}
			}
		}

		HANDLE ph = nullptr;
		DWORD pid = 0;
		{
			std::lock_guard<std::mutex> lk(m_mutex);
			ph = m_processHandle;
			pid = m_pid;
		}

		if (ph) {
			DWORD status = WaitForSingleObject(ph, 2000);
			if (status == WAIT_OBJECT_0) {
				std::lock_guard<std::mutex> lk(m_mutex);
				CloseHandle(m_processHandle);
				m_processHandle = nullptr;
				m_pid = 0;
				m_running = false;
				m_owned = false;
			}
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(800));
		}

		{
			std::lock_guard<std::mutex> lk(m_mutex);
			if (!m_running && !m_owned && m_processHandle == nullptr && m_pid == 0) {
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}
}

std::wstring TelegramProxy::BuildExePath() const
{
	WCHAR exePath[MAX_PATH];
	if (GetModuleFileNameW(NULL, exePath, MAX_PATH) == 0) return {};
	std::wstring path(exePath);
	auto pos = path.find_last_of(L"\\/");
	std::wstring dir = (pos == std::wstring::npos) ? L"." : path.substr(0, pos + 1);
	std::wstring candidate = dir + L"client\\tgwsproxy\\TgWsProxy_windows.exe";
	if (GetFileAttributesW(candidate.c_str()) != INVALID_FILE_ATTRIBUTES) return candidate;
	candidate = dir + L"client\\tgwsproxy\\tgwsproxy.exe";
	if (GetFileAttributesW(candidate.c_str()) != INVALID_FILE_ATTRIBUTES) return candidate;
	return {};
}
