#include "pch.h"
#include "aboutWindow.h"
#include "window.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <Windows.h>
#include "../logic/configSystem.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;

void ShowAboutWindow()
{
	static Window aboutWindow{};
	if (aboutWindow) {
		try {
			ActivateOwnedWindow(aboutWindow, L"\u0418\u043D\u0444\u043E\u0440\u043C\u0430\u0446\u0438\u044F \u043E \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u0435");
			return;
		}
		catch (...) {
			aboutWindow = nullptr;
		}
	}
	aboutWindow = Window();

	try { aboutWindow.Title(winrt::hstring(L"\u0418\u043D\u0444\u043E\u0440\u043C\u0430\u0446\u0438\u044F \u043E \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u0435")); }
	catch (...) {}

	Grid grid{};
	int theme = GetSavedTheme();
	grid.RequestedTheme(theme == 1 ? ElementTheme::Dark : ElementTheme::Light);

	winrt::Windows::UI::Color bg{};
	if (theme == 1) { bg.A = 0xFF; bg.R = 0x12; bg.G = 0x12; bg.B = 0x12; }
	else { bg.A = 0xFF; bg.R = 0xFF; bg.G = 0xFF; bg.B = 0xFF; }
	grid.Background(SolidColorBrush{ bg });

		winrt::Microsoft::UI::Xaml::Controls::Pivot pivot{};
		pivot.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
		pivot.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch);
		pivot.Margin({8,8,8,8});

		winrt::Microsoft::UI::Xaml::Controls::PivotItem licenseItem{};
		licenseItem.Header(winrt::box_value(winrt::hstring(L"\u041E \u043F\u0440\u043E\u0435\u043A\u0442\u0435")));
		TextBlock licenseText{};
		licenseText.Text(winrt::hstring(L"Jaylee Client \u043D\u0435 \u044F\u0432\u043B\u044F\u0435\u0442\u0441\u044F \u043E\u0440\u0438\u0433\u0438\u043D\u0430\u043B\u044C\u043D\u044B\u043C \u043F\u0440\u043E\u0435\u043A\u0442\u043E\u043C zapret \u043E\u0442 Flowseal / Bol-van. \u042D\u0442\u043E \u0444\u043E\u0440\u043A \u0441 \u0443\u0434\u043E\u0431\u043D\u044B\u043C \u0433\u0440\u0430\u0444\u0438\u0447\u0435\u0441\u043A\u0438\u043C \u0438\u043D\u0442\u0435\u0440\u0444\u0435\u0439\u0441\u043E\u043C. \u0412\u0441\u0435 \u043F\u0440\u0430\u0432\u0430 \u043D\u0430 Jaylee Client \u0437\u0430\u0449\u0438\u0449\u0435\u043D\u044B \u043B\u0438\u0446\u0435\u043D\u0437\u0438\u0435\u0439\nBy Jaylee Group and OpinionLeaders"));
		licenseText.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::Wrap);
		licenseText.Margin({18,12,18,12});
		licenseText.FontSize(14);
		licenseText.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);

		Grid licenseGrid{};
		RowDefinition rd1{};
		rd1.Height(GridLengthHelper::FromValueAndType(1.0, GridUnitType::Star));
		licenseGrid.RowDefinitions().Append(rd1);
		RowDefinition rd2{};
		rd2.Height(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
		licenseGrid.RowDefinitions().Append(rd2);

		ScrollViewer licenseScroll{};
		licenseScroll.Content(licenseText);
		Grid::SetRow(licenseScroll, 0);
		licenseGrid.Children().Append(licenseScroll);

		TextBlock byText{};
		byText.Text(winrt::hstring(L"By Jaylee Group and OpinionLeaders"));
		byText.FontSize(12);
		byText.Margin({18,6,18,12});
		byText.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left);
		Grid::SetRow(byText, 1);
		licenseGrid.Children().Append(byText);

		licenseItem.Content(licenseGrid);

		winrt::Microsoft::UI::Xaml::Controls::PivotItem versionItem{};
		versionItem.Header(winrt::box_value(winrt::hstring(L"\u0412\u0435\u0440\u0441\u0438\u044F")));
		StackPanel versionPanel{};
		versionPanel.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Vertical);
		versionPanel.Margin({18,24,18,24});
		versionPanel.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);

		TextBlock verLine{};
		verLine.Text(winrt::hstring(L"Version - Jaylee Client V0.7.5"));
		verLine.FontSize(20);
		verLine.Margin({0,0,0,8});
		verLine.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);

		TextBlock buildLine{};
		buildLine.Text(winrt::hstring(L"Build - Patch [Open Beta]"));
		buildLine.FontSize(16);
		buildLine.Opacity(0.9);
		buildLine.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);

		versionPanel.Children().Append(verLine);
		versionPanel.Children().Append(buildLine);
		versionItem.Content(versionPanel);

		pivot.Items().Append(licenseItem);
		pivot.Items().Append(versionItem);

		winrt::Microsoft::UI::Xaml::Controls::PivotItem socialItem{};
		socialItem.Header(winrt::box_value(winrt::hstring(L"\u0421\u043E\u0446\u0441\u0435\u0442\u0438")));
		StackPanel socialPanel{};
		socialPanel.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Vertical);
		socialPanel.Margin({18,12,18,12});

		HyperlinkButton tgBtn{};
		tgBtn.Content(winrt::box_value(winrt::hstring(L"Telegram - \u041C\u044B \u0432 Telegram")));
		tgBtn.NavigateUri(winrt::Windows::Foundation::Uri(winrt::hstring(L"https://t.me/Jaylee_Client")));
		tgBtn.FontSize(14);
		tgBtn.Margin({0,0,0,8});

		HyperlinkButton donationBtn{};
		donationBtn.Content(winrt::box_value(winrt::hstring(L"DonationAlerts - \u041F\u043E\u0434\u0434\u0435\u0440\u0436\u0430\u0442\u044C \u043F\u0440\u043E\u0435\u043A\u0442")));
		donationBtn.NavigateUri(winrt::Windows::Foundation::Uri(winrt::hstring(L"https://dalink.to/jaylee_client")));
		donationBtn.FontSize(14);
		donationBtn.Margin({0,0,0,8});

		TextBlock githubLine{};
		githubLine.Text(winrt::hstring(L"GitHub: (\u0441\u0441\u044B\u043B\u043A\u0430 \u0431\u0443\u0434\u0435\u0442 \u0434\u043E\u0431\u0430\u0432\u043B\u0435\u043D\u0430)"));
		githubLine.FontSize(14);
		githubLine.Margin({0,0,0,8});

		HyperlinkButton flowsealBtn{};
		flowsealBtn.Content(winrt::box_value(winrt::hstring(L"Flowseal - \u0421\u0442\u0440\u0430\u043D\u0438\u0446\u0430 \u0440\u0430\u0437\u0440\u0430\u0431\u043E\u0447\u0438\u043A\u0430 \u043E\u0440\u0438\u0433\u0438\u043D\u0430\u043B\u044C\u043D\u043E\u0439 \u0441\u0431\u043E\u0440\u043A\u0438 Zapret")));
		flowsealBtn.NavigateUri(winrt::Windows::Foundation::Uri(winrt::hstring(L"https://github.com/Flowseal")));
		flowsealBtn.FontSize(14);
		flowsealBtn.Margin({0,0,0,6});

		HyperlinkButton bolvanBtn{};
		bolvanBtn.Content(winrt::box_value(winrt::hstring(L"Bol-van - \u0421\u0442\u0440\u0430\u043D\u0438\u0446\u0430 \u0440\u0430\u0437\u0440\u0430\u0431\u043E\u0447\u0438\u043A\u0430 \u043E\u0440\u0438\u0433\u0438\u043D\u0430\u043B\u044C\u043D\u043E\u0439 \u0441\u0431\u043E\u0440\u043A\u0438 Zapret")));
		bolvanBtn.NavigateUri(winrt::Windows::Foundation::Uri(winrt::hstring(L"https://github.com/Bol-van")));
		bolvanBtn.FontSize(14);
		bolvanBtn.Margin({0,0,0,6});

		socialPanel.Children().Append(tgBtn);
		socialPanel.Children().Append(donationBtn);
		HyperlinkButton githubSimple{};
		githubSimple.Content(winrt::box_value(winrt::hstring(L"GitHub - \u041C\u044B \u043D\u0430 \u043F\u043B\u0430\u0442\u0444\u043E\u0440\u043C\u0435 GitHub")));
		githubSimple.NavigateUri(winrt::Windows::Foundation::Uri(winrt::hstring(L"https://github.com/LeonMagicSkills")));
		githubSimple.FontSize(14);
		githubSimple.Margin({0,0,0,8});
		socialPanel.Children().Append(githubSimple);
		socialPanel.Children().Append(flowsealBtn);
		socialPanel.Children().Append(bolvanBtn);

		socialItem.Content(socialPanel);
		pivot.Items().Append(socialItem);

		grid.Children().Append(pivot);

		aboutWindow.Content(grid);
		ActivateOwnedWindow(aboutWindow, L"\u0418\u043D\u0444\u043E\u0440\u043C\u0430\u0446\u0438\u044F \u043E \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u0435");

		HWND hwnd = FindWindowW(nullptr, L"\u0418\u043D\u0444\u043E\u0440\u043C\u0430\u0446\u0438\u044F \u043E \u043F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u0435");
		if (hwnd) {
			const int width = 520, height = 360;
			SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);

			LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
			style &= ~WS_MAXIMIZEBOX;
			style &= ~WS_MINIMIZEBOX;
			style &= ~WS_THICKFRAME;
			SetWindowLongPtr(hwnd, GWL_STYLE, style);
			SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);

			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu) {
				DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
				DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
				DrawMenuBar(hwnd);
			}
		}
	}
