#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

class WinApp {
public:
	~WinApp();
	static const int kClientWidth = 1280;
	static const int kClientHeight = 720;
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Log(const std::string& message);
	std::wstring ConvertString(const std::string& str);
	std::string ConvertString(const std::wstring& str);
	void CreateGameWindow(const wchar_t* title, int32_t kClientWidth, int32_t kClientHeight);
	bool MessageRoop();
	HWND GetHwnd() { return hwnd_; };
private:
	WNDCLASS wc_{};
	RECT wrc_{};
	HWND hwnd_{};
	MSG msg_{};
};