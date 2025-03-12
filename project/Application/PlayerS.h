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
	void Initialize(Object3d* object3d, Camera* camera, WinApp* winApp);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightData, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightData, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightData);

private: // メンバ変数

	Transform modelTransform_;

	Object3d* object3d_ = nullptr;

	Camera* camera_ = nullptr;

	Vector4 modelColor_;

	Input* input_ = nullptr;

	bool modelEnableLighting_;

	float shininess_;
};

