#include <d3d12.h>
#include <wrl.h>
#include "WinApp.h"
#include "kMath.h"
#include "Transform.h"
#include <string>

#pragma once

class Sprite {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string textureFilePath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// テクスチャ変更
	/// </summary>
	void ChangeTexture(std::string textureFilePath);
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float pad[3];
		Matrix4x4 uvTransform;
		float shininess;
		Vector3 specularColor;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	// VertexResourceを作成する
	void CreateVertexResource();
	// IndexResourceを作成する
	void CreateIndexResource();
	// MaterialResouceを作成する
	void CreateMaterialResource();
	// TransformationMatrixを作成する
	void CreateTransformationMatrixResource();

	// VertexBufferViewを作成する (値を設定するだけ)
	void CreateVertexBufferView();
	// IndexBufferViewを作成する (値を設定するだけ)
	void CreateIndexBufferView();
	// Materialの値を設定
	void SetMaterial();
	// TransformationMatrixの値を設定
	void SetTransformatinMatrix();

	// テクスチャサイズを死めーじに合わせる
	void AdjustTextureSize();

private:

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	// バッファリソース内のデータを指すポインタ
	uint32_t* indexData = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	// バッファリソース内のデータを指すポインタ
	Material* materialData = nullptr;

	// バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	// バッファリソース内のデータを指すポインタ
	TransformationMatrix* transformationMatrixData = nullptr;

	
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexbufferView;

private:

	// 位置
	Vector2 position = {0.0f, 0.0f};
	// 回転
	float rotation = 0.0f;
	// 拡縮
	Vector2 scale = {0.0f, 0.0f};
	// 色(RGBA)
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	// 回転の原点
	Vector2 anchorPoint = {0.0f, 0.0f};
	// 左右フリップ
	bool isFlipX = false;
	// 上下フリップ
	bool isFlipY = false;
	// テクスチャ左上座標
	Vector2 textureLeftTop = {0.0f, 0.0f};
	// テクスチャ切り出しサイズ
	Vector2 textureSize = {100.0f, 100.0f};

	// 移動量
	Transform velocity = { 0.0f, 0.0f };
	// 減衰量
	Transform acceleration = { 0.0f, 0.0f };

	// テクスチャ番号
	uint32_t textureIndex = 0;

public:
	/// <summary>
	/// Getter(Position)
	/// </summary>
	/// <param name="Position">位置</param>
	const Vector2& GetPosition() const { return position; }
	/// <summary>
	/// Getter(Rotation)
	/// </summary>
	/// <param name="Rotation">回転</param>
	const float& GetRotation() const { return rotation; }
	/// <summary>
	/// Getter(Scale)
	/// </summary>
	/// <param name="Scale">大きさ</param>
	const Vector2& GetScale() const { return scale; }
	/// <summary>
	/// Getter(Color)
	/// </summary>
	/// <param name="Color">色</param>
	const Vector4& GetColor() const { return materialData->color; }
	/// <summary>
	/// Getter(AnchorPoint)
	/// </summary>
	/// <param name="AnchorPoint">回転の原点</param>
	const Vector2& GetAnchorPoint() const { return anchorPoint; }
	/// <summary>
	/// Getter(FlipX)
	/// </summary>
	/// <param name="FlipX">左右フリップX</param>
	const bool& GetIsFlipX() const { return isFlipX; }
	/// <summary>
	/// Getter(FlipY)
	/// </summary>
	/// <param name="FlipY">左右フリップY</param>
	const bool& GetIsFlipY() const { return isFlipY; }
	/// <summary>
	/// Getter(TextureLeftTop)
	/// </summary>
	/// <param name="TextureLeftTop">テクスチャ左上サイズ</param>
	const Vector2& GetTextureLeftTop() const { return textureLeftTop; }
	/// <summary>
	/// Getter(TextureSize)
	/// </summary>
	/// <param name="TextureSize">テクスチャ切り出しサイズX</param>
	const Vector2& GetTextureSize() const { return textureSize; }
	/// <summary>
	/// Getter(Velocity)
	/// </summary>
	/// <param name="Velocity">値の移動量</param>
	const Transform& GetVelocity() const { return velocity; }
	/// <summary>
	/// Getter(Acceleration)
	/// </summary>
	/// <param name="Acceleration">Velocityの減衰量</param>
	const Transform& GetAcceleration() const { return acceleration; }

	/// <summary>
	/// Setter(Position)
	/// </summary>
	/// <param name="Position">位置</param>
	void SetPosition(const Vector2& pos) { position = pos; }
	/// <summary>
	/// Setter(Rotation)
	/// </summary>
	/// <param name="Rotation">回転</param>
	void SetRotatioin(const float& rotate) { rotation = rotate; }
	/// <summary>
	/// Setter(Scale)
	/// </summary>
	/// <param name="Scale">大きさ</param>
	void SetScale(const Vector2& size) { scale = size; }
	/// <summary>
	/// Setter(Color)
	/// </summary>
	/// <param name="Color">色</param>
	void SetColor(const Vector4& color) { materialData->color = color; }
	/// <summary>
	/// Setter(AnchorPoint)
	/// </summary>
	/// <param name="AnchorPoint">回転の原点</param>
	void SetAnchorPoint(const Vector2& anchPoint) { anchorPoint = anchPoint; }
	/// <summary>
	/// Setter(FlipX)
	/// </summary>
	/// <param name="FlipX">左右フリップX</param>
	void SetIsFlipX(const bool& FlipX) { isFlipX = FlipX; }
	/// <summary>
	/// Setter(FlipY)
	/// </summary>
	/// <param name="FlipY">左右フリップY</param>
	void SetIsFlipY(const bool& FlipY) { isFlipY = FlipY; }
	/// <summary>
	/// Setter(FlipXY)
	/// </summary>
	/// <param name="FlipX : ">左右フリップX</param>
	/// <param name="FlipY: ">左右フリップY</param>
	void SetIsFlip(const bool& FlipX, const bool& FlipY);
	/// <summary>
	/// Setter(TextureLeftTop)
	/// </summary>
	/// <param name="TextureLeftTop: ">テクスチャ左上サイズ</param>
	void SetTextureLeftTop(const Vector2& LeftTop) { textureLeftTop = LeftTop; }
	/// <summary>
	/// Setter(TextureSize)
	/// </summary>
	/// <param name="TextureSize: ">テクスチャ切り出しサイズ</param>
	void SetTextureSize(const Vector2& size) { textureSize = size; }
	/// <summary>
	/// Setter(Transform)
	/// </summary>
	/// <param name="Transform: ">大きさ、回転、位置の構造体</param>
	void SetTransform(Transform& transform);
	/// <summary>
	/// Setter(Transform)
	/// </summary>
	/// <param name="Position: ">位置</param>
	/// <param name="Rotation: ">回転</param>
	/// <param name="Scale: ">大きさ</param>
	void SetTransform(const Vector2& position, const float& rotation, const Vector2& scale);
	/// <summary>
	/// Setter(Velocity)
	/// </summary>
	/// <param name="Velocity: ">値の移動量</param>
	/// /// <param name="Transform: ">大きさ、回転、位置の構造体</param>
	void SetVelocityTransform(const Transform& transform) { velocity = transform; }
	/// <summary>
	/// Setter(Acceleration)
	/// </summary>
	/// <param name="Acceleration: ">Velocityの減衰量</param>
	/// /// <param name="Transform: ">大きさ、回転、位置の構造体</param>
	void SetAccelerationTransform(const Transform& transform) { velocity = transform; }
	//void SetMaterial(Material* material);
	// 
	// 初期化時などの一度に変更したい場合に
	void SetStatus(const Vector2& position, const float& rotation, const Vector2& scale, const Vector4& color);

};
