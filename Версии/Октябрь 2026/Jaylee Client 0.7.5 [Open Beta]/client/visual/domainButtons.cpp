#include "pch.h"
#include "domainButtons.h"
#include "../logic/buttonsLogic.h"

#include <winrt/Microsoft.UI.Xaml.Controls.h>

using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;

winrt::Microsoft::UI::Xaml::UIElement CreateDomainButtonsPanel()
{
	StackPanel buttons{};
	buttons.Orientation(Orientation::Vertical);
	buttons.Spacing(12);
	buttons.HorizontalAlignment(HorizontalAlignment::Center);
	buttons.VerticalAlignment(VerticalAlignment::Center);
	buttons.Margin({20, 0, 20, 0});

	const wchar_t* labels[] = {
		L"\u041E\u0441\u043D\u043E\u0432\u043D\u044B\u0435 \u0434\u043E\u043C\u0435\u043D\u044B",
		L"\u0413\u0443\u0433\u043B \u0434\u043E\u043C\u0435\u043D\u044B",
		L"\u0413\u043B\u043E\u0431\u0430\u043B\u044C\u043D\u044B\u0435 \u0434\u043E\u043C\u0435\u043D\u044B"
	};
	const wchar_t* files[] = {
		L"list-general.txt",
		L"list-google.txt",
		L"list-exclude.txt"
	};
	for (int i = 0; i < 3; ++i) {
		Button button{};
		button.Content(winrt::box_value(winrt::hstring(labels[i])));
		button.Width(280);
		button.Height(44);
		const wchar_t* fileName = files[i];
		button.Click([fileName](auto&&, auto&&) {
			OpenDomainListFromLogic(fileName);
		});
		buttons.Children().Append(button);
	}

	return buttons;
}
