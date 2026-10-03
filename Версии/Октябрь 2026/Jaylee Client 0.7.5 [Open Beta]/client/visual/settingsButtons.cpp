#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "settingsButtons.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <Windows.h>

#include "../logic/buttonsLogic.h"
#include "../logic/configSystem.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;

winrt::Microsoft::UI::Xaml::UIElement CreateSettingsButtonsPanel()
{
	Grid container{};
	container.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
	container.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch);
	container.Margin({8,8,8,8});

	int theme = GetSavedTheme();
	winrt::Windows::UI::Color containerBg{};
	winrt::Windows::UI::Color buttonBg{};
	winrt::Windows::UI::Color buttonBorder{};
	winrt::Windows::UI::Color textColor{};
	winrt::Windows::UI::Color redColor{}; redColor.A = 0xFF; redColor.R = 0xFF; redColor.G = 0x00; redColor.B = 0x00;
	winrt::Windows::UI::Color greenColor{}; greenColor.A = 0xFF; greenColor.R = 0x00; greenColor.G = 0xC8; greenColor.B = 0x00;

	unsigned char containerAlpha = 0xFF;
	if (theme == 1) {
		containerBg.A = 0xFF; containerBg.R = 0x12; containerBg.G = 0x12; containerBg.B = 0x12;
		buttonBg.A = 0xFF; buttonBg.R = 0x22; buttonBg.G = 0x22; buttonBg.B = 0x22;
		buttonBorder.A = 0xFF; buttonBorder.R = 0x33; buttonBorder.G = 0x33; buttonBorder.B = 0x33;
		textColor.A = 0xFF; textColor.R = 0xFF; textColor.G = 0xFF; textColor.B = 0xFF;
	} else {
		containerBg.A = 0xFF; containerBg.R = 0xFF; containerBg.G = 0xFF; containerBg.B = 0xFF;
		buttonBg.A = 0xFF; buttonBg.R = 0xF5; buttonBg.G = 0xF5; buttonBg.B = 0xF5;
		buttonBorder.A = 0xFF; buttonBorder.R = 0xDD; buttonBorder.G = 0xDD; buttonBorder.B = 0xDD;
		textColor.A = 0xFF; textColor.R = 0x00; textColor.G = 0x00; textColor.B = 0x00;
	}

	containerBg.A = containerAlpha;
	auto containerBgBrush = SolidColorBrush{ containerBg };
	auto btnBgBrush = SolidColorBrush{ buttonBg };
	auto btnBorderBrush = SolidColorBrush{ buttonBorder };
	auto textBrush = SolidColorBrush{ textColor };
	auto redBrush = SolidColorBrush{ redColor };
	auto greenBrush = SolidColorBrush{ greenColor };

	Border btnBorder{};
	btnBorder.Tag(winrt::box_value(winrt::hstring(L"settings:autostart")));
	btnBorder.CornerRadius({6,6,6,6});
	btnBorder.Padding({10,6,10,6});
	btnBorder.Background(btnBgBrush);
	bool startupPresent = IsAutorunEnabled();
	if (startupPresent) {
		btnBorder.BorderBrush(greenBrush);
	} else {
		btnBorder.BorderBrush(redBrush);
	}
	btnBorder.BorderThickness({1,1,1,1});

	TextBlock tb{};
	tb.Text(winrt::hstring(L"\u0414\u043E\u0431\u0430\u0432\u0438\u0442\u044C \u0432 \u0430\u0432\u0442\u043E\u0437\u0430\u0433\u0440\u0443\u0437\u043A\u0443"));
	tb.FontSize(12);
	tb.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	tb.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	tb.Foreground(textBrush);
	tb.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::NoWrap);
	btnBorder.MaxWidth(260.0);

	btnBorder.Child(tb);

	btnBorder.PointerPressed([=](winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		auto border = sender.as<Border>();
		bool newState = ToggleAutorun();
		if (newState) {
			border.BorderBrush(greenBrush);
		} else {
			border.BorderBrush(redBrush);
		}
	});

	Grid topGrid{};
	topGrid.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
	topGrid.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
	topGrid.Margin({0,6,0,0});

	ColumnDefinition col1{};
	ColumnDefinition col2{};
	col1.Width(GridLength(1.0, GridUnitType::Star));
	col2.Width(GridLength(1.0, GridUnitType::Star));
	topGrid.ColumnDefinitions().Append(col1);
	topGrid.ColumnDefinitions().Append(col2);

	StackPanel leftPanel{};
	leftPanel.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal);
	leftPanel.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	leftPanel.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
	leftPanel.Margin({0,20,0,0});

	btnBorder.Margin({0,0,8,0});
	btnBorder.MaxWidth(160.0);

	leftPanel.Children().Append(btnBorder);

	{
		Border bgModeBorder{};
		bgModeBorder.Tag(winrt::box_value(winrt::hstring(L"settings:bgmode")));
		bgModeBorder.CornerRadius({6,6,6,6});
		bgModeBorder.Padding({8,4,8,4});
		bgModeBorder.Background(btnBgBrush);
		bgModeBorder.BorderBrush(btnBorderBrush);
		bgModeBorder.BorderThickness({1,1,1,1});
		bgModeBorder.Margin({6,0,0,0});
		bgModeBorder.MinWidth(80.0);

		TextBlock tbBg{};
		tbBg.Text(winrt::hstring(L"\u0424\u043E\u043D\u043E\u0432\u044B\u0439 \u0440\u0435\u0436\u0438\u043C"));
		tbBg.FontSize(13);
		tbBg.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
		tbBg.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
		tbBg.Foreground(textBrush);

		bgModeBorder.Child(tbBg);


		bool trayRunning = IsTrayRunning();
		if (trayRunning) {
			bgModeBorder.BorderBrush(greenBrush);
		} else {
			bgModeBorder.BorderBrush(redBrush);
		}

		bgModeBorder.PointerPressed([=](winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
			auto border = sender.as<Border>();
			bool newState = ToggleTray();
			if (newState) {
				border.BorderBrush(greenBrush);
			} else {
				border.BorderBrush(redBrush);
			}
		});

		leftPanel.Children().Append(bgModeBorder);




	}



	StackPanel centerColumn{};
	centerColumn.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Vertical);
	centerColumn.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	centerColumn.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
	centerColumn.Margin({0,0,0,0});

	centerColumn.Children().Append(leftPanel);

	Border strategyBorder{};
	strategyBorder.Tag(winrt::box_value(winrt::hstring(L"settings:strategy")));
	strategyBorder.CornerRadius({6,6,6,6});
	strategyBorder.Padding({10,6,10,6});
	strategyBorder.Background(btnBgBrush);
	bool stratEnabled = IsStrategyAutostartEnabled();
	if (stratEnabled) {
		strategyBorder.BorderBrush(greenBrush);
	} else {
		strategyBorder.BorderBrush(redBrush);
	}
	strategyBorder.BorderThickness({1,1,1,1});
	strategyBorder.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
	strategyBorder.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
	strategyBorder.Margin({0,12,0,0});
	strategyBorder.MinHeight(36.0);
	strategyBorder.MaxWidth(360.0);

	TextBlock tbStrategy{};
	tbStrategy.Text(winrt::hstring(L"\u0410\u0432\u0442\u043E\u0437\u0430\u043F\u0443\u0441\u043A \u0441\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u0438"));
	tbStrategy.FontSize(13);
	tbStrategy.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	tbStrategy.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);

	tbStrategy.Foreground(textBrush);

	strategyBorder.Child(tbStrategy);

	strategyBorder.PointerPressed([=](winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		auto border = sender.as<Border>();
		bool newState = ToggleStrategyAutostart();
		if (newState) {
			border.BorderBrush(greenBrush);
		} else {
			border.BorderBrush(redBrush);
		}
	});

	centerColumn.Children().Append(strategyBorder);

	{
		Border themeBorder{};
		themeBorder.Tag(winrt::box_value(winrt::hstring(L"settings:theme")));
		themeBorder.CornerRadius({6,6,6,6});
		themeBorder.Padding({8,4,8,4});
		themeBorder.Background(btnBgBrush);
		themeBorder.BorderBrush(btnBorderBrush);
		themeBorder.BorderThickness({1,1,1,1});
		themeBorder.Margin({0,8,0,0});
		themeBorder.MinWidth(260.0);

	TextBlock tbTheme{};
		int curTheme = GetSavedTheme();
		winrt::hstring name = (curTheme == 1) ? winrt::hstring(L"\u0422\u0451\u043C\u043D\u0430\u044F") : winrt::hstring(L"\u0421\u0432\u0435\u0442\u043B\u0430\u044F");
		tbTheme.Text(winrt::hstring(L"\u0422\u0435\u043C\u0430: ") + name);
		tbTheme.FontSize(13);
		tbTheme.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
		tbTheme.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	tbTheme.Foreground(textBrush);
	themeBorder.Child(tbTheme);

		MenuFlyout mf{};
		MenuFlyoutItem itemLight{};
		itemLight.Text(winrt::hstring(L"\u0421\u0432\u0435\u0442\u043B\u0430\u044F"));
		itemLight.Click([tbTheme, themeBorder](auto const&, auto const&) {
			ApplyTheme(0);
			tbTheme.Text(winrt::hstring(L"\u0422\u0435\u043C\u0430: ") + winrt::hstring(L"\u0421\u0432\u0435\u0442\u043B\u0430\u044F"));
			winrt::Windows::UI::Color bg{}; bg.A = 0xFF; bg.R = 0xF5; bg.G = 0xF5; bg.B = 0xF5;
			winrt::Windows::UI::Color border{}; border.A = 0xFF; border.R = 0xDD; border.G = 0xDD; border.B = 0xDD;
			winrt::Windows::UI::Color text{}; text.A = 0xFF; text.R = 0x00; text.G = 0x00; text.B = 0x00;
			themeBorder.Background(SolidColorBrush{ bg });
			themeBorder.BorderBrush(SolidColorBrush{ border });
			tbTheme.Foreground(SolidColorBrush{ text });
		});
		MenuFlyoutItem itemDark{};
		itemDark.Text(winrt::hstring(L"\u0422\u0451\u043C\u043D\u0430\u044F"));
		itemDark.Click([tbTheme, themeBorder](auto const&, auto const&) {
			ApplyTheme(1);
			tbTheme.Text(winrt::hstring(L"\u0422\u0435\u043C\u0430: ") + winrt::hstring(L"\u0422\u0451\u043C\u043D\u0430\u044F"));
			winrt::Windows::UI::Color bg{}; bg.A = 0xFF; bg.R = 0x22; bg.G = 0x22; bg.B = 0x22;
			winrt::Windows::UI::Color border{}; border.A = 0xFF; border.R = 0x33; border.G = 0x33; border.B = 0x33;
			winrt::Windows::UI::Color text{}; text.A = 0xFF; text.R = 0xFF; text.G = 0xFF; text.B = 0xFF;
			themeBorder.Background(SolidColorBrush{ bg });
			themeBorder.BorderBrush(SolidColorBrush{ border });
			tbTheme.Foreground(SolidColorBrush{ text });
		});

		mf.Items().Append(itemLight);
		mf.Items().Append(itemDark);

		themeBorder.ContextFlyout(mf);
		themeBorder.PointerPressed([mf, itemLight, itemDark](winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
			auto fe = sender.as<FrameworkElement>();
			double width = 0.0;
			try { width = fe.ActualWidth(); } catch(...) { width = 140.0; }
			try { itemLight.MinWidth(width); } catch(...) {}
			try { itemDark.MinWidth(width); } catch(...) {}
			winrt::Windows::Foundation::Point pt{};
			pt.X = 0.0;
			pt.Y = static_cast<float>(fe.ActualHeight());
			mf.ShowAt(fe, pt);
		});

		centerColumn.Children().Append(themeBorder);
	}

	container.Children().Append(centerColumn);

	Border closeBorder{};
	closeBorder.Tag(winrt::box_value(winrt::hstring(L"settings:close")));
	closeBorder.CornerRadius({6,6,6,6});
	closeBorder.Padding({10,6,10,6});
	closeBorder.Background(btnBgBrush);
	closeBorder.BorderBrush(btnBorderBrush);
	closeBorder.BorderThickness({1,1,1,1});
	closeBorder.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	closeBorder.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom);
	closeBorder.Margin({0,0,0,12});

	TextBlock tbClose{};
	tbClose.Text(winrt::hstring(L"\u0417\u0430\u043A\u0440\u044B\u0442\u044C"));
	tbClose.FontSize(13);
	tbClose.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	tbClose.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	tbClose.Foreground(textBrush);

	closeBorder.Child(tbClose);

	closeBorder.PointerPressed([](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		HWND hwnd = FindWindowW(nullptr, L"\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438");
		if (hwnd) {
			ShowWindow(hwnd, SW_HIDE);
		}
	});

	container.Children().Append(closeBorder);

	return container;
}
