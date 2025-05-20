#include "Vector3.h"
#include "Vector4.h"
#include "OBB.h"
#include "AABB.h"
#include "vector"
#include "Segment.h"
#include <string>

#pragma once

enum LenXZ {
	X,
	Z,
};

enum ColNormal {
	None,
	Front,
	Back,
};

class PlayerCollision {
public:

	//~PlayerCollision();

	//初期化
	//void Initialize(Player* player);

	// 衝突判定Xの相手を設定して更新(必要な分移動させる)
	Vector3 UpdateCollisionX(const AABB& playerAABB, const float& playerVelocityX);

	ColNormal IsColX(const AABB& playerAABB, const float& playerVelocityX, const float& speed);

	// 衝突判定Yの相手を設定して更新(必要な分移動させる)
	Vector3 UpdateCollisionY(const AABB& playerAABB, const float& playerVelocityY);

	// 衝突判定Zの相手を設定して更新(必要な分移動させる)
	Vector3 UpdateCollisionZ(const AABB& playerAABB, const float& playerVelocityZ);

	ColNormal IsColZ(const AABB& playerAABB, const float& playerVelocityZ, const float& speed);

	// 衝突判定Yの上部に衝突しているかをboolで返す
	bool IsColYUpside(const AABB& playerAABB, const float& playerVelocityY);

	// 衝突判定Yの下部に衝突しているかをboolで返す
	bool IsColYUnderside(const AABB& playerAABB, const float& playerVelocityY);

	LenXZ GetLenXZPos(const AABB& playerAABB, const Vector3& playerVelocity);

	void GetLenXZVelocity(const Vector3& playerVelocity);

	const LenXZ GetLenXZ() const { return lenXz; }
	/// <summary>
	/// 衝突判定の追加(壁)
	/// </summary>
	/// <param name="directoryPath">ディレクトリのパス</param>
	/// <param name="filename">ファイルの名前</param>
	void AddCollision(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 衝突判定の削除
	/// </summary>
	void ClearCollisionList();

	// 登録されている衝突判定のサイズを取得
	const size_t GetCollisionListSize() const { return collisionListPlate.size(); }

	// カメラ用の衝突判定
	Vector3 UpdateCameraCollision(const AABB& cameraAABB, const AABB& playerAABB, const Vector3& cameraVelocity, const Vector3& cameraOffset);

private:

	struct VertexData {
		Vector4 position;
		Vector3 normal;
	};

	struct Plate
	{
		AABB aabb;
		OBB obb;
		Vector3 normal;
	};

	bool CollisionAABB(const AABB& a, const AABB& b);

	bool CollisionAABBMin(const AABB& a, const AABB& b);

	bool CollisionAABBMax(const AABB& a, const AABB& b);

	bool IsCollisionOBB(const OBB& obb1, const OBB& obb2);

	bool IsCollision(const AABB& aabb, const Segment& segment);

	// 衝突判定が計算される距離
	float collisionDistance = 1000.0f;

	std::vector<Plate> collisionListPlate;

	LenXZ lenXz = LenXZ::X;
};

