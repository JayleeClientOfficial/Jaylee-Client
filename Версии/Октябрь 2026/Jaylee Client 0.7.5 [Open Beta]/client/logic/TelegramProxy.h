#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

class TelegramProxy {
public:
	TelegramProxy();
	~TelegramProxy();

	void StartAsync(const std::wstring& port = L"8080", const std::wstring& domain = L"");

	void Stop();

	bool IsRunning() const;

	DWORD GetPid() const;

private:
	DWORD FindExistingProcess() const;
	bool LaunchProcess(const std::wstring& exePath, const std::wstring& cmdLine);
	void MonitorLoop();
	std::wstring BuildExePath() const;

private:
	std::atomic<bool> m_running{ false };
	std::atomic<bool> m_owned{ false };
	mutable std::mutex m_mutex;
	HANDLE m_processHandle{ nullptr };
	DWORD m_pid{ 0 };
	std::thread m_monitorThread;
	std::wstring m_port;
	std::wstring m_domain;
};
