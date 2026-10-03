#pragma once

#include <winrt/Microsoft.UI.Xaml.h>

winrt::Microsoft::UI::Xaml::Window CreateJayleeWindow(winrt::hstring const& title = L"Jaylee Client");
void RegisterMainWindow(winrt::Microsoft::UI::Xaml::Window const& window, winrt::hstring const& title);
void ActivateOwnedWindow(winrt::Microsoft::UI::Xaml::Window const& window, wchar_t const* title);
