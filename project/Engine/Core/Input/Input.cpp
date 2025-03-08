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
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY);
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
	result = mouse->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	HRESULT result;

	// 前回のキー入力を保存
	memcpy(keyPres, keys, sizeof(keys));
	// キーボード情報の取得開始
	result = keyboard->Acquire();
	// 全キーの入力情報を取得する
	result = keyboard->GetDeviceState(sizeof(keys), keys);

	mouseStatePre = mouseState;
	// マウスの状態の取得
	result = mouse->Acquire();
	// ポーリング開始
	result = mouse->Poll();
	// 全ボタンの入力情報を取得する
	result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

}

bool Input::PushKey(BYTE keyNumber) {
	if (keys[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	if (keys[keyNumber] && !keyPres[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::ReturnKey(BYTE keyNumber) {
	if (!keys[keyNumber] && keyPres[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::PressMouse(int mouseNumber) {
	if (mouseState.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

bool Input::TriggerMouse(int mouseNumber) {
	if (mouseState.rgbButtons[mouseNumber] && !mouseStatePre.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

bool Input::ReturnMouse(int mouseNumber) {
	if (!mouseState.rgbButtons[mouseNumber] && mouseStatePre.rgbButtons[mouseNumber] && (0x80))
	{
		return true;
	}
	return false;
}

Vector2& Input::GetMousePos2() {
	Vector2 result = { mouseState.lX, mouseState.lY };
	return result;
}

Vector3& Input::GetMousePos3() {
	Vector3 result = { mouseState.lX, mouseState.lY, mouseState.lZ };
	return result;
}