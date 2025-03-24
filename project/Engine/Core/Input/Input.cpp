#include "Input.h"
#include <cassert>
#include "WinApp.h"

void Input::Initialize(WinApp* winApp) {
	winApp_ = winApp;
	HRESULT result;
	// DirectInputの初期化
	// DirectInputのインスタンス生成 キーボード
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// 排他制御レベルセット
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// DorectxInputのインスタンス生成 マウス
	ComPtr<IDirectInput8> directInputMouse = nullptr;
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInputMouse, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInputMouse->CreateDevice(GUID_SysMouse, &mouse, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	// 排他制御レベルセット
	result = mouse->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));

	// DorectxInputのインスタンス生成 コントローラー(ゲームパッド)
	ComPtr<IDirectInput8> directInputGamePad = nullptr;
	result = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInputGamePad, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInputGamePad->CreateDevice(GUID_Joystick, &joystick, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = joystick->SetDataFormat(&c_dfDIJoystick);
	assert(SUCCEEDED(result));

	// 軸モード設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS; // 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	// 軸モードを変更
	result = joystick->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	result = joystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	
	result = joystick->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 排他制御レベルセット
	result = mouse->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
}

void Input::ShowMouseCursor(bool flag) {
	if (flag)
	{
		mouse->Unacquire();
	}
	showCursor = flag;
}

void Input::Update() {
	HRESULT result;

	// 前回のキー入力を保存
	memcpy(keyPres, keys, sizeof(keys));
	// キーボード情報の取得開始
	result = keyboard->Acquire();
	// 全キーの入力情報を取得する
	result = keyboard->GetDeviceState(sizeof(keys), keys);

	if (!showCursor)
	{
		mouseStatePre = mouseState;
		// マウスの状態の取得
		result = mouse->Acquire();
		// ポーリング開始
		result = mouse->Poll();
		// 全ボタンの入力情報を取得する
		result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	}

	// コントローラーの状態取得開始
	result = joystick->Acquire();
	// ポーリング開始
	result = joystick->Poll();
	// 入力情報を取得
	result = joystick->GetDeviceState(sizeof(DIJOYSTATE), &joystickState);


}

const bool& Input::PushKey(BYTE keyNumber) const {
	if (keys[keyNumber]) {
		return true;
	}
	return false;
}

const bool& Input::TriggerKey(BYTE keyNumber) const {
	if (keys[keyNumber] && !keyPres[keyNumber]) {
		return true;
	}
	return false;
}

const bool& Input::ReturnKey(BYTE keyNumber) const {
	if (!keys[keyNumber] && keyPres[keyNumber]) {
		return true;
	}
	return false;
}

const bool& Input::PressMouse(int mouseNumber) const {
	if (mouseState.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

const bool& Input::TriggerMouse(int mouseNumber) const {
	if (mouseState.rgbButtons[mouseNumber] && !mouseStatePre.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

const bool& Input::ReturnMouse(int mouseNumber) const {
	if (!mouseState.rgbButtons[mouseNumber] && mouseStatePre.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

Vector2& Input::GetMousePos2() {
	Vector2 result = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY) };
	return result;
}

Vector3& Input::GetMousePos3() {
	Vector3 result = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY), static_cast<float>(mouseState.lZ) };
	return result;
}

Vector2& Input::GetJoyStickPos2() {
	Vector2 result = { static_cast<float>(joystickState.lX), static_cast<float>(joystickState.lY) };
	return result;
}

Vector3& Input::GetJoyStickPos3() {
	Vector3 result = { static_cast<float>(joystickState.lX), static_cast<float>(joystickState.lY), static_cast<float>(joystickState.lZ) };
	return result;
}

const bool& Input::TriggerButton(BYTE ButtonNumber) const {
	if (dijoA)
	{

	}
}