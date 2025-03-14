#pragma once
#include "Input.h"
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Transform.h"
#include "Camera.h"
#include "WinApp.h"
#include "kMath.h"

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

class PlayerS {
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	PlayerS();

	/// <summary>
	/// 初期化
	/// </summary>
	~PlayerS();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Object3d* object3d, Camera* camera, Input* input);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightData, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightData, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightData);

	Camera* GetCamera();

private: // メンバ変数

	Transform modelTransform_;

	Object3d* object3d_ = nullptr;

	Camera* camera_ = nullptr;

	Vector4 modelColor_;

	Input* input_ = nullptr;

	bool modelEnableLighting_;

	float shininess_;


};

