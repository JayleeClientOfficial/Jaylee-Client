#include "pch.h"
#include "telegramMenu.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Media.Animation.h>
#include "../logic/configSystem.h"
#include <Windows.h>
#include <shellapi.h>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Windows::UI::Xaml::Media::Animation;

winrt::Microsoft::UI::Xaml::UIElement CreateTelegramMenu()
{
	Grid root{};
	root.HorizontalAlignment(HorizontalAlignment::Stretch);
	root.VerticalAlignment(VerticalAlignment::Stretch);

	Border card{};
	card.HorizontalAlignment(HorizontalAlignment::Center);
	card.VerticalAlignment(VerticalAlignment::Center);
	card.CornerRadius({8,8,8,8});
	card.Padding({18,18,18,18});
	{
		int theme = 0; try { theme = GetSavedTheme(); } catch(...) {}
		winrt::Windows::UI::Color bg{}; bg.A = 0xFF;
		if (theme == 1) { bg.R = 0x1E; bg.G = 0x1E; bg.B = 0x1E; }
		else { bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
		card.Background(SolidColorBrush{ bg });
		winrt::Windows::UI::Color borderCol{}; borderCol.A = 0xFF;
		if (theme == 1) { borderCol.R = 0x33; borderCol.G = 0x33; borderCol.B = 0x33; }
		else { borderCol.R = 0xE6; borderCol.G = 0xE6; borderCol.B = 0xE6; }
		card.BorderBrush(SolidColorBrush{ borderCol });
		card.BorderThickness({1,1,1,1});
	}

	StackPanel content{};
	content.Orientation(Orientation::Vertical);
	content.Spacing(12);

	TextBlock title{};
	title.Text(winrt::hstring(L"TG WS Proxy — Настройки"));
	title.FontSize(15);
	title.Margin({0,0,0,4});
	content.Children().Append(title);

	Border section1{};
	section1.CornerRadius({6,6,6,6});
	section1.Padding({12,12,12,12});
	{
		int theme = 0; try { theme = GetSavedTheme(); } catch(...) {}
		winrt::Windows::UI::Color sbg{}; sbg.A = 0xFF;
		if (theme == 1) { sbg.R = 0x16; sbg.G = 0x16; sbg.B = 0x16; }
		else { sbg.R = 0xFB; sbg.G = 0xFB; sbg.B = 0xFB; }
		section1.Background(SolidColorBrush{ sbg });
	}

	StackPanel s1stack{}; s1stack.Orientation(Orientation::Vertical); s1stack.Spacing(10);

	Grid rowEnable{};
	ColumnDefinition rowEnableCol1{}; rowEnableCol1.Width(GridLengthHelper::FromValueAndType(1.0, GridUnitType::Star));
	ColumnDefinition rowEnableCol2{}; rowEnableCol2.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
	rowEnable.ColumnDefinitions().Append(rowEnableCol1);
	rowEnable.ColumnDefinitions().Append(rowEnableCol2);
	CheckBox enableChk{}; enableChk.Content(winrt::box_value(winrt::hstring(L"Включить TG WS Proxy")));
	enableChk.IsChecked(false);
	enableChk.VerticalAlignment(VerticalAlignment::Center);
	rowEnable.Children().Append(enableChk);
	Button testBtn{}; testBtn.Content(winrt::box_value(winrt::hstring(L"Тест"))); Grid::SetColumn(testBtn, 1); rowEnable.Children().Append(testBtn);
	s1stack.Children().Append(rowEnable);

	Grid rowDomain{};
	ColumnDefinition domCol1{}; domCol1.Width(GridLengthHelper::FromValueAndType(1.0, GridUnitType::Star));
	ColumnDefinition domCol2{}; domCol2.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
	rowDomain.ColumnDefinitions().Append(domCol1);
	rowDomain.ColumnDefinitions().Append(domCol2);
	TextBox domainBox{}; domainBox.PlaceholderText(winrt::hstring(L"Свой домен (опционально)")); domainBox.HorizontalAlignment(HorizontalAlignment::Stretch);
	rowDomain.Children().Append(domainBox);
	Button helpBtn{}; helpBtn.Content(winrt::box_value(winrt::hstring(L"?"))); helpBtn.Margin({8,0,0,0}); Grid::SetColumn(helpBtn,1); rowDomain.Children().Append(helpBtn);
	s1stack.Children().Append(rowDomain);

	Grid rowPort{};
	ColumnDefinition rp1{}; rp1.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
	ColumnDefinition rp2{}; rp2.Width(GridLengthHelper::FromValueAndType(1.0, GridUnitType::Star));
	ColumnDefinition rp3{}; rp3.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
	rowPort.ColumnDefinitions().Append(rp1);
	rowPort.ColumnDefinitions().Append(rp2);
	rowPort.ColumnDefinitions().Append(rp3);
	TextBlock portLabel{}; portLabel.Text(winrt::hstring(L"Port:")); portLabel.VerticalAlignment(VerticalAlignment::Center); rowPort.Children().Append(portLabel);
	TextBox portBox{}; portBox.Width(100); portBox.Text(winrt::hstring(L"8080")); portBox.Margin({8,0,8,0}); Grid::SetColumn(portBox,1); rowPort.Children().Append(portBox);
	Button launchBtn{}; launchBtn.Content(winrt::box_value(winrt::hstring(L"Запустить встроенный UI"))); Grid::SetColumn(launchBtn,2); rowPort.Children().Append(launchBtn);
	s1stack.Children().Append(rowPort);

	section1.Child(s1stack);
	content.Children().Append(section1);

	TextBlock note{};
	note.Text(winrt::hstring(L"Запускает TgWsProxy и предоставляет локальный веб-интерфейс. Настройте порт и домен по необходимости."));
	note.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::Wrap);
	note.FontSize(12);
	note.Margin({0,4,0,0});
	content.Children().Append(note);

	card.Child(content);

	root.Children().Append(card);
	return root;
}

DWORD LaunchTgWsProxy()
{
	WCHAR exePath[MAX_PATH];
	if (GetModuleFileNameW(NULL, exePath, MAX_PATH) == 0) return 0;
	std::wstring path(exePath);
	auto pos = path.find_last_of(L"\\/");
	std::wstring dir = (pos == std::wstring::npos) ? L"." : path.substr(0, pos + 1);
	std::wstring target = dir + L"TgWsProxy_windows.exe";

	if (GetFileAttributesW(target.c_str()) == INVALID_FILE_ATTRIBUTES) {
		return 0;
	}

	STARTUPINFOW si{};
	PROCESS_INFORMATION pi{};
	si.cb = sizeof(si);
	std::wstring cmd = target;
	if (!CreateProcessW(target.c_str(), cmd.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		SHELLEXECUTEINFOW sei{};
		sei.cbSize = sizeof(sei);
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		sei.hwnd = nullptr;
		sei.lpVerb = L"open";
		sei.lpFile = target.c_str();
		sei.lpParameters = nullptr;
		sei.lpDirectory = nullptr;
		sei.nShow = SW_SHOW;
		if (ShellExecuteExW(&sei) && sei.hProcess) {
			DWORD pid = GetProcessId(sei.hProcess);
			CloseHandle(sei.hProcess);
			return pid;
		}
		return 0;
	}

	DWORD pid = pi.dwProcessId;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return pid;
}

static HWND FindMainWindowForPid(DWORD pid)
{
	struct Finder { DWORD pid; HWND result; } f{ pid, nullptr };
	auto enumProc = [](HWND hwnd, LPARAM lparam) -> BOOL {
		Finder* p = reinterpret_cast<Finder*>(lparam);
		DWORD wpid = 0; GetWindowThreadProcessId(hwnd, &wpid);
		if (wpid != p->pid) return TRUE;
		if (!IsWindowVisible(hwnd)) return TRUE;
		int len = GetWindowTextLengthW(hwnd);
		if (len == 0) return TRUE;
		p->result = hwnd;
		return FALSE;
	};
	EnumWindows((WNDENUMPROC)enumProc, (LPARAM)&f);
	return f.result;
}

static void EmbedWindowIntoHost(HWND hostParent, HWND extWnd)
{
	if (!extWnd || !hostParent) return;
	SetParent(extWnd, hostParent);
	LONG_PTR style = GetWindowLongPtrW(extWnd, GWL_STYLE);
	style &= ~(WS_POPUP | WS_BORDER | WS_DLGFRAME);
	style |= WS_CHILD | WS_VISIBLE;
	SetWindowLongPtrW(extWnd, GWL_STYLE, style);
	RECT rc; GetClientRect(hostParent, &rc);
	SetWindowPos(extWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
}

void EmbedTgWsProxyWindow()
{
	DWORD pid = LaunchTgWsProxy();
	if (pid == 0) return;

	HWND ext = nullptr;
	for (int i = 0; i < 50 && ext == nullptr; ++i) {
		ext = FindMainWindowForPid(pid);
		if (ext) break;
		Sleep(100);
	}
	if (!ext) return;

	DWORD mypid = GetCurrentProcessId();
	HWND myMain = nullptr;
	struct Finder2 { DWORD pid; HWND result; } f2{ mypid, nullptr };
	auto enumProc2 = [](HWND hwnd, LPARAM lparam) -> BOOL {
		Finder2* p = reinterpret_cast<Finder2*>(lparam);
		DWORD wpid = 0; GetWindowThreadProcessId(hwnd, &wpid);
		if (wpid != p->pid) return TRUE;
		if (!IsWindowVisible(hwnd)) return TRUE;
		int len = GetWindowTextLengthW(hwnd);
		if (len == 0) return TRUE;
		p->result = hwnd;
		return FALSE;
	};
	EnumWindows((WNDENUMPROC)enumProc2, (LPARAM)&f2);
	myMain = f2.result;
	if (!myMain) return;

	RECT client; GetClientRect(myMain, &client);
	int left = 8; int top = 48; int right = client.right - 8; int bottom = client.bottom - 90;
	int w = right - left; int h = bottom - top;
	HWND host = CreateWindowExW(0, L"Static", L"", WS_CHILD | WS_VISIBLE, left, top, w, h, myMain, NULL, GetModuleHandle(NULL), NULL);
	if (!host) return;

	EmbedWindowIntoHost(host, ext);
}
