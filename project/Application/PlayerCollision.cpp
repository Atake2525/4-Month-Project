#include "PlayerCollision.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>
#define NOMINMAX

// 衝突判定の相手を設定して更新
const Vector3& PlayerCollision::UpdateCollision(const AABB& playerAABB) const {
	Vector3 result = { 0.0f, 0.0f, 0.0f };

	// 壁の衝突判定
	for (Plate collisionPlate : collisionListPlate)
	{
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.x == 1.0f)
			{
				float moveAmount = collisionPlate.aabb.max.x - playerAABB.min.x;
				result.x = moveAmount;
			}
			else if (collisionPlate.normal.x == -1.0f)
			{
				float moveAmount = playerAABB.max.x - collisionPlate.aabb.max.x;
				result.x = -moveAmount;
			}
			else if (collisionPlate.normal.z == 1.0f)
			{
				float moveAmount = collisionPlate.aabb.max.z - playerAABB.min.z;
				result.z = moveAmount;
			}
			else if (collisionPlate.normal.z == -1.0f)
			{
				float moveAmount = playerAABB.max.z - collisionPlate.aabb.max.z;
				result.z = -moveAmount;
			}
		}
	}
	return result;
}

// 衝突判定の追加(壁)
void PlayerCollision::AddCollision(const std::string& directoryPath, const std::string& filename) {

	const float epsilon = 1e-6;

	std::vector<VertexData> vertices;
	AABB aabb;
	Vector3 collisionNormal = { 0.0f, 0.0f, 0.0f };
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュが無いのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線が無いMeshは今回は非対応
		// ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			aiFace& face = mesh->mFaces[faceIndex];

			assert(face.mNumIndices == 4); // 4角形のみサポート
			// ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element)
			{
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				// 数字が極めて小さい時ほぼ0に変換する
				if (fabs(position.x) < epsilon)
				{
					position.x = 0.00000f;
				}
				if (fabs(position.y) < epsilon)
				{
					position.y = 0.00000f;
				}
				if (fabs(position.z) < epsilon)
				{
					position.z = 0.00000f;
				}
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				// 先にaabbにモデルの情報を入れておく(原点からの比較だと計算に誤りが発生する為)
				if (element == 1)
				{
					aabb.min.x = vertex.position.x;
					aabb.min.y = vertex.position.y;
					aabb.min.z = vertex.position.z;
					aabb.max.x = vertex.position.x;
					aabb.max.y = vertex.position.y;
					aabb.max.z = vertex.position.z;
					collisionNormal = vertex.normal;
				}
				vertices.push_back(vertex);
			}
			// AABBを生成する
			for (VertexData vertices : vertices)
			{
				aabb.min.x = std::min(aabb.min.x, vertices.position.x);
				aabb.min.y = std::min(aabb.min.y, vertices.position.y);
				aabb.min.z = std::min(aabb.min.z, vertices.position.z);

				aabb.max.x = std::max(aabb.max.x, vertices.position.x);
				aabb.max.y = std::max(aabb.max.y, vertices.position.y);
				aabb.max.z = std::max(aabb.max.z, vertices.position.z);

			}

			// 平面衝突判定の構造体を作成
			Plate plate = { aabb, collisionNormal };
			// listに登録(vector)
			collisionListPlate.push_back(plate); 
			// 参照したいのは1つの平面情報だけなので以前取得した頂点情報をクリア
			vertices.clear();
		}
	}

}

const bool& PlayerCollision::CollisionAABB(const AABB& a, const AABB& b) const {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}