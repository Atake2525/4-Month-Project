#include "CollisionModel.h"
#include "kMath.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void CollisionModel::Initialize(std::string directoryPath, std::string filename) {
	// ���f���ǂݍ���
	modelData = LoadModelFile(directoryPath, filename);

}

// �}���`�X���b�h���\��
ModelData CollisionModel::LoadModelFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;            // �\�z����ModelData
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // ���b�V���������̂͑Ή����Ȃ�

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // �@��������Mesh�͍���͔�Ή�
		// ��������Mesh�̒��g(Face)�̉�͂��s���Ă���
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			aiFace& face = mesh->mFaces[faceIndex];

			assert(face.mNumIndices == 4); // �l�p�`�̂݃T�|�[�g
			// ��������Face�̒��g(Vertex)�̉�͂��s���Ă���
			for (uint32_t element = 0; element < face.mNumIndices; ++element)
			{
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHanded��z*=-1�ŁA�E��->����ɕϊ�����̂Ŏ蓮�őΏ�
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}

		}
	}
	return modelData;
	
}