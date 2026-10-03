#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "buttons.h"
#include "../logic/buttonsLogic.h"
#include "../logic/configSystem.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <string>

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Windows::UI;

namespace {
	static void ApplyStrategyButtonTheme(Border const& border, TextBlock const& tb, bool selected, int theme)
	{
		winrt::Windows::UI::Color bgCol{};
		winrt::Windows::UI::Color textCol{};
		winrt::Windows::UI::Color borderCol{};
		winrt::Windows::UI::Color selectedBlue{};
		if (theme == 1) {
			bgCol.A = 0xFF; bgCol.R = 0x1E; bgCol.G = 0x1E; bgCol.B = 0x1E;
			textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF;
			borderCol.A = 0xFF; borderCol.R = 0x44; borderCol.G = 0x44; borderCol.B = 0x44;
			selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
		} else {
			bgCol.A = 0xFF; bgCol.R = 0xF5; bgCol.G = 0xF5; bgCol.B = 0xF5;
			textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00;
			borderCol.A = 0xFF; borderCol.R = 0xCC; borderCol.G = 0xCC; borderCol.B = 0xCC;
			selectedBlue.A = 0xFF; selectedBlue.R = 0x0B; selectedBlue.G = 0x66; selectedBlue.B = 0xC7;
		}

		if (selected) {
			border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedBlue });
			border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selectedBlue });
			tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ winrt::Windows::UI::Color{0xFF, 0xFF, 0xFF, 0xFF} });
		} else {
			border.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
			border.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ borderCol });
			tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol });
		}
	}
}

winrt::Microsoft::UI::Xaml::UIElement CreateStrategyButtonsPanel()
{
	StackPanel container{};
	container.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Vertical);

	Grid grid{};
	for (int c = 0; c < 5; ++c) {
		ColumnDefinition cd{};
		cd.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
		grid.ColumnDefinitions().Append(cd);
	}
	for (int r = 0; r < 4; ++r) {
		RowDefinition rd{};
		rd.Height(GridLengthHelper::FromValueAndType(1.0, GridUnitType::Star));
		grid.RowDefinitions().Append(rd);
	}

	Grid tlsGrid{};
	for (int i = 0; i < 4; ++i) {
		ColumnDefinition cd{};
		cd.Width(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
		tlsGrid.ColumnDefinitions().Append(cd);
	}
	for (int i = 0; i < 2; ++i) {
		RowDefinition rd{};
		rd.Height(GridLengthHelper::FromValueAndType(0.0, GridUnitType::Auto));
		tlsGrid.RowDefinitions().Append(rd);
	}
	tlsGrid.HorizontalAlignment(HorizontalAlignment::Left);
	tlsGrid.Margin({2,0,0,0});


	int theme = GetSavedTheme();
	Color bgCol{}; Color textCol{}; Color hoverBlue{}; Color selectedBlue{}; Color outlineCol{};
	if (theme == 1) {
		bgCol.A = 0xFF; bgCol.R = 0x2D; bgCol.G = 0x2D; bgCol.B = 0x2D;
		textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF;
		hoverBlue.A = 0xFF; hoverBlue.R = 0x3F; hoverBlue.G = 0x3F; hoverBlue.B = 0x3F;
		selectedBlue.A = 0xFF; selectedBlue.R = 0x1F; selectedBlue.G = 0x1F; selectedBlue.B = 0x1F;
		outlineCol.A = 0xFF; outlineCol.R = 0x88; outlineCol.G = 0x88; outlineCol.B = 0x88;
	} else {
		bgCol.A = 0xFF; bgCol.R = 0xF0; bgCol.G = 0xF0; bgCol.B = 0xF0;
		textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00;
		hoverBlue.A = 0xFF; hoverBlue.R = 0xE0; hoverBlue.G = 0xE0; hoverBlue.B = 0xE0;
		selectedBlue.A = 0xFF; selectedBlue.R = 0xD0; selectedBlue.G = 0xD0; selectedBlue.B = 0xD0;
		outlineCol.A = 0xFF; outlineCol.R = 0x88; outlineCol.G = 0x88; outlineCol.B = 0x88;
	}

	SolidColorBrush normalBrush{ bgCol };
	SolidColorBrush hoverBrush{ hoverBlue };
	SolidColorBrush selectedBrush{ selectedBlue };
	SolidColorBrush textNormalBrush{ textCol };
	SolidColorBrush textSelectedBrush{ bgCol };
	SolidColorBrush outlineBrush{ outlineCol };
	SolidColorBrush selectedOutlineBrush{ selectedBlue };

	int index = 0;
	int currentSelected = GetSelectedStrategy();

	for (int r = 0; r < 3; ++r) {
		for (int c = 0; c < 4; ++c) {
			++index;
			
			Border btn{};
			std::wstring label;
			if (index <= 12) {
				label = std::wstring(L"\u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F ") + std::to_wstring(index);
			} else {
				label = std::wstring(L"\u0054\u004C\u0053 \u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F ") + std::to_wstring(index - 12);
			}

			btn.Margin({ 4,4,4,4 });
			btn.Tag(winrt::box_value(false));
			btn.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
			btn.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch);
			btn.Width(110);
			btn.Height(36);
			btn.MinWidth(72);
			btn.MinHeight(32);
			btn.Padding({ 6,3,6,3 });
			btn.CornerRadius({ 4,4,4,4 });
			winrt::Windows::UI::Color btnBgColOpaque = bgCol; btnBgColOpaque.A = 0xFF;
			winrt::Windows::UI::Color outlineColOpaque = outlineCol; outlineColOpaque.A = 0xFF;
			btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ btnBgColOpaque });
			btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineColOpaque });
			btn.BorderThickness({ 2,2,2,2 });

			TextBlock tb{};
			tb.Text(winrt::hstring(label));
			tb.HorizontalAlignment(HorizontalAlignment::Center);
			tb.VerticalAlignment(VerticalAlignment::Center);
			tb.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::NoWrap);
			tb.FontSize(13);
			tb.Foreground(textNormalBrush);
			btn.Child(tb);

			btn.PointerEntered([btn, tb](auto&&, auto&&) {
				try {
					auto tag = btn.Tag();
					bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
					if (!selected) {
						int themeNow = GetSavedTheme();
						winrt::Windows::UI::Color hoverBlue{}; hoverBlue.A = 0xFF;
						if (themeNow == 1) { hoverBlue.R = 0x2A; hoverBlue.G = 0x78; hoverBlue.B = 0xD8; }
						else { hoverBlue.R = 0x1E; hoverBlue.G = 0x88; hoverBlue.B = 0xE5; }
						btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
						btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
						winrt::Windows::UI::Color bgCol{};
						if (themeNow == 1) { bgCol.A = 0xFF; bgCol.R = 0x1E; bgCol.G = 0x1E; bgCol.B = 0x1E; }
						else { bgCol.A = 0xFF; bgCol.R = 0xFF; bgCol.G = 0xFF; bgCol.B = 0xFF; }
						tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
					}
				} catch(...) {}
			});

			btn.PointerExited([btn, tb](auto&&, auto&&) {
				try {
					auto tag = btn.Tag();
					bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
					ApplyStrategyButtonTheme(btn, tb, selected, GetSavedTheme());
				} catch(...) {}
			});

			btn.PointerPressed([grid, tlsGrid, btn, tb, index](auto&&, auto&&) {
				try {
					auto tag = btn.Tag();
					bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
					bool newSel = !selected;
					int themeNow = GetSavedTheme();
					winrt::Windows::UI::Color bgCol{}; winrt::Windows::UI::Color textCol{}; winrt::Windows::UI::Color selBlue{}; winrt::Windows::UI::Color outlineCol{};
					if (themeNow == 1) { bgCol.A = 0xFF; bgCol.R = 0x1E; bgCol.G = 0x1E; bgCol.B = 0x1E; textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF; selBlue.A = 0xFF; selBlue.R = 0x0B; selBlue.G = 0x66; selBlue.B = 0xC7; outlineCol.A = 0xFF; outlineCol.R = 0x44; outlineCol.G = 0x44; outlineCol.B = 0x44; }
					else { bgCol.A = 0xFF; bgCol.R = 0xFF; bgCol.G = 0xFF; bgCol.B = 0xFF; textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00; selBlue.A = 0xFF; selBlue.R = 0x0B; selBlue.G = 0x66; selBlue.B = 0xC7; outlineCol.A = 0xFF; outlineCol.R = 0xCC; outlineCol.G = 0xCC; outlineCol.B = 0xCC; }

					for (auto const& child : grid.Children()) {
						if (auto otherBtn = child.try_as<Border>()) {
							otherBtn.Tag(winrt::box_value(false));
							try { otherBtn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol }); } catch(...) {}
							try { otherBtn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol }); } catch(...) {}
							if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
								try { tbInner.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol }); } catch(...) {}
							}
						}
					}

					for (auto const& child : tlsGrid.Children()) {
						if (auto otherBtn = child.try_as<Border>()) {
							otherBtn.Tag(winrt::box_value(false));
							try { otherBtn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol }); } catch(...) {}
							try { otherBtn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol }); } catch(...) {}
							if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
								try { tbInner.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol }); } catch(...) {}
							}
						}
					}

					if (newSel) {
						btn.Tag(winrt::box_value(true));
						try { btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selBlue }); } catch(...) {}
						try { btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selBlue }); } catch(...) {}
						try { tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol }); } catch(...) {}
						SelectStrategy(index);
					} else {
						SelectStrategy(0);
					}
				} catch(...) {}
			});

			if (currentSelected == index) {
				btn.Tag(winrt::box_value(true));
				ApplyStrategyButtonTheme(btn, tb, true, GetSavedTheme());
			} else {
				btn.Tag(winrt::box_value(false));
				ApplyStrategyButtonTheme(btn, tb, false, GetSavedTheme());
			}

			Grid::SetRow(btn, r);
			Grid::SetColumn(btn, c);
			grid.Children().Append(btn);
		}
	}

	Border strategy13Btn{};
	strategy13Btn.Margin({ 4,4,4,4 });
	strategy13Btn.Tag(winrt::box_value(false));
	strategy13Btn.HorizontalAlignment(HorizontalAlignment::Stretch);
	strategy13Btn.VerticalAlignment(VerticalAlignment::Stretch);
	strategy13Btn.Width(110);
	strategy13Btn.Height(36);
	strategy13Btn.MinWidth(72);
	strategy13Btn.MinHeight(32);
	strategy13Btn.Padding({ 6,3,6,3 });
	strategy13Btn.CornerRadius({ 4,4,4,4 });
	strategy13Btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
	strategy13Btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol });
	strategy13Btn.BorderThickness({ 2,2,2,2 });
	TextBlock strategy13Tb{};
	strategy13Tb.Text(std::wstring(L"\u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F 13"));
	strategy13Tb.HorizontalAlignment(HorizontalAlignment::Center);
	strategy13Tb.VerticalAlignment(VerticalAlignment::Center);
	strategy13Tb.TextWrapping(TextWrapping::NoWrap);
	strategy13Tb.FontSize(13);
	strategy13Tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol });
	strategy13Btn.Child(strategy13Tb);
	strategy13Btn.PointerEntered([strategy13Btn, strategy13Tb](auto&&, auto&&) {
		try {
			auto tag = strategy13Btn.Tag();
			bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
			if (!selected) {
				int themeNow = GetSavedTheme();
				winrt::Windows::UI::Color hoverBlue{}; hoverBlue.A = 0xFF;
				if (themeNow == 1) { hoverBlue.R = 0x2A; hoverBlue.G = 0x78; hoverBlue.B = 0xD8; }
				else { hoverBlue.R = 0x1E; hoverBlue.G = 0x88; hoverBlue.B = 0xE5; }
				strategy13Btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
				strategy13Btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
				winrt::Windows::UI::Color hoverText{};
				if (themeNow == 1) { hoverText.A = 0xFF; hoverText.R = 0x1E; hoverText.G = 0x1E; hoverText.B = 0x1E; }
				else { hoverText.A = 0xFF; hoverText.R = 0xFF; hoverText.G = 0xFF; hoverText.B = 0xFF; }
				strategy13Tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverText });
			}
		} catch(...) {}
	});
	strategy13Btn.PointerExited([strategy13Btn, strategy13Tb](auto&&, auto&&) {
		try {
			auto tag = strategy13Btn.Tag();
			bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
			ApplyStrategyButtonTheme(strategy13Btn, strategy13Tb, selected, GetSavedTheme());
		} catch(...) {}
	});
	strategy13Btn.PointerPressed([grid, tlsGrid, strategy13Btn, strategy13Tb](auto&&, auto&&) {
		try {
			bool selected = GetSelectedStrategy() == 13;
			bool newSel = !selected;
			int themeNow = GetSavedTheme();
			winrt::Windows::UI::Color bgCol{}; winrt::Windows::UI::Color textCol{}; winrt::Windows::UI::Color selBlue{}; winrt::Windows::UI::Color outlineCol{};
			if (themeNow == 1) { bgCol.A = 0xFF; bgCol.R = 0x1E; bgCol.G = 0x1E; bgCol.B = 0x1E; textCol.A = 0xFF; textCol.R = 0xFF; textCol.G = 0xFF; textCol.B = 0xFF; selBlue.A = 0xFF; selBlue.R = 0x0B; selBlue.G = 0x66; selBlue.B = 0xC7; outlineCol.A = 0xFF; outlineCol.R = 0x44; outlineCol.G = 0x44; outlineCol.B = 0x44; }
			else { bgCol.A = 0xFF; bgCol.R = 0xFF; bgCol.G = 0xFF; bgCol.B = 0xFF; textCol.A = 0xFF; textCol.R = 0x00; textCol.G = 0x00; textCol.B = 0x00; selBlue.A = 0xFF; selBlue.R = 0x0B; selBlue.G = 0x66; selBlue.B = 0xC7; outlineCol.A = 0xFF; outlineCol.R = 0xCC; outlineCol.G = 0xCC; outlineCol.B = 0xCC; }

			for (auto const& child : grid.Children()) {
				if (auto otherBtn = child.try_as<Border>()) {
					otherBtn.Tag(winrt::box_value(false));
					otherBtn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
					otherBtn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol });
					if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
						tbInner.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol });
					}
				}
			}
			for (auto const& child : tlsGrid.Children()) {
				if (auto otherBtn = child.try_as<Border>()) {
					otherBtn.Tag(winrt::box_value(false));
					otherBtn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
					otherBtn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ outlineCol });
					if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
						tbInner.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ textCol });
					}
				}
			}

			if (newSel) {
				strategy13Btn.Tag(winrt::box_value(true));
				strategy13Btn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selBlue });
				strategy13Btn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ selBlue });
				strategy13Tb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
				SelectStrategy(13);
			} else {
				SelectStrategy(0);
			}
		} catch(...) {}
	});
	strategy13Btn.Tag(winrt::box_value(currentSelected == 13));
	ApplyStrategyButtonTheme(strategy13Btn, strategy13Tb, currentSelected == 13, GetSavedTheme());
	Grid::SetRow(strategy13Btn, 0);
	Grid::SetColumn(strategy13Btn, 4);
	grid.Children().Append(strategy13Btn);

	auto makeTlsButton = [=](int tlsIndex, const std::wstring &labelText) {
		Border tbtn{};
		tbtn.Margin({4,4,4,4});
		tbtn.Tag(winrt::box_value(false));
		tbtn.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Stretch);
		tbtn.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Stretch);
		tbtn.Width(140);
		tbtn.Height(36);
		tbtn.MinWidth(72);
		tbtn.MinHeight(32);
		tbtn.Padding({6,3,6,3});
		tbtn.CornerRadius({4,4,4,4});
		tbtn.Background(normalBrush);
		tbtn.BorderBrush(outlineBrush);
		tbtn.BorderThickness({1,1,1,1});

		TextBlock ctb{};
		ctb.Text(winrt::hstring(labelText));
		ctb.HorizontalAlignment(HorizontalAlignment::Center);
		ctb.VerticalAlignment(VerticalAlignment::Center);
		ctb.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::NoWrap);
		ctb.FontSize(13);
		ctb.Foreground(textNormalBrush);
		tbtn.Child(ctb);

		tbtn.PointerEntered([tbtn, ctb](auto&&, auto&&) {
			try {
				auto tag = tbtn.Tag();
				bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
				if (!selected) {
					int themeNow = GetSavedTheme();
					winrt::Windows::UI::Color hoverBlue{}; hoverBlue.A = 0xFF;
					if (themeNow == 1) { hoverBlue.R = 0x2A; hoverBlue.G = 0x78; hoverBlue.B = 0xD8; }
					else { hoverBlue.R = 0x1E; hoverBlue.G = 0x88; hoverBlue.B = 0xE5; }
					tbtn.Background(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
					tbtn.BorderBrush(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ hoverBlue });
					winrt::Windows::UI::Color bgCol{};
					if (themeNow == 1) { bgCol.A = 0xFF; bgCol.R = 0x1E; bgCol.G = 0x1E; bgCol.B = 0x1E; }
					else { bgCol.A = 0xFF; bgCol.R = 0xFF; bgCol.G = 0xFF; bgCol.B = 0xFF; }
					ctb.Foreground(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ bgCol });
				}
			} catch(...) {}
		});

		tbtn.PointerExited([tbtn, ctb](auto&&, auto&&) {
			try {
				auto tag = tbtn.Tag();
				bool selected = tag ? winrt::unbox_value<bool>(tag) : false;
				ApplyStrategyButtonTheme(tbtn, ctb, selected, GetSavedTheme());
			} catch(...) {}
		});

		tbtn.PointerPressed([=](winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
			try {
				if (auto selfBtn = sender.try_as<Border>()) {
					bool selected = GetSelectedStrategy() == tlsIndex;
					int themeNow = GetSavedTheme();
					for (auto const& child : grid.Children()) {
						if (auto otherBtn = child.try_as<Border>()) {
							otherBtn.Tag(winrt::box_value(false));
							if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
								ApplyStrategyButtonTheme(otherBtn, tbInner, false, themeNow);
							}
						}
					}
					for (auto const& child : tlsGrid.Children()) {
						if (auto otherBtn = child.try_as<Border>()) {
							otherBtn.Tag(winrt::box_value(false));
							if (auto tbInner = otherBtn.Child().try_as<TextBlock>()) {
								ApplyStrategyButtonTheme(otherBtn, tbInner, false, themeNow);
							}
						}
					}

					if (!selected) {
						selfBtn.Tag(winrt::box_value(true));
						if (auto tbInner = selfBtn.Child().try_as<TextBlock>()) {
							ApplyStrategyButtonTheme(selfBtn, tbInner, true, themeNow);
						}
						SelectStrategy(tlsIndex);
					} else {
						SelectStrategy(0);
					}
				}
			} catch(...) {}
		});

		bool selected = GetSelectedStrategy() == tlsIndex;
		tbtn.Tag(winrt::box_value(selected));
		ApplyStrategyButtonTheme(tbtn, ctb, selected, GetSavedTheme());

		return tbtn;
	};

	{
		auto t1 = makeTlsButton(16, std::wstring(L"\u0054\u004C\u0053 \u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F 1"));
		Grid::SetColumn(t1, 0);
		Grid::SetRow(t1, 0);
		tlsGrid.Children().Append(t1);
		auto expButton = makeTlsButton(17, std::wstring(L"\u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F EXP"));
		Grid::SetColumn(expButton, 0);
		Grid::SetRow(expButton, 1);
		tlsGrid.Children().Append(expButton);
		auto standardButton = makeTlsButton(18, std::wstring(L"\u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F standart"));
		Grid::SetColumn(standardButton, 1);
		Grid::SetRow(standardButton, 1);
		tlsGrid.Children().Append(standardButton);
		auto t2 = makeTlsButton(14, std::wstring(L"\u0054\u004C\u0053 \u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F 2"));
		Grid::SetColumn(t2, 1);
		Grid::SetRow(t2, 0);
		tlsGrid.Children().Append(t2);
		auto t3 = makeTlsButton(15, std::wstring(L"\u0054\u004C\u0053 \u0421\u0442\u0440\u0430\u0442\u0435\u0433\u0438\u044F 3"));
		Grid::SetColumn(t3, 2);
		Grid::SetRow(t3, 0);
		tlsGrid.Children().Append(t3);
	}

	container.Children().Append(grid);
	container.Children().Append(tlsGrid);

	return container;
}

winrt::Microsoft::UI::Xaml::UIElement CreateToggleBypassButton()
{
	Border btnBorder{};
	btnBorder.Width(320);
	btnBorder.Height(52);
	btnBorder.CornerRadius({ 6,6,6,6 });
	btnBorder.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	btnBorder.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom);
	btnBorder.Margin({ 0,0,0,12 });

	winrt::hstring startLabel = winrt::hstring(L"\u0417\u0430\u043F\u0443\u0441\u0442\u0438\u0442\u044C");
	winrt::hstring stopLabel = winrt::hstring(L"\u041E\u0441\u0442\u0430\u043D\u043E\u0432\u0438\u0442\u044C");

	TextBlock tb{};
	tb.Text(startLabel);
	tb.HorizontalAlignment(HorizontalAlignment::Center);
	tb.VerticalAlignment(VerticalAlignment::Center);
	tb.FontSize(16);
	tb.Opacity(1.0);
	winrt::Windows::UI::Text::FontWeight fw{};
	fw.Weight = 600;
	tb.FontWeight(fw);

	btnBorder.Child(tb);

	winrt::Windows::UI::Color greenColor{}; greenColor.A = 0x80; greenColor.R = 0x21; greenColor.G = 0x96; greenColor.B = 0x35;
	winrt::Windows::UI::Color redColor{}; redColor.A = 0x80; redColor.R = 0xE5; redColor.G = 0x39; redColor.B = 0x35;
	winrt::Microsoft::UI::Xaml::Media::SolidColorBrush greenBrush{ greenColor };
	winrt::Microsoft::UI::Xaml::Media::SolidColorBrush redBrush{ redColor };
	winrt::Windows::UI::Color greenOutlineColor{}; greenOutlineColor.A = 0xFF; greenOutlineColor.R = greenColor.R; greenOutlineColor.G = greenColor.G; greenOutlineColor.B = greenColor.B;
	winrt::Windows::UI::Color redOutlineColor{}; redOutlineColor.A = 0xFF; redOutlineColor.R = redColor.R; redOutlineColor.G = redColor.G; redOutlineColor.B = redColor.B;
	winrt::Microsoft::UI::Xaml::Media::SolidColorBrush greenOutlineBrush{ greenOutlineColor };
	winrt::Microsoft::UI::Xaml::Media::SolidColorBrush redOutlineBrush{ redOutlineColor };
	winrt::Windows::UI::Color whiteOpaque{}; whiteOpaque.A = 0xFF; whiteOpaque.R = 0xFF; whiteOpaque.G = 0xFF; whiteOpaque.B = 0xFF;
	winrt::Microsoft::UI::Xaml::Media::SolidColorBrush whiteTextBrush{ whiteOpaque };

	bool initialRunning = GetUIBypassRunningState() || IsBypassRunning();
	btnBorder.Tag(winrt::box_value(initialRunning));
	if (initialRunning) {
		tb.Text(stopLabel);
		btnBorder.Background(redBrush);
		btnBorder.BorderBrush(redOutlineBrush);
		tb.Foreground(whiteTextBrush);
	} else {
		tb.Text(startLabel);
		btnBorder.Background(greenBrush);
		btnBorder.BorderBrush(greenOutlineBrush);
		tb.Foreground(whiteTextBrush);
	}

	btnBorder.PointerPressed([btnBorder, tb, startLabel, stopLabel, greenBrush, redBrush, whiteTextBrush, greenOutlineBrush, redOutlineBrush](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		auto tag = btnBorder.Tag();
		bool running = tag ? winrt::unbox_value<bool>(tag) : false;
		if (!running) {
			bool ok = StartSelectedBypass();
			if (ok) {
				btnBorder.Tag(winrt::box_value(true));
				tb.Text(stopLabel);
				btnBorder.Background(redBrush);
				btnBorder.BorderBrush(redOutlineBrush);
				tb.Foreground(whiteTextBrush);
			} else {
				ReportBypassStatusIfNotRunning();
				btnBorder.Tag(winrt::box_value(false));
			}
		} else {
			StopBypass();
			btnBorder.Tag(winrt::box_value(false));
			tb.Text(startLabel);
			btnBorder.Background(greenBrush);
			btnBorder.BorderBrush(greenOutlineBrush);
			tb.Foreground(whiteTextBrush);
		}
	});

	btnBorder.PointerEntered([btnBorder, tb, greenBrush, redBrush, whiteTextBrush, greenOutlineBrush, redOutlineBrush](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		auto tag = btnBorder.Tag();
		bool running = tag ? winrt::unbox_value<bool>(tag) : false;
		if (running) {
			btnBorder.Background(redBrush);
			btnBorder.BorderBrush(redOutlineBrush);
		} else {
			btnBorder.Background(greenBrush);
			btnBorder.BorderBrush(greenOutlineBrush);
		}
		tb.Foreground(whiteTextBrush);
	});

	btnBorder.PointerExited([btnBorder, tb, greenBrush, redBrush, whiteTextBrush, greenOutlineBrush, redOutlineBrush](winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
		auto tag = btnBorder.Tag();
		bool running = tag ? winrt::unbox_value<bool>(tag) : false;
		if (running) {
			btnBorder.Background(redBrush);
			btnBorder.BorderBrush(redOutlineBrush);
		} else {
			btnBorder.Background(greenBrush);
			btnBorder.BorderBrush(greenOutlineBrush);
		}
		tb.Foreground(whiteTextBrush);
	});

	return btnBorder;
}

winrt::Microsoft::UI::Xaml::UIElement CreateBottomCornerButtonsPanel()
{
	Grid container{};

	StackPanel spLeft{};
	spLeft.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal);
	spLeft.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left);
	spLeft.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom);
	spLeft.Margin({12,0,0,12});

	Border listBorder{};
	listBorder.Width(48);
	listBorder.Height(48);
	listBorder.CornerRadius({8,8,8,8});
	TextBlock listTb{};
	{
		int theme = GetSavedTheme();
		winrt::Windows::UI::Color listBg{}; listBg.A = 0xFF;
		winrt::Windows::UI::Color listBorderColor{}; listBorderColor.A = 0xFF;
		winrt::Windows::UI::Color listFg{}; listFg.A = 0xFF;
		if (theme == 1) {
			listBg.R = 0x22; listBg.G = 0x22; listBg.B = 0x22;
			listBorderColor.R = 0x33; listBorderColor.G = 0x33; listBorderColor.B = 0x33;
			listFg.R = 0xFF; listFg.G = 0xFF; listFg.B = 0xFF;
		} else {
			listBg.R = 0xF5; listBg.G = 0xF5; listBg.B = 0xF5;
			listBorderColor.R = 0xDD; listBorderColor.G = 0xDD; listBorderColor.B = 0xDD;
			listFg.R = 0x9B; listFg.G = 0x85; listFg.B = 0xC8;
		}
		listBorder.Background(SolidColorBrush{ listBg });
		listBorder.BorderBrush(SolidColorBrush{ listBorderColor });
		listTb.Foreground(SolidColorBrush{ listFg });
	}
	listBorder.BorderThickness({1,1,1,1});
	listTb.Text(winrt::hstring(L"\U0001F4CB")); 
	listTb.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	listTb.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	listTb.FontSize(18);
	listBorder.Child(listTb);
	listBorder.PointerPressed([](auto&&, auto&&) {
		::OpenListFileFromLogic();
	});
	spLeft.Children().Append(listBorder);

	Border toolBorder{};
	toolBorder.Width(48);
	toolBorder.Height(48);
	toolBorder.CornerRadius({8,8,8,8});
	toolBorder.Margin({8,0,0,0});
	TextBlock toolTb{};
	{
		int theme = GetSavedTheme();
		winrt::Windows::UI::Color bg{}; bg.A = 0xFF; winrt::Windows::UI::Color borderCol{}; borderCol.A = 0xFF; winrt::Windows::UI::Color fg{}; fg.A = 0xFF;
		if (theme == 1) { bg.R = 0x22; bg.R = 0x22; bg.B = 0x22; borderCol.R = 0x33; borderCol.G = 0x33; borderCol.B = 0x33; fg.R = 0xFF; fg.G = 0xFF; fg.B = 0xFF; }
		else { bg.R = 0xF5; bg.G = 0xF5; bg.B = 0xF5; borderCol.R = 0xDD; borderCol.G = 0xDD; borderCol.B = 0xDD; fg.R = 0x9B; fg.G = 0x85; fg.B = 0xC8; }
		toolBorder.Background(SolidColorBrush{ bg });
		toolBorder.BorderBrush(SolidColorBrush{ borderCol });
		try { toolTb.Foreground(SolidColorBrush{ fg }); } catch(...) {}
	}
	toolBorder.BorderThickness({1,1,1,1});
	toolTb.Text(winrt::hstring(L"\u2139\uFE0F")); 
	toolTb.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	toolTb.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	toolTb.FontSize(18);
	toolBorder.Child(toolTb);
	toolBorder.PointerPressed([](auto&&, auto&&) {
		::OpenAboutFromLogic();
	});
	spLeft.Children().Append(toolBorder);

	container.Children().Append(spLeft);

	StackPanel spRight{};
	spRight.Orientation(winrt::Microsoft::UI::Xaml::Controls::Orientation::Horizontal);
	spRight.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Right);
	spRight.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Bottom);
	spRight.Margin({0,0,12,12});

	Border consoleBorder{};
	consoleBorder.Width(48);
	consoleBorder.Height(48);
	consoleBorder.CornerRadius({8,8,8,8});
	consoleBorder.Margin({0,0,8,0});
	TextBlock consoleLargeTb{};
	winrt::Windows::UI::Color consoleBg{}; consoleBg.A = 0xFF; consoleBg.R = 0xF5; consoleBg.G = 0xF5; consoleBg.B = 0xF5;
	{
		int theme = GetSavedTheme();
		winrt::Windows::UI::Color bg{}; bg.A = 0xFF; winrt::Windows::UI::Color borderCol{}; borderCol.A = 0xFF; winrt::Windows::UI::Color fg{}; fg.A = 0xFF;
		if (theme == 1) { bg.R = 0x22; bg.R = 0x22; bg.B = 0x22; borderCol.R = 0x33; borderCol.G = 0x33; borderCol.B = 0x33; fg.R = 0x8F; fg.G = 0x78; fg.B = 0xC4; }
		else { bg.R = 0xF5; bg.G = 0xF5; bg.B = 0xF5; borderCol.R = 0xDD; borderCol.G = 0xDD; borderCol.B = 0xDD; fg.R = 0x8F; fg.G = 0x78; fg.B = 0xC4; }
		consoleBorder.Background(SolidColorBrush{ bg });
		consoleBorder.BorderBrush(SolidColorBrush{ borderCol });
		try { consoleLargeTb.Foreground(SolidColorBrush{ fg }); } catch(...) {}
	}
	consoleBorder.BorderThickness({1,1,1,1});
	consoleLargeTb.Text(winrt::hstring(L">_"));
	consoleLargeTb.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
	consoleLargeTb.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
	consoleLargeTb.FontSize(32);
	winrt::Windows::UI::Text::FontWeight consoleFw{}; consoleFw.Weight = 800;
	consoleLargeTb.FontWeight(consoleFw);
	winrt::Windows::UI::Color consoleFg{}; consoleFg.A = 0xFF; consoleFg.R = 0x8F; consoleFg.G = 0x78; consoleFg.B = 0xC4;
	consoleLargeTb.Foreground(SolidColorBrush{ consoleFg });
	consoleLargeTb.Margin({ -2, -6, 0, 0 });
	consoleBorder.Child(consoleLargeTb);

	consoleBorder.PointerPressed([](auto&&, auto&&) {
		::OpenServiceBatchFromLogic();
	});

	Border gearBorder{};
	gearBorder.Width(48);
	gearBorder.Height(48);
	gearBorder.CornerRadius({8,8,8,8});
	winrt::Windows::UI::Color gearBg{}; gearBg.A = 0xFF;
	{
		int theme = GetSavedTheme();
		winrt::Windows::UI::Color bg{}; bg.A = 0xFF; winrt::Windows::UI::Color borderCol{}; borderCol.A = 0xFF; winrt::Windows::UI::Color fg{}; fg.A = 0xFF;
		if (theme == 1) { bg.R = 0x22; bg.G = 0x22; bg.B = 0x22; borderCol.R = 0x33; borderCol.G = 0x33; borderCol.B = 0x33; fg.R = 0x9B; fg.G = 0x85; fg.B = 0xC8; }
		else { bg.R = 0xF5; bg.G = 0xF5; bg.B = 0xF5; borderCol.R = 0xDD; borderCol.G = 0xDD; borderCol.B = 0xDD; fg.R = 0x9B; fg.G = 0x85; fg.B = 0xC8; }
		gearBorder.Background(SolidColorBrush{ bg });
		gearBorder.BorderBrush(SolidColorBrush{ borderCol });
		gearBorder.BorderThickness({2,2,2,2});
		TextBlock gearTb{};
		gearTb.Text(winrt::hstring(L"\u2699"));
		gearTb.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Center);
		gearTb.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Center);
		gearTb.FontSize(18);
		gearTb.Foreground(SolidColorBrush{ fg });
		gearBorder.Child(gearTb);
	}

	gearBorder.PointerPressed([](auto&&, auto&&) {
		OpenSettingsFromLogic();
	});

	spRight.Children().Append(consoleBorder);
	spRight.Children().Append(gearBorder);
	container.Children().Append(spRight);

	return container;
}