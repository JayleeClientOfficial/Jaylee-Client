#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>

winrt::Microsoft::UI::Xaml::UIElement CreateTelegramMenu();

DWORD LaunchTgWsProxy();

void EmbedTgWsProxyWindow();
