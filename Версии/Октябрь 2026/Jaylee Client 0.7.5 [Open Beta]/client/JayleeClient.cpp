#include "pch.h"
#include "JayleeClient.h"
#include "visual/window.h"
#include "logic/hiddenWinwsStarter.h"
#include "logic/configSystem.h"
#include "logic/processController.h"
#include <string>

using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Jaylee_client::implementation {

JayleeClient::JayleeClient()
{
}

void JayleeClient::Start()
{
	if (!processcontroller::EnsureSingleInstance())
	{
		return;
	}

	LoadSettingsFromConfig();

	m_window = CreateJayleeWindow(L"Jaylee Client");
	if (m_window)
	{
		m_window.Activate();
		RegisterMainWindow(m_window, L"Jaylee Client");
	}
}

void JayleeClient::StartWinws(const std::wstring& exePath, const std::wstring& args)
{
	HiddenWinwsStarter::Start(exePath, args);
}
}