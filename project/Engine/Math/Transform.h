#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <wrl.h>
#include <d3d12.h>

#pragma once

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

class WorldTransform {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

	const Microsoft::WRL::ComPtr<ID3D12Resource> GetTransformationMatrixResource() const { return transformationMatrixResource; }

	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };
	Vector3 translation = { 0.0f, 0.0f, 0.0f };

	Matrix4x4 worldMatrix;

	const WorldTransform* parent = nullptr;

private:

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	Transform transform;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	// 座標変換リソースのバッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// 座標変換行列リソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrix = nullptr;
};
