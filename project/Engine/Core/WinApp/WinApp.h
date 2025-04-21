#include <Windows.h>
#include <cstdint>
#include "AABB.h"

#pragma comment(lib, "winmm.lib")
#pragma once

enum class WindowMode {
	Window,
	FullScreen,
};

class WinApp {
private:
	static WinApp* instance;

	WinApp() = default;
	~WinApp() = default;

	WinApp(WinApp&) = delete;
	WinApp& operator=(WinApp&) = delete;

public:
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:

	static WinApp* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Update();

	// 終了
	void Finalize();

	// メッセージの処理
	bool ProcessMessage();

	const AABB& GetWindowAABB() const;

	// クライアント領域サイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	WindowMode windowMode = WindowMode::Window;

	// getter
	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return wc.hInstance; }

private:
	// ウィンドウハンドル
	HWND hwnd = nullptr;

	// ウィンドウクラスの設定
	WNDCLASS wc{};
};
