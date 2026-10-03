#pragma once
#include <string>
#include <functional>

void OpenSettingsFromLogic();
void OpenListFileFromLogic();
void OpenDomainListFromLogic(std::wstring const& fileName);
void OpenServiceBatchFromLogic();

void OpenAboutFromLogic();

void SelectStrategy(int altIndex);
int GetSelectedStrategy();
void LoadSettingsFromConfig();
bool IsAutorunEnabled();
bool ToggleAutorun();

bool IsTrayRunning();
bool ToggleTray();

bool IsAeroEnabled();
bool ToggleAero();

bool IsStrategyAutostartEnabled();
bool ToggleStrategyAutostart();

bool StartSelectedBypass();
void StopBypass();

bool GetUIBypassRunningState();

bool IsBypassRunning();

bool CheckBypassStatus(std::wstring &outMessage);

void ReportBypassStatusIfNotRunning();

bool CheckBypassStatusEnhanced(std::wstring& outMessage, DWORD* pidOut = nullptr);

void ApplyTheme(int theme);
void RegisterAppWindow(winrt::Microsoft::UI::Xaml::Window const& w);
void UnregisterAppWindow(winrt::Microsoft::UI::Xaml::Window const& w);
void RegisterThemeListener(winrt::Microsoft::UI::Xaml::Window const& w, std::function<void(int)> listener);
