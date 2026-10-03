#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "settings.h"
#include "window.h"
#include "settingsButtons.h"
#include "../logic/buttonsLogic.h"

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <Windows.h>
#include "../logic/configSystem.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;

static LRESULT CALLBACK SettingsWindowProcImpl(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLOSE) return 0;
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ShowSettingsWindow()
{
    static Window settingsWindow{};
    static bool created = false;
    static Grid settingsGrid{};

    if (!created) {
        settingsWindow = Window();

        settingsGrid = Grid{};
        int theme = GetSavedTheme();
        settingsGrid.RequestedTheme(theme == 1 ? ElementTheme::Dark : ElementTheme::Light);

        winrt::Windows::UI::Color bg{};
        unsigned char rootAlpha = 0xFF;
        if (theme == 1) { bg.A = rootAlpha; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12; }
        else { bg.A = rootAlpha; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
        settingsGrid.Background(SolidColorBrush{ bg });

        {
            auto settingsButtons = CreateSettingsButtonsPanel();
            settingsGrid.Children().Append(settingsButtons);
        }

        settingsWindow.Content(settingsGrid);

        try {
            settingsWindow.Title(winrt::hstring(L"\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438"));
        }
        catch (...) {
        }

        ActivateOwnedWindow(settingsWindow, L"\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438");

        try {
            RegisterAppWindow(settingsWindow);
            RegisterThemeListener(settingsWindow, [=](int theme) {
                try {
                    if (settingsGrid) {
                        try {
                            settingsGrid.RequestedTheme(theme == 1 ? ElementTheme::Dark : ElementTheme::Light);
                        } catch (...) {}

                        unsigned char rootAlpha = 0xFF;
                        winrt::Windows::UI::Color bg{};
                        if (theme == 1) { bg.A = rootAlpha; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12; }
                        else { bg.A = rootAlpha; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
                        try {
                            settingsGrid.Background(SolidColorBrush{ bg });
                        } catch (...) {}
                    }

                    auto content = settingsWindow.Content();
                    if (content) {
                        if (auto fe = content.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>()) {
                            fe.RequestedTheme(theme == 1 ? ElementTheme::Dark : ElementTheme::Light);
                        }
                        if (auto panel = content.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
                            unsigned char rootAlpha = 0xFF;
                            winrt::Windows::UI::Color bg{};
                            if (theme == 1) { bg.A = rootAlpha; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12; }
                            else { bg.A = rootAlpha; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
                            panel.Background(SolidColorBrush{ bg });
                        try {
                            winrt::Windows::UI::Color textCol{}; winrt::Windows::UI::Color hoverBlue{}; winrt::Windows::UI::Color selectedBlue{}; winrt::Windows::UI::Color outlineCol{};
                            if (theme == 1) {
                                textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF;
                                hoverBlue.A = 0xFF; hoverBlue.R = 0x2A; hoverBlue.G = 0x78; hoverBlue.B = 0xD8;
                                selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
                                outlineCol.A = 0xFF; outlineCol.R = 0x33; outlineCol.G = 0x33; outlineCol.B = 0x33;
                            } else {
                                textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00;
                                hoverBlue.A = 0xFF; hoverBlue.R = 0x1E; hoverBlue.G = 0x88; hoverBlue.B = 0xE5;
                                selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
                                outlineCol.A = 0xFF; outlineCol.R = 0xDD; outlineCol.G = 0xDD; outlineCol.B = 0xDD;
                            }
                            auto textNormalBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol };
                            winrt::Windows::UI::Color buttonBgCol{}; winrt::Windows::UI::Color buttonBorderCol{};
                            unsigned char btnAlpha = 0xFF;
                            if (theme == 1) { buttonBgCol.A = btnAlpha; buttonBgCol.R = 0x22; buttonBgCol.G = 0x22; buttonBgCol.B = 0x22; buttonBorderCol.A = 0xFF; buttonBorderCol.R = 0x33; buttonBorderCol.G = 0x33; buttonBorderCol.B = 0x33; }
                            else { buttonBgCol.A = btnAlpha; buttonBgCol.R = 0xF5; buttonBgCol.G = 0xF5; buttonBgCol.B = 0xF5; buttonBorderCol.A = 0xFF; buttonBorderCol.R = 0xDD; buttonBorderCol.G = 0xDD; buttonBorderCol.B = 0xDD; }
                            auto btnBgBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ buttonBgCol };
                            auto btnBorderBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ buttonBorderCol };

                            std::function<void(winrt::Windows::Foundation::IInspectable const&)> walk;
                            walk = [&](winrt::Windows::Foundation::IInspectable const& node) {
                                try {
                                    if (!node) return;
                                    if (auto tbOnly = node.try_as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>()) {
                                        try { tbOnly.Foreground(textNormalBrush); } catch(...) {}
                                    }
                                    if (auto border = node.try_as<winrt::Microsoft::UI::Xaml::Controls::Border>()) {
                                        auto tag = border.Tag();
                                        winrt::hstring tagStr;
                                        if (tag) {
                                            try { tagStr = winrt::unbox_value<winrt::hstring>(tag); } catch(...) {}
                                        }

                                        bool keepStateIndicator = false;
                                        bool stateIsOn = false;
                                        if (!tagStr.empty()) {
                                            std::wstring ts = tagStr.c_str();
                                            if (ts == L"settings:autostart") {
                                                keepStateIndicator = true;
                                                stateIsOn = IsAutorunEnabled();
                                            } else if (ts == L"settings:bgmode") {
                                                keepStateIndicator = true;
                                                stateIsOn = IsTrayRunning();
                                            } else if (ts == L"settings:strategy") {
                                                keepStateIndicator = true;
                                                stateIsOn = IsStrategyAutostartEnabled();
                                            }
                                        }

                                        try {
                                            if (!keepStateIndicator) {
                                                border.Background(btnBgBrush);
                                                border.BorderBrush(btnBorderBrush);
                                            } else {
                                                winrt::Windows::UI::Color stateColor{};
                                                if (theme == 1) {
                                                    stateColor.A = 0xFF; stateColor.R = 0x00; stateColor.G = 0xC8; stateColor.B = 0x00;
                                                    if (!stateIsOn) {
                                                        stateColor.A = 0xFF; stateColor.R = 0xFF; stateColor.G = 0x00; stateColor.B = 0x00;
                                                    }
                                                } else {
                                                    stateColor.A = 0xFF; stateColor.R = 0x00; stateColor.G = 0xA0; stateColor.B = 0x00;
                                                    if (!stateIsOn) {
                                                        stateColor.A = 0xFF; stateColor.R = 0xD9; stateColor.G = 0x1B; stateColor.B = 0x1B;
                                                    }
                                                }
                                                border.Background(btnBgBrush);
                                                border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ stateColor });
                                            }
                                        } catch(...) {}

                                        auto child = border.Child();
                                        if (child) {
                                            if (auto tb = child.try_as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>()) {
                                                bool applied = false;
                                                try {
                                                    std::wstring ts = tagStr.c_str();
                                                    if (!ts.empty() && ts.rfind(L"settings:", 0) == 0) {
                                                        try { tb.Foreground(textNormalBrush); } catch(...) {}
                                                        applied = true;
                                                    }
                                                } catch(...) {}
                                                if (!applied) {
                                                    try { tb.Foreground(textNormalBrush); } catch(...) {}
                                                }
                                            } else if (auto innerPanel = child.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
                                                for (auto const& c : innerPanel.Children()) walk(c);
                                            }
                                        }
                                    }

                                    if (auto border = node.try_as<winrt::Microsoft::UI::Xaml::Controls::Border>()) {
                                        auto tag = border.Tag();
                                        winrt::hstring tagStr;
                                        if (tag) {
                                            try { tagStr = winrt::unbox_value<winrt::hstring>(tag); } catch(...) {}
                                        }
                                        if (!tagStr.empty() && tagStr.c_str() == std::wstring(L"settings:theme")) {
                                            try { border.Background(btnBgBrush); } catch(...) {}
                                            try { border.BorderBrush(btnBorderBrush); } catch(...) {}
                                        }
                                    }
                                    if (auto panelNode = node.try_as<winrt::Microsoft::UI::Xaml::Controls::Panel>()) {
                                        for (auto const& c : panelNode.Children()) walk(c);
                                    }
                                } catch(...) {}
                            };
                            walk(panel);
                        } catch(...) {}
                        }
                    }
                } catch(...) {}
            });
        } catch(...) {}

        HWND hwnd = FindWindowW(nullptr, L"\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438");
        if (hwnd) {
            const int width = 380, height = 360;
            SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);

            LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
            style &= ~WS_MAXIMIZEBOX;
            style &= ~WS_MINIMIZEBOX;
            style &= ~WS_THICKFRAME;
            SetWindowLongPtr(hwnd, GWL_STYLE, style);
            SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);

            HMENU hMenu = GetSystemMenu(hwnd, FALSE);
            if (hMenu) {
                DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
                DrawMenuBar(hwnd);
            }

            static WNDPROC g_prevSettingsWndProc = nullptr;
            static bool g_subclassed = false;
            if (!g_subclassed) {
                g_prevSettingsWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)SettingsWindowProcImpl);
                g_subclassed = true;
            }
        }

        created = true;
        return;
    }



    try {
        ActivateOwnedWindow(settingsWindow, L"\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438");
    }
    catch (...) {}
}

