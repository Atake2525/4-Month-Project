#include "Model.h"
#include "ModelBase.h"
#include "DirectXBase.h"
#include "kMath.h"
#include "TextureManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Initialize(std::string directoryPath, std::string filename, bool enableLighting) {
	// モデル読み込み
	modelData = LoadModelFile(directoryPath, filename);

	vertexResource.resize(modelData.matVertexData.size());
	// Resourceの作成
	CreateVertexResource();
	CreateMaterialResouce();

	vertexBufferView.resize(modelData.matVertexData.size());
	// BufferResourceの作成
	CreateVertexBufferView();

	vertexData.resize(modelData.matVertexData.size());
	// VertexResourceにデータを書き込むためのアドレスを取得してvertexDataに割り当てる
	for (uint32_t i = 0; i < modelData.material.size(); i++)
	{
		vertexResource.at(i)->Map(0, nullptr, reinterpret_cast<void**>(&vertexData[i]));
		std::memcpy(vertexData[i], modelData.matVertexData.at(i).vertices.data(), sizeof(VertexData) * modelData.matVertexData.at(i).vertices.size()); // 頂点データをリソースにコピー
	}
	//  書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));


	// データを書き込む
	// 今回は赤を書き込んでみる
	materialData->color = {1.0f, 1.0f, 1.0f, 1.0f};

	materialData->uvTransform = MakeIdentity4x4();

	materialData->enableLighting = enableLighting;
	materialData->shininess = 70.0f;
	materialData->specularColor = {1.0f, 1.0f, 1.0f};

}

void Model::Draw() {


	// wvp用のCBufferの場所を設定
	ModelBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	for (uint32_t i = 0; i < modelData.material.size(); i++)
	{
		// checkerBoadのmodelが描画されない
		// 頂点情報が正しく読めていない可能性あり
		// 
		// ModelTerrain
		ModelBase::GetInstance()->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView.at(i)); // VBVを設定

		ModelBase::GetInstance()->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.at(i).textureIndex));

		ModelBase::GetInstance()->GetDxBase()->GetCommandList()->DrawInstanced(UINT(modelData.matVertexData.at(i).vertices.size()), 1, 0, 0);
	}
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	// 1, 中で必要となる変数の宣言
	MaterialData materialData; // 構築するMaterialData
	std::string line;          // ファイルから読んだ１行を格納するもの
	// 2, ファイルを開く
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open());                             // とりあえず開けなかったら止める
	// 3, 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
		else
		{
			// map_Kdが存在しなかったらwhite1x1をテクスチャとして使用する
			materialData.textureFilePath = "Resources/Debug/white1x1.png";
		}
	}
	// 4, MaterialDataを返す
	return materialData;
}

// マルチスレッド化予定
ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;            // 構築するModelData
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate);
	modelData.matVertexData.resize(scene->mNumMaterials - 1);
	assert(scene->HasMeshes()); // メッシュが無いのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線が無いMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordsがないMeshは今回は非対応
		// ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			aiFace& face = mesh->mFaces[faceIndex];
			
			assert(face.mNumIndices == 3); // 3角形のみサポート
			// ここからFaceの中身(Vertex)の解析を行っていく
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
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
				if (mesh->mMaterialIndex == 0)
				{
					continue;
				}
				modelData.matVertexData.at(mesh->mMaterialIndex - 1).vertices.push_back(vertex);
			}

		}
	}
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
	{
		if (materialIndex == 0)
		{
			continue;
		}

		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0)
		{
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			MaterialData matData;
			matData.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();

			// テクスチャ読み込み
			TextureManager::GetInstance()->LoadTexture(matData.textureFilePath);
			// 読み込んだテクスチャの番号尾を取得
			matData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(matData.textureFilePath);

			modelData.material.push_back(matData);
		}
		else
		{
			MaterialData matData;
			matData.textureFilePath = "Resources/Debug/white1x1.png";

			// テクスチャ読み込み
			TextureManager::GetInstance()->LoadTexture(matData.textureFilePath);
			// 読み込んだテクスチャの番号尾を取得
			matData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(matData.textureFilePath);

			modelData.material.push_back(matData);
		}
	}
	return modelData;
}

void Model::CreateVertexResource() {
	for (uint32_t i = 0; i < modelData.material.size(); i++)
	{
		// 頂点リソースの作成
		Microsoft::WRL::ComPtr<ID3D12Resource> vResource = ModelBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(VertexData) * modelData.matVertexData.at(i).vertices.size());
		vertexResource[i] = vResource;
	}
}

void Model::CreateVertexBufferView() {
	for (uint32_t i = 0; i < modelData.material.size(); i++)
	{
		// 頂点バッファビューを作成する
		D3D12_VERTEX_BUFFER_VIEW vBV;
		vBV.BufferLocation = vertexResource.at(i)->GetGPUVirtualAddress();
		vBV.SizeInBytes = UINT(sizeof(VertexData) * modelData.matVertexData.at(i).vertices.size()); // 使用するリソースのサイズは頂点サイズ
		vBV.StrideInBytes = sizeof(VertexData);                                 // １頂点あたりのサイズ
		vertexBufferView[i] = vBV;
	}
}

void Model::CreateMaterialResouce() { 
	materialResource = ModelBase::GetInstance()->GetDxBase()->CreateBufferResource(sizeof(Material)); 
}