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

class Camera;
class DirectXBase;

class WorldTransform {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXBase* directxBase);

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateMatrix(Camera* camera = nullptr);

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

	const Microsoft::WRL::ComPtr<ID3D12Resource> GetTransformationMatrixResource() const { return transformationMatrixResource; }

	// Getter(Transform) worldMatrix変換前なので計算にはあまり使わないほうが良い
	const Transform GetTransform() const { return transform; }
	// Getter(Scale)
	const Vector3 GetScale() const { return transform.scale; }
	// Getter(Rotate)
	const Vector3 GetRotate() const { return transform.rotate; }
	// Getter(Translate)
	const Vector3 GetTranslate() const { return transform.translate; }
	// Getter(translation) worldMatrixを参照しているので計算に使える
	const Vector3 GetWorldTranslation() const;
	// Getter(worldMatrix)
	const Matrix4x4 GetWorldMatrix() const { return worldMatrix; }

	// Setter(Transform)
	void SetTransform(const Transform& transform) { this->transform = transform; }
	// Setter(scale)
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	// Setter(rotate)
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	// Setter(translate)
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }

	const WorldTransform* parent = nullptr;

private:

	DirectXBase* dxBase;

	void CreateTransformationMatrixResrouce();

public:

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	Transform transform;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	// 座標変換リソースのバッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// 座標変換行列リソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrix = nullptr;
};
