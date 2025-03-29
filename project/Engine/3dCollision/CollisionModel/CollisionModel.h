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

	// ������
	void Initialize(std::string directoryPath, std::string filename);

	// Getter(ModelData)
	const ModelData& GetModelData() const { return modelData; }

private:

	// ���_�f�[�^�̃o�b�t�@���\�[�X���̃f�[�^���w���|�C���^
	VertexData* vertexData = nullptr;

	// Obj�t�@�C���̃f�[�^
	ModelData modelData;

private:
	// Model�t�@�C���̓ǂݎ��
	static ModelData LoadModelFile(const std::string& directoryPath, const std::string& fileName);
};
