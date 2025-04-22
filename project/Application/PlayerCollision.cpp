#include "PlayerCollision.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "kMath.h"
#include <cassert>
#define NOMINMAX

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

const bool& PlayerCollision::IsCollisionOBB(const OBB& obb1, const OBB& obb2) const {

	// 分離軸リスト
	Vector3 axes[15];
	for (int i = 0; i < 3; ++i) {
		axes[i] = obb1.orientations[i]; // obb1の軸
		axes[3 + i] = obb2.orientations[i]; // obb2の軸
	}

	// obb1 と obb2 の軸のクロス積
	int k = 6;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			axes[k++] = Cross(obb1.orientations[i], obb2.orientations[j]);
		}
	}

	// 各分離軸について調べる
	for (int i = 0; i < 15; ++i) {
		Vector3 axis = axes[i];
		if (axis.x == 0 && axis.y == 0 && axis.z == 0) continue; // ゼロベクトルは無視

		axis = Normalize(axis); // 軸を正規化

		// 各OBBの半径を計算
		float r1 =
			std::abs(Dot(axis, obb1.orientations[0] * obb1.size.x)) +
			std::abs(Dot(axis, obb1.orientations[1] * obb1.size.y)) +
			std::abs(Dot(axis, obb1.orientations[2] * obb1.size.z));

		float r2 =
			std::abs(Dot(axis, obb2.orientations[0] * obb2.size.x)) +
			std::abs(Dot(axis, obb2.orientations[1] * obb2.size.y)) +
			std::abs(Dot(axis, obb2.orientations[2] * obb2.size.z));

		// 投影距離
		Vector3 distance = obb2.center - obb1.center;
		float projectedDistance = std::abs(Dot(distance, axis));

		if (projectedDistance > r1 + r2) {
			return false; // 投影が重ならない -> 当たり判定なし
		}
	}

	return true; // すべての軸で投影が重なる -> 当たり判定あり
}


// 衝突判定の相手を設定して更新
const Vector3& PlayerCollision::UpdateCollisionX(const AABB& playerAABB, const float& playerVelocityX) const {
	Vector3 result = { 0.0f, 0.0f, 0.0f };

	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		float objectLen = Distance(collisionPlate.aabb.min, collisionPlate.aabb.max);
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::DragFloat("objectLen", &objectLen);
		ImGui::End();*/
		if (dist > objectLen)
		{
			continue;
		}
		if (playerAABB.max.y > collisionPlate.aabb.max.y)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.x == 1.0f && playerVelocityX < 0.0f)
			{
				float moveAmount = collisionPlate.aabb.max.x - playerAABB.min.x;
				result.x = moveAmount;
			}
			else if (collisionPlate.normal.x == -1.0f && playerVelocityX > 0.0f)
			{
				float moveAmount = playerAABB.max.x - collisionPlate.aabb.max.x;
				result.x = -moveAmount;
			}
		}
	}
	return result;
}

// 衝突判定の相手を設定して更新
const Vector3& PlayerCollision::UpdateCollisionY(const AABB& playerAABB, const float& playerVelocityY) const {
	Vector3 result = { 0.0f, 0.0f, 0.0f };

	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		/*if (playerAABB.max.y > collisionPlate.aabb.max.y)
		{
			continue;
		}*/
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.y == 1.0f && playerVelocityY < 0.0f)
			{
				float moveAmount = collisionPlate.aabb.max.y - playerAABB.min.y;
				result.y = moveAmount;
			}
			else if (collisionPlate.normal.y == -1.0f && playerVelocityY > 0.0f)
			{
				float moveAmount = playerAABB.max.y - collisionPlate.aabb.max.y;
				result.y = -moveAmount;
			}
		}
	}
	return result;
}

// 衝突判定の相手を設定して更新
const Vector3& PlayerCollision::UpdateCollisionZ(const AABB& playerAABB, const float& playerVelocityZ) const {
	Vector3 result = { 0.0f, 0.0f, 0.0f };

	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		if (playerAABB.max.y > collisionPlate.aabb.max.y)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.z == 1.0f && playerVelocityZ < 0.0f)
			{
				float moveAmount = collisionPlate.aabb.max.z - playerAABB.min.z;
				result.z = moveAmount;
			}
			else if (collisionPlate.normal.z == -1.0f && playerVelocityZ > 0.0f)
			{
				float moveAmount = playerAABB.max.z - collisionPlate.aabb.min.z;
				result.z = -moveAmount;
			}
		}
	}
	return result;
}

const ColNormal& PlayerCollision::IsColZ(const AABB& playerAABB, const float& playerVelocityZ) const {
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		if (playerAABB.max.z > collisionPlate.aabb.max.z)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.z == 1.0f && playerVelocityZ < 0.0f)
			{
				return ColNormal::Front;
			}
			else if (collisionPlate.normal.z == -1.0f && playerVelocityZ > 0.0f)
			{
				return ColNormal::Back;
			}
		}
	}
	return ColNormal::None;
}

const ColNormal& PlayerCollision::IsColX(const AABB& playerAABB, const float& playerVelocityX) const {
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		if (playerAABB.max.x > collisionPlate.aabb.max.x)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.x == 1.0f && playerVelocityX < 0.0f)
			{
				return ColNormal::Front;
			}
			else if (collisionPlate.normal.x == -1.0f && playerVelocityX > 0.0f)
			{
				return ColNormal::Back;
			}
		}
	}
	return ColNormal::None;
}

// 衝突判定Yの上部に衝突しているかをboolで返す
const bool& PlayerCollision::IsColYUpside(const AABB& playerAABB, const float& playerVelocityY) const {
	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.y == 1.0f && playerVelocityY < 0.0f)
			{
				return true;
			}
		}
	}
	return false;
}

// 衝突判定Yの下部に衝突しているかをboolで返す
const bool& PlayerCollision::IsColYUnderside(const AABB& playerAABB, const float& playerVelocityY) const {

	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		if (dist > collisionDistance)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.y == -1.0f && playerVelocityY > 0.0f)
			{
				return true;
			}
		}
	}
	return false;
}

const LenXZ& PlayerCollision::GetLenXZ(const AABB& playerAABB, const Vector3& playerVelocity) const {
	int in = 0;
	int num = 0;
	float len = Distance(collisionListPlate.at(1).aabb.max, playerAABB.max);
	for (const auto& collisionPlate : collisionListPlate)
	{
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);

		if (len > dist)
		{
			len = dist;
			num = in;
		}
		in++;
	}
	// プレイヤーの中心を求める
	Vector3 center = (collisionListPlate.at(num).aabb.max - collisionListPlate.at(num).aabb.min) / 2 + collisionListPlate.at(num).aabb.min;

	// 衝突判定用のAABBの作成
	AABB centerAABB = { center, center };
	// 衝突判定をAABBとAABBでとる
	if (CollisionAABB(playerAABB, centerAABB))
	{
		//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
		if (collisionListPlate.at(num).normal.x == 1.0f || collisionListPlate.at(num).normal.x == -1.0f)
		{
			return LenXZ::X;
		}
		else if (collisionListPlate.at(num).normal.z == 1.0f || collisionListPlate.at(num).normal.z == -1.0f)
		{
			return LenXZ::Z;
		}
	}
	ImGui::Begin("collisionDistance");
	ImGui::DragFloat("Len", &len);
	ImGui::End();
	return LenXZ::Z;
}

// 衝突判定の追加(壁)
void PlayerCollision::AddCollision(const std::string& directoryPath, const std::string& filename) {

	const float epsilon = 1e-6;

	std::vector<VertexData> vertices;
	AABB aabb;
	OBB obb;

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
			for (const auto& vertex : vertices)
			{
				aabb.min.x = std::min(aabb.min.x, vertex.position.x);
				aabb.min.y = std::min(aabb.min.y, vertex.position.y);
				aabb.min.z = std::min(aabb.min.z, vertex.position.z);

				aabb.max.x = std::max(aabb.max.x, vertex.position.x);
				aabb.max.y = std::max(aabb.max.y, vertex.position.y);
				aabb.max.z = std::max(aabb.max.z, vertex.position.z);

			}

			obb.center = aabb.max / 2;

			Matrix4x4 rotate = Multiply(MakeRotateXMatrix(0.0f), Multiply(MakeRotateYMatrix(0.0f), MakeRotateZMatrix(0.0f)));

			for (int i = 0; i < 3; i++)
			{
				obb.orientations[i].x = rotate.m[i][0];
				obb.orientations[i].y = rotate.m[i][1];
				obb.orientations[i].z = rotate.m[i][2];
			}

			obb.size = aabb.max;

			// 平面衝突判定の構造体を作成
			Plate plate = { aabb, obb, collisionNormal };
			// listに登録(vector)
			collisionListPlate.push_back(plate);
			// 参照したいのは1つの平面情報だけなので以前取得した頂点情報をクリア
			vertices.clear();
		}
	}

}

const bool& PlayerCollision::CollisionAABB(const AABB& a, const AABB& b) const {
	if ((a.min.x < b.max.x && a.max.x > b.min.x) &&
		(a.min.y < b.max.y && a.max.y > b.min.y) &&
		(a.min.z < b.max.z && a.max.z > b.min.z)) {
		return true;
	}
	return false;
}