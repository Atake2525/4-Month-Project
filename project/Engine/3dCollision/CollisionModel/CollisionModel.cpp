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
	//// 1. ���ŕK�v�ƂȂ�ϐ��̐錾
	//std::vector<Vector4> positions; // �ʒu
	//std::vector<Vector3> normals;   // �@��
	//std::vector<Vector2> texcoords; // �e�N�X�`�����W
	//std::string line;               // �t�@�C������ǂ�1�s���i�[�������

	//// 2. �t�@�C�����J��
	//std::ifstream file(directoryPath + "/" + filename); // �t�@�C�����J��
	//assert(file.is_open());                             // �Ƃ肠�����J���Ȃ�������~�߂�
	//// 3. ���ۂɃt�@�C����ǂ݁AModelData���\�z���Ă���
	//while (std::getline(file, line)) {
	//	std::string identifier;
	//	std::istringstream s(line);
	//	s >> identifier; // �擪�̎��ʎq��ǂ�

	//	// identifier�ɉ���������
	//	if (identifier == "v") {
	//		Vector4 position;
	//		s >> position.x >> position.y >> position.z;
	//		position.w = 1.0f;
	//		positions.push_back(position);
	//	} else if (identifier == "vt") {
	//		Vector2 texcoord;
	//		s >> texcoord.x >> texcoord.y;
	//		texcoords.push_back(texcoord);
	//	} else if (identifier == "vn") {
	//		Vector3 normal;
	//		s >> normal.x >> normal.y >> normal.z;
	//		normals.push_back(normal);
	//	} else if (identifier == "f") {
	//		VertexData triangle[3];

	//		// �ʂ͎O�p�`����B���̑��͖��Ή�
	//		for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
	//			std::string vertexDefinition;
	//			s >> vertexDefinition;
	//			// ���_�̗v�f�ւ�Index�́u�ʒu/UV/�@���v�Ŋi�[����Ă���̂ŁA��������Index���擾����
	//			std::istringstream v(vertexDefinition);
	//			uint32_t elementIndices[3];
	//			for (int32_t element = 0; element < 3; ++element) {
	//				std::string index;
	//				std::getline(v, index, '/'); // /��؂�ŃC���f�b�N�X��ǂ�ł���
	//				elementIndices[element] = std::stoi(index);
	//			}
	//			// �v�f�ւ�Index����A���ۂ̗v�f��l���擾���āA���_���\�z����
	//			Vector4 position = positions[elementIndices[0] - 1];
	//			Vector2 texcoord = texcoords[elementIndices[1] - 1];
	//			Vector3 normal = normals[elementIndices[2] - 1];
	//			// VertexData vertex = { position, texcoord, normal };
	//			// modelData.vertices.push_back(vertex);
	//			position.x *= -1.0f;
	//			//position.y *= -1.0f;
	//			normal.x *= -1.0f;
	//			texcoord.y = 1.0f - texcoord.y;

	//			triangle[faceVertex] = {position, texcoord, normal};
	//		}
	//		// ���_���t���œo�^���邱�ƂŁA���菇���t�ɂ���
	//		modelData.vertices.push_back(triangle[2]);
	//		modelData.vertices.push_back(triangle[1]);
	//		modelData.vertices.push_back(triangle[0]);
	//	} else if (identifier == "mtllib") {
	//		// materialTemplateLibrary�t�@�C���̖��O���擾����
	//		std::string materialFilename;
	//		s >> materialFilename;
	//		// ��{�I��obj�t�@�C���Ɠ���K�w��mtl�͑��݂�����̂ŁA�f�B���N�g�����ƃt�@�C������n��
	//		modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
	//	}
	//}
	//// 4. ModelData��Ԃ�
	//return modelData;
}