#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include <string>
#include <sstream>
#include <fstream>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#pragma once

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct MaterialVertexData {
	std::vector<VertexData> vertices;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float pad[3];
	Matrix4x4 uvTransform;
	float shininess;
	Vector3 specularColor;
};

struct MaterialData {
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

struct ModelData {
	std::vector<VertexData> vertices; 
	std::vector<MaterialVertexData> matVertexData;
	std::vector<MaterialData> material;
};

class Model {
public:

	// 初期化
	void Initialize(std::string directoryPath, std::string filename, bool enableLighting);
	
	// 更新
	void Draw();

	void SetIA();

	// Getter(Color)
	const Vector4& GetColor() const { return materialData->color; }
	// Getter(EnableLighting)
	const bool GetEnableLighting() const { return materialData->enableLighting; }
	// Getter(SpecularColor)
	//const Vector3& GetSpecularColor() const { return materialData->specularColor; }
	// Getter(Shininess)
	const float& GetShininess() const { return materialData->shininess; }
	// Getter(ModelData)
	const ModelData& GetModelData() const { return modelData;}
	// Getter(ModelData vertices)
	const std::vector<VertexData>& GetVertices() const { return modelData.vertices; }

	// Setter(Color)
	void SetColor(const Vector4& color) { 
		materialData->color = color; 
	}
	// Setter(EnableLighting)
	void SetEnableLighting(const bool& enableLighting) {
		materialData->enableLighting = enableLighting; 
	}
	// Setter(SpecularColor)
	//void SetSpecularColor(const Vector3& specularColor) { materialData->specularColor = specularColor; }
	// Setter(Shininess)
	void SetShininess(const float& shininess) { materialData->shininess = shininess; }

	void DebugMode(bool debugMode) { useWireFrameTexture = debugMode; }

private:

	// 頂点データのバッファリソース
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexResource;
	// 頂点データのバッファリソース内のデータを指すポインタ
	std::vector<VertexData*> vertexData;
	// バッファリソースの使い道を指定するバッファビュー
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferView;

	// Objファイルのデータ
	ModelData modelData;

	// マテリアルのバッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	// マテリアルバッファリソース内のデータを指すポインタ
	Material* materialData = nullptr;

private:
	// .mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);
	// .objファイルの読み取り
	static ModelData LoadModelFile(const std::string& directoryPath, const std::string& fileName);

	// VertexResourceを作成する
	void CreateVertexResource();
	// MaterialResourceを作成する
	void CreateMaterialResouce();

	// VertexBufferViewを作成する(値を設定するだけ)
	void CreateVertexBufferView();

	bool useWireFrameTexture;
	uint32_t whiteTextureIndex;
public:

	Model() = default;
};
