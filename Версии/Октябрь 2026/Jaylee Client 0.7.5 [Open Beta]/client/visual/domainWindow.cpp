#include "pch.h"
#include "domainWindow.h"
#include "window.h"
#include "domainButtons.h"

#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <Windows.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;

void ShowDomainWindow()
{
	static Window domainWindow{ nullptr };
	constexpr wchar_t windowTitle[] = L"\u0412\u044B\u0431\u043E\u0440 \u0441\u043F\u0438\u0441\u043A\u0430 \u0434\u043E\u043C\u0435\u043D\u043E\u0432";

	if (domainWindow) {
		try {
			ActivateOwnedWindow(domainWindow, windowTitle);
			return;
		}
		catch (...) {
			domainWindow = nullptr;
		}
	}

	domainWindow = Window();
	domainWindow.Title(winrt::hstring(windowTitle));
	Grid content{};
	content.RequestedTheme(ElementTheme::Light);
	winrt::Windows::UI::Color background{};
	background.A = 0xFF;
	background.R = 0xFF;
	background.G = 0xFF;
	background.B = 0xFF;
	content.Background(SolidColorBrush{ background });
	content.Children().Append(CreateDomainButtonsPanel());
	domainWindow.Content(content);
	domainWindow.Closed([](auto&&, auto&&) { domainWindow = nullptr; });
	ActivateOwnedWindow(domainWindow, windowTitle);

	HWND hwnd = FindWindowW(nullptr, windowTitle);
	if (hwnd) {
		constexpr int width = 380;
		constexpr int height = 290;
		LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
		style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
		SetWindowLongPtrW(hwnd, GWL_STYLE, style);
		SetWindowPos(hwnd, nullptr, 0, 0, width, height,
			SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
	}
}
