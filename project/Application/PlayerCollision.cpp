#include "PlayerCollision.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Plane.h"
#include "kMath.h"
#include <cassert>
#define NOMINMAX

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

bool PlayerCollision::IsCollisionOBB(const OBB& obb1, const OBB& obb2) {

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
Vector3 PlayerCollision::UpdateCollisionX(const AABB& playerAABB, const float& playerVelocityX) {
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
		if (playerAABB.min.y + 0.1f > collisionPlate.aabb.max.y)
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
Vector3 PlayerCollision::UpdateCollisionY(const AABB& playerAABB, const float& playerVelocityY) {
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
				result.y = moveAmount - 0.01f;
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
Vector3 PlayerCollision::UpdateCollisionZ(const AABB& playerAABB, const float& playerVelocityZ) {
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
		if (playerAABB.min.y + 0.1f > collisionPlate.aabb.max.y)
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
				float moveAmount = playerAABB.max.z - collisionPlate.aabb.max.z;
				result.z = -moveAmount;
			}
		}
	}
	return result;
}

Vector3 PlayerCollision::UpdateCameraCollision(const AABB& cameraAABB, const AABB& playerAABB, const Vector3& cameraVelocity, const Vector3& cameraOffset, const Vector3& defaultCameraOffset) {
	Vector3 result = cameraOffset;


	// cameraOffsetをNormalize(正規化)しておく
	Vector3 normalCameraOffset = Normalize(cameraOffset);
	// cameraOffsetの要素の割合を計算する
	Vector3 cameraRate = { 0.0f, std::abs(defaultCameraOffset.y) / std::abs(defaultCameraOffset.z), std::abs(defaultCameraOffset.z) / std::abs(defaultCameraOffset.y) };
	// プレイヤーとカメラの距離を計算
	// AABBではなく中心座標からの距離が欲しいので中心座標を計算する
	Vector3 plCenterPos = CenterAABB(playerAABB);
	Vector3 camCenterPos = CenterAABB(cameraAABB);

	Vector3 origin;
	origin.x = 0.0f;
	origin.z = camCenterPos.z - 5.0f;
	origin.y = camCenterPos.z * cameraRate.y * -1.0f;

	Vector3 diff = plCenterPos - camCenterPos;
	Segment segment = {
		.origin = camCenterPos + cameraVelocity,
		.diff = diff,
	};

	Sphere sphere;
	sphere.center = camCenterPos;
	sphere.radius = 2.0f;

	float plcamDist = Distance(plCenterPos, camCenterPos);

	std::vector<float> distList;

	// 壁の衝突判定
	for (const auto& collisionPlate : collisionListPlate)
	{
		//float objectLen = Distance(collisionPlate.aabb.min, collisionPlate.aabb.max);
		Vector3 collisionPlateCenterPos = CenterAABB(collisionPlate.aabb);
		Vector3 collisionPlateSize = (collisionPlate.aabb.max - collisionPlate.aabb.min) / 2;
		// 判定対象とプレイヤーとの距離を計算
		float dist = Distance(collisionPlateCenterPos + collisionPlateSize, plCenterPos);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::DragFloat("objectLen", &objectLen);
		ImGui::End();*/
		// 判定対象がプレイヤーとカメラの距離
		/*if (dist > plcamDist)
		{
			continue;
		}*/
		if (cameraAABB.min.y + 0.1f > collisionPlate.aabb.max.y)
		{
			continue;
		}

		// プレイヤーから最も近い判定対象に対して衝突判定をとる
		//if (CollisionAABB(cameraAABB, collisionPlate.aabb))
		//{
		//	result.z = (Distance(collisionPlateCenterPos, plCenterPos) * -1.0f) - 2.0f;
		//}
		//// 判定対象からプレイヤーまでの距離を求める
		//result.y = result.z * cameraRate.y * -1.0f;

		if (IsCollision(collisionPlate.aabb, segment)) 
		{
			Vector3 closestPoint;
			closestPoint.x = std::clamp(plCenterPos.x, collisionPlate.aabb.min.x, collisionPlate.aabb.max.x);
			closestPoint.y = std::clamp(plCenterPos.y, collisionPlate.aabb.min.y, collisionPlate.aabb.max.y);
			closestPoint.z = std::clamp(plCenterPos.z, collisionPlate.aabb.min.z, collisionPlate.aabb.max.z);
			result.z = (Distance(closestPoint, plCenterPos) * -1.0f);
			//result.z = closestPoint.z;
		}
		if (IsCollision(collisionPlate.aabb, sphere))
		{
			Vector3 closestPoint;
			closestPoint.x = std::clamp(plCenterPos.x, collisionPlate.aabb.min.x, collisionPlate.aabb.max.x);
			closestPoint.y = std::clamp(plCenterPos.y, collisionPlate.aabb.min.y, collisionPlate.aabb.max.y);
			closestPoint.z = std::clamp(plCenterPos.z, collisionPlate.aabb.min.z, collisionPlate.aabb.max.z);
			result.z = (Distance(closestPoint, plCenterPos) * -1.0f);
			//result.z = closestPoint.z;

		}

		// 複数のオブジェクトに接触しているときに対処するため一時的にlistに格納する
		if (IsCollision(collisionPlate.aabb, segment) || IsCollision(collisionPlate.aabb, sphere))
		{
			distList.push_back(result.z);
		}

	}

	// よりプレイヤーから近い距離のresultを使用する
	//float resultDist = 20.0f;
	for (float dist : distList)
	{
		if (result.z < dist)
		{
			result.z = dist;
		}
	}
	distList.clear();

	result.x = std::max(result.x, cameraOffset.x);
	result.y = std::max(result.y, cameraOffset.y);
	result.z = std::clamp(result.z, cameraOffset.z, -2.0f);
	// 判定対象からプレイヤーまでの距離を求める
	result.y = result.z * cameraRate.y * -1.0f;

	return result;
}

ColNormal PlayerCollision::IsColZ(const AABB& playerAABB, const float& playerVelocityZ, const float& speed) {
	for (const auto& collisionPlate : collisionListPlate)
	{
		float objectLen = Distance(collisionPlate.aabb.min, collisionPlate.aabb.max);
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::DragFloat("objectLen", &objectLen);
		ImGui::End();*/
		AABB plAABB = playerAABB;
		plAABB.min.x = playerAABB.min.x + playerVelocityZ * speed;
		plAABB.max.x = playerAABB.max.x + playerVelocityZ * speed;
		if (dist > objectLen)
		{
			continue;
		}
		if (plAABB.max.y > collisionPlate.aabb.max.y)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(plAABB, collisionPlate.aabb))
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

ColNormal PlayerCollision::IsColX(const AABB& playerAABB, const float& playerVelocityX, const float& speed) {
	for (const auto& collisionPlate : collisionListPlate)
	{
		float objectLen = Distance(collisionPlate.aabb.min, collisionPlate.aabb.max);
		float dist = Distance(collisionPlate.aabb.max, playerAABB.max);
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::DragFloat("objectLen", &objectLen);
		ImGui::End();*/
		/*ImGui::Begin("collisionDistance");
		ImGui::DragFloat("dist", &dist);
		ImGui::End();*/
		AABB plAABB = playerAABB;
		plAABB.min.x = playerAABB.min.x + playerVelocityX * speed;
		plAABB.max.x = playerAABB.max.x + playerVelocityX * speed;

		if (dist > objectLen)
		{
			continue;
		}
		if (plAABB.max.y > collisionPlate.aabb.max.y)
		{
			continue;
		}
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(plAABB, collisionPlate.aabb))
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
bool PlayerCollision::IsColYUpside(const AABB& playerAABB, const float& playerVelocityY) {
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
		// 衝突判定をAABBとAABBでとる
		if (CollisionAABB(playerAABB, collisionPlate.aabb))
		{
			//　壁の向いている方向からプレイヤーがどれくらい移動すればよいかを出す
			if (collisionPlate.normal.y == 1.0f && (playerVelocityY < 0.0f || playerVelocityY > 0.0f))
			{
				return true;
			}
		}
	}
	return false;
}

// 衝突判定Yの下部に衝突しているかをboolで返す
bool PlayerCollision::IsColYUnderside(const AABB& playerAABB, const float& playerVelocityY) {

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

LenXZ PlayerCollision::GetLenXZPos(const AABB& playerAABB, const Vector3& playerVelocity) {
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
	ImGui::Begin("collisionDistance");
	ImGui::DragFloat("Len", &len);
	ImGui::End();
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
	return LenXZ::Z;
}

void PlayerCollision::GetLenXZVelocity(const Vector3& playerVelocity)
{
	Vector3 absVelocity = { 0.0f, 0.0f, 0.0f };
	absVelocity.x = std::abs(playerVelocity.x);
	absVelocity.z = std::abs(playerVelocity.z);

	if (absVelocity.x > absVelocity.z)
	{
		lenXz = LenXZ::X;
	}
	else if (absVelocity.x < absVelocity.z)
	{
		lenXz = LenXZ::Z;
	}
}

// 衝突判定の追加(壁)
void PlayerCollision::AddCollision(const std::string& directoryPath, const std::string& filename) {

	//const float epsilon = 1e-6;

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
				/*if (fabs(position.x) < epsilon)
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
				}*/
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

void PlayerCollision::ClearCollisionList() {
	collisionListPlate.clear();
}

bool PlayerCollision::CollisionAABB(const AABB& a, const AABB& b) {
	if ((a.min.x < b.max.x && a.max.x > b.min.x) &&
		(a.min.y < b.max.y && a.max.y > b.min.y) &&
		(a.min.z < b.max.z && a.max.z > b.min.z)) {
		return true;
	}
	return false;
}

bool PlayerCollision::CollisionAABBMin(const AABB& a, const AABB& b) {
	if ((a.max.x > b.min.x) &&
		(a.max.y > b.min.y) &&
		(a.max.z > b.min.z)) {
		return true;
	}
	return false;
}

bool PlayerCollision::CollisionAABBMax(const AABB& a, const AABB& b) {
	if ((a.min.x < b.max.x) &&
		(a.min.y < b.max.y) &&
		(a.min.z < b.max.z)) {
		return true;
	}
	return false;
}

bool PlayerCollision::IsCollision(const AABB& aabb, const Segment& segment) {
	Vector3 seg1 = segment.origin;
	Vector3 seg2 = segment.origin + segment.diff;

	float tMin = 0.0f;
	float tMax = 1.0f;

	// X軸方向での判定
	if (std::abs(segment.diff.x) < 1e-8) {
		if (seg1.x < aabb.min.x || seg1.x > aabb.max.x) {
			return false;
		}
	}
	else {
		float od = 1.0f / segment.diff.x;
		float t1 = (aabb.min.x - seg1.x) * od;
		float t2 = (aabb.max.x - seg1.x) * od;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return false;
	}

	// Y軸方向での判定
	if (std::abs(segment.diff.y) < 1e-8) {
		if (seg1.y < aabb.min.y || seg1.y > aabb.max.y) {
			return false;
		}
	}
	else {
		float od = 1.0f / segment.diff.y;
		float t1 = (aabb.min.y - seg1.y) * od;
		float t2 = (aabb.max.y - seg1.y) * od;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return false;
	}

	// Z軸方向での判定
	if (std::abs(segment.diff.z) < 1e-8) {
		if (seg1.z < aabb.min.z || seg1.z > aabb.max.z) {
			return false;
		}
	}
	else {
		float od = 1.0f / segment.diff.z;
		float t1 = (aabb.min.z - seg1.z) * od;
		float t2 = (aabb.max.z - seg1.z) * od;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return false;
	}

	// すべての軸方向での判定を通過した場合、衝突している
	return true;
}

bool PlayerCollision::IsCollision(const AABB& aabb, const Sphere& sphere) {
	//　最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};
	// 最近接点と弾の中心との距離を求める
	float distance = Distance(closestPoint, sphere.center);

	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		// 衝突
		return true;
	}
	return false;
}
