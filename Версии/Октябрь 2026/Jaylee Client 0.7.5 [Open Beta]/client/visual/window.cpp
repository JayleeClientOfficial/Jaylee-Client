#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "window.h"
#include "buttons.h"
#include "../logic/buttonsLogic.h"
#include "../logic/configSystem.h"
#include "../logic/processController.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.h>
#include <Windows.h>
#include <shellapi.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;

namespace {
HWND g_mainWindowHwnd = nullptr;
}

void RegisterMainWindow(Window const& window, winrt::hstring const& title)
{
	if (!window) return;
	g_mainWindowHwnd = FindWindowW(nullptr, title.c_str());
}

void ActivateOwnedWindow(Window const& window, wchar_t const* title)
{
	if (!window) return;
	window.Activate();

	HWND hwnd = FindWindowW(nullptr, title);
	if (!hwnd) return;

	if (g_mainWindowHwnd && IsWindow(g_mainWindowHwnd) && hwnd != g_mainWindowHwnd) {
		SetWindowLongPtrW(hwnd, GWLP_HWNDPARENT, reinterpret_cast<LONG_PTR>(g_mainWindowHwnd));
	}

	if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);
	else ShowWindow(hwnd, SW_SHOW);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	BringWindowToTop(hwnd);
	SetForegroundWindow(hwnd);
}

Window CreateJayleeWindow(winrt::hstring const& title)
{
	Window window{};
	::RegisterAppWindow(window);

	Grid grid{};
	try {
		int savedTheme = GetSavedTheme();
		if (savedTheme == 1) {
			grid.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Dark);
		} else {
			grid.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);
		}
	} catch (...) {
		grid.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);
	}
	winrt::Windows::UI::Color color{};
	unsigned char rootAlpha = 0xFF;
	if (GetSavedTheme() == 1) {
		color.A = rootAlpha; color.R = 0x12; color.G = 0x12; color.B = 0x12;
	} else {
		color.A = rootAlpha; color.R = 0xFF; color.G = 0xFF; color.B = 0xFF;
	}
	SolidColorBrush brush{ color };
	grid.Background(brush);

	auto buttonsPanel = CreateStrategyButtonsPanel();
	grid.Children().Append(buttonsPanel);

	auto toggleBtn = CreateToggleBypassButton();
	auto bottomPanel = CreateBottomCornerButtonsPanel();
	grid.Children().Append(toggleBtn);
	grid.Children().Append(bottomPanel);

	window.Content(grid);

	try {
		RegisterAppWindow(window);
		RegisterThemeListener(window, [window](int theme) {
			try {
				auto content = window.Content();
				if (content) {
					if (auto fe = content.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>()) {
						fe.RequestedTheme(theme == 1 ? winrt::Microsoft::UI::Xaml::ElementTheme::Dark : winrt::Microsoft::UI::Xaml::ElementTheme::Light);
					}
					if (auto panel = content.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
						winrt::Windows::UI::Color bg{};
						winrt::Windows::UI::Color textCol{};
						winrt::Windows::UI::Color hoverBlue{}; winrt::Windows::UI::Color selectedBlue{}; winrt::Windows::UI::Color outlineCol{};
						unsigned char bgAlpha = 0xFF;
						if (theme == 1) {
							bg.A = bgAlpha; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12;
							textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF;
							hoverBlue.A = 0xFF; hoverBlue.R = 0x2A; hoverBlue.G = 0x78; hoverBlue.B = 0xD8;
							selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
							outlineCol.A = 0xFF; outlineCol.R = 0x44; outlineCol.G = 0x44; outlineCol.B = 0x44;
						} else {
							bg.A = bgAlpha; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF;
							textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00;
							hoverBlue.A = 0xFF; hoverBlue.R = 0x1E; hoverBlue.G = 0x88; hoverBlue.B = 0xE5;
							selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
							outlineCol.A = 0xFF; outlineCol.R = 0xCC; outlineCol.G = 0xCC; outlineCol.B = 0xCC;
						}
						auto normalBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bg };
						auto hoverBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue };
						auto selectedBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedBlue };
						auto outlineBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol };
						auto selectedOutlineBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedBlue };
						auto textNormalBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol };
						winrt::Windows::UI::Color buttonBgCol{}; winrt::Windows::UI::Color buttonBorderCol{};
						unsigned char btnAlpha = 0xFF;
						if (theme == 1) {
							buttonBgCol.A = btnAlpha; buttonBgCol.R = 0x22; buttonBgCol.G = 0x22; buttonBgCol.B = 0x22;
							buttonBorderCol.A = 0xFF; buttonBorderCol.R = 0x33; buttonBorderCol.G = 0x33; buttonBorderCol.B = 0x33;
						} else {
							buttonBgCol.A = btnAlpha; buttonBgCol.R = 0xF5; buttonBgCol.G = 0xF5; buttonBgCol.B = 0xF5;
							buttonBorderCol.A = 0xFF; buttonBorderCol.R = 0xDD; buttonBorderCol.G = 0xDD; buttonBorderCol.B = 0xDD;
						}
						auto btnBgBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ buttonBgCol };
						auto btnBorderBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ buttonBorderCol };

						try {
							if (!panel.Background()) {
								panel.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bg });
							}
						} catch(...) {}

						std::function<void(winrt::Windows::Foundation::IInspectable const&)> walk;
						walk = [&](winrt::Windows::Foundation::IInspectable const& node) {
							try {
								if (!node) return;
								if (auto border = node.try_as<winrt::Microsoft::UI::Xaml::Controls::Border>()) {
									auto child = border.Child();
									if (child) {
										if (auto tb = child.try_as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>()) {
											std::wstring txt = tb.Text().c_str();
											bool isStrategy = (txt.find(L"\u0421\u0442\u0440\u0430\u0442") != std::wstring::npos) || (txt.find(L"TLS") != std::wstring::npos) || (txt.find(L"Strategy") != std::wstring::npos);
											bool isToggle = (txt.find(L"\u043E\u0431\u0445\u043E\u0434") != std::wstring::npos) || (txt.find(L"\u0417\u0430\u043F\u0443\u0441") != std::wstring::npos) || (txt.find(L"\u0417\u0430\u043F\u0440") != std::wstring::npos);
											auto tag = border.Tag();
											bool selected = false;
											winrt::hstring tagStr;
											if (tag) {
												try { selected = winrt::unbox_value<bool>(tag); } catch(...) { try { tagStr = winrt::unbox_value<winrt::hstring>(tag); } catch(...) { } }
											}

											winrt::Windows::UI::Color borderBg = theme == 1 ? winrt::Windows::UI::Color{ 0xFF, 0x1E, 0x1E, 0x1E } : winrt::Windows::UI::Color{ 0xFF, 0xF5, 0xF5, 0xF5 };
											winrt::Windows::UI::Color borderOutline = theme == 1 ? winrt::Windows::UI::Color{ 0xFF, 0x44, 0x44, 0x44 } : winrt::Windows::UI::Color{ 0xFF, 0xDD, 0xDD, 0xDD };
											winrt::Windows::UI::Color textColor = theme == 1 ? winrt::Windows::UI::Color{ 0xFF, 0xFF, 0xFF, 0xFF } : winrt::Windows::UI::Color{ 0xFF, 0x00, 0x00, 0x00 };
											winrt::Windows::UI::Color selectedColor = winrt::Windows::UI::Color{ 0xFF, 0x0B, 0x66, 0xC7 };
											winrt::Windows::UI::Color whiteColor = winrt::Windows::UI::Color{ 0xFF, 0xFF, 0xFF, 0xFF };
											winrt::Windows::UI::Color greenColor = winrt::Windows::UI::Color{ 0xFF, 0x21, 0x96, 0x35 };
											winrt::Windows::UI::Color redColor = winrt::Windows::UI::Color{ 0xFF, 0xE5, 0x39, 0x35 };

											bool appliedSettingsStyle = false;
											try {
												std::wstring ts = tagStr.c_str();
												if (!ts.empty() && ts.rfind(L"settings:", 0) == 0) {
													try { border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderBg }); } catch(...) {}
													try { border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderOutline }); } catch(...) {}
													try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textColor }); } catch(...) {}
													appliedSettingsStyle = true;
												}
											} catch(...) {}

											if (!appliedSettingsStyle && isStrategy) {
												if (selected) {
													try { border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedColor }); } catch(...) {}
													try { border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedColor }); } catch(...) {}
													try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ whiteColor }); } catch(...) {}
												} else {
													try { border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderBg }); } catch(...) {}
													try { border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderOutline }); } catch(...) {}
													try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textColor }); } catch(...) {}
												}
											} else if (!appliedSettingsStyle && isToggle) {
												try { border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selected ? redColor : greenColor }); } catch(...) {}
												try { border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selected ? redColor : greenColor }); } catch(...) {}
												try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ whiteColor }); } catch(...) {}
											} else if (!appliedSettingsStyle) {
												try { border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderBg }); } catch(...) {}
												try { border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderOutline }); } catch(...) {}
												try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textColor }); } catch(...) {}
											}
										} else if (auto innerPanel = child.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
											for (auto const& c : innerPanel.Children()) walk(c);
										}
									}
								}

								if (auto panelNode = node.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
									for (auto const& c : panelNode.Children()) walk(c);
								}
								else if (auto contentCtrl = node.try_as<winrt::Microsoft::UI::Xaml::Controls::ContentControl>()) {
									try {
										auto cont = contentCtrl.Content();
										if (cont) walk(cont);
									} catch(...) {}
								} else if (auto contentPres = node.try_as<winrt::Microsoft::UI::Xaml::Controls::ContentPresenter>()) {
									try {
										auto cont = contentPres.Content();
										if (cont) walk(cont);
									} catch(...) {}
								}
							} catch(...) {}
						};

						try { walk(panel); } catch(...) {}
					}
				}
			} catch(...) {}
		});
	} catch(...) {}

	try { window.Title(title); } catch (...) {}

	window.Activate();
	RegisterMainWindow(window, title);

	const int width = 800;
	const int height = 440;

	auto hTitle = title.c_str();
	HWND hwnd = FindWindowW(nullptr, hTitle);

	if (hwnd)
	{
		RECT rc;
		if (GetWindowRect(hwnd, &rc)) {
			int newX = rc.left;
			int newY = rc.top - 40;
			if (newY < 0) newY = 0;
			SetWindowPos(hwnd, nullptr, newX, newY, width, height, SWP_NOZORDER);
		}
		LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		SetWindowLongPtrW(hwnd, GWL_STYLE, style);

		HMENU hMenu = GetSystemMenu(hwnd, FALSE);
		if (hMenu) {
			EnableMenuItem(hMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
			RemoveMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
		}

		SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
	}

	window.Closed([hwnd, window](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args) {
		if (IsTrayRunning()) {
			args.Handled(true);
			if (hwnd) {
				ShowWindow(hwnd, SW_HIDE);
			}
		} else {
			processcontroller::TerminateChildProcesses();
		}
		try { UnregisterAppWindow(window); } catch(...) {}
	});

	return window;
}
