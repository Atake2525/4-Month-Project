#define DIRECTIONPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <wrl.h>
#include <Windows.h>
#include "Vector2.h"
#include "Vector3.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#pragma once

class WinApp;

class Input {
public:
	// namespcae省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize(WinApp* winApp);
	void Update();

public:
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号 例(DIK_0)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号 例(DIK_0)</param>
	/// <returns>押したかどうか</returns>
	bool TriggerKey(BYTE keyNumber);


	/// <summary>
	/// キーのリターンをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号 例(DIK_0)</param>
	/// <returns>離されたか</returns>
	bool ReturnKey(BYTE keyNumber);

	/// <summary>
	/// マウスの押下をチェック
	/// </summary>
	/// <param name="mouseNumer">0 = 左ボタン</param>
	/// <param name="mouseNumer">1 = 右ボタン</param>
	/// <param name="mouseNumer">2 = マウスホイール押し込み</param>
	bool PressMouse(int mouseNumer);

	/// <summary>
	/// マウスのトリガーをチェック
	/// </summary>
	/// <param name="mouseNumer">0 = 左ボタン</param>
	/// <param name="mouseNumer">1 = 右ボタン</param>
	/// <param name="mouseNumer">2 = マウスホイール押し込み</param>
	bool TriggerMouse(int mouseNumber);

	/// <summary>
	/// マウスのリターンをチェック
	/// </summary>
	/// <param name="mouseNumer">0 = 左ボタン</param>
	/// <param name="mouseNumer">1 = 右ボタン</param>
	/// <param name="mouseNumer">2 = マウスホイール押し込み</param>
	bool ReturnMouse(int mouseNumber);

	// マウスの移動量を取得(Vector2)
	Vector2& GetMousePos2();

	// マウスの移動量を取得(Vector3)
	Vector3& GetMousePos3();

	/// <summary>
	/// マウスカーソルの表示変更
	/// </summary>
	/// <param name="">True  = 表示</param>
	/// <param name="">False = 非表示</param>
	void ShowMouseCursor(bool flag);

private:
	// キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard;

	WinApp* winApp_ = nullptr;

	// 全キーの状態
	BYTE keys[256] = {};
	// 前回の全キーの状態
	BYTE keyPres[256] = {};

	// マウスデバイス
	ComPtr<IDirectInputDevice8> mouse;

	// 全マウスの状態
	DIMOUSESTATE mouseState;
	// 前回の全マウスの状態
	DIMOUSESTATE mouseStatePre;
	// マウスカーソル表示
	bool showCursor = false;
};
