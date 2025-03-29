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

struct ModelData {
	std::vector<VertexData> vertices;
};

class CollisionModel {
public:

	// 初期化
	void Initialize(std::string directoryPath, std::string filename);

	// Getter(ModelData)
	const ModelData& GetModelData() const { return modelData; }

private:

	// 頂点データのバッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	// Objファイルのデータ
	ModelData modelData;

private:
	// Modelファイルの読み取り
	static ModelData LoadModelFile(const std::string& directoryPath, const std::string& fileName);
};
