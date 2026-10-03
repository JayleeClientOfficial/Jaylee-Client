#pragma once

#include <windows.h>
#include <string>

class HiddenWinwsStarter {
public:
	struct StartResult {
		bool success = false;
		DWORD processId = 0;
		HANDLE processHandle = nullptr;
		std::wstring errorMessage;
	};

	static StartResult Start(const std::wstring& exePath, const std::wstring& args);
};
