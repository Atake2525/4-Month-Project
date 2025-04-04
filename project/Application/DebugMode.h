#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

// DirectXBase
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


#include <dxgidebug.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include "Vector4.h"
#include "Matrix4x4.h"
#include <cassert>
#include "kMath.h"

// クラス化した部分
#include "Input.h"
#include "WinApp.h"
#include "Logger.h"
#include "D3DResourceLeakChecker.h"
#include "DirectXBase.h"
#include "SpriteBase.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Transform.h"
#include "Camera.h"
#include "Audio.h"
#include "FrameWork.h"
#include "WireFrameObjectBase.h"
#include "Player.h"
#include"LightBlock.h"
#include"switchLight.h"

#include "algorithm"
#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"
#include "externels/DirectXTex/DirectXTex.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#pragma once
class DebugMode : public FrameWork {
public:
	// 初期化
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// ループ終了
	bool RoopOut() override { return Finished; }

	// ↑までシーンの作成に必須

private:
	D3DResourceLeakChecker d3dResourceLeakChecker;

	// メンバ変数宣言

#pragma region 基盤システム

	WinApp* winApp = nullptr;

	DirectXBase* directxBase = nullptr;

	Camera* camera = nullptr;

#pragma endregion 基盤システム

	Sprite* sprite = nullptr;

	Object3d* object3d = nullptr;

	// グリッド
	Object3d* grid = nullptr;

	Input* input = nullptr;

	// ライトデータの宣言

	DirectionalLight* directionalLightData = nullptr;


	PointLight* pointLightData = nullptr;

	SpotLight* spotLightData = nullptr;

	// Transform変数を作る
	Transform transform{
		{1.0f, 1.0f,   1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f,   0.0f}
	};

	Transform cameraTransform{
		{1.0f,  1.0f, 1.0f  },
		{0.36f, 0.0f, 0.0f  },
		{0.0f,  6.0f, -19.0f}
	};

	// カメラ
	CameraForGPU* cameraData = nullptr;

	// サウンド
	SoundData soundData1;
	SoundData soundData2;

	// ライトリソース宣言
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource;

	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource;


	// ImGuiで使用する変数の宣言
	Vector2 position;
	float rotation;
	Vector2 scale;
	Vector4 color;
	Vector2 anchorPoint;
	bool isFlipX;
	bool isFlipY;
	Vector2 textureLeftTop;
	Vector2 textureSize;

	Transform modelTransform;
	Vector4 modelColor;
	Vector3 specularColor;
	float shininess;
	bool modelEnableLighting;

	float farClip;
	float fov;

	bool Finished = false;

	bool showCursor = false;

	Vector2 mousePos2;
	Vector3 mousePos3;

	Vector3 axis = { 0.0f, 1.0f, 0.0f };

	float angle = 0.0f;

	bool isCollision = false;

	Player* player = nullptr;

	Object3d* playerObj = nullptr;

	/*=====消えるブロック=====*/
	LightBlock* lightBlock = nullptr;
	switchLight* lightSwitch = nullptr;
	Transform switchTransform;
};
