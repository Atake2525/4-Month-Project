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
	const Vector3& UpdateCollisionX(const AABB& playerAABB, const float& playerVelocityX) const;

	const ColNormal& IsColX(const AABB& playerAABB, const float& playerVelocityX, const float& speed) const;

	// 衝突判定Yの相手を設定して更新(必要な分移動させる)
	const Vector3& UpdateCollisionY(const AABB& playerAABB, const float& playerVelocityY) const;

	// 衝突判定Zの相手を設定して更新(必要な分移動させる)
	const Vector3& UpdateCollisionZ(const AABB& playerAABB, const float& playerVelocityZ) const;

	const ColNormal& IsColZ(const AABB& playerAABB, const float& playerVelocityZ, const float& speed) const;

	// 衝突判定Yの上部に衝突しているかをboolで返す
	const bool& IsColYUpside(const AABB& playerAABB, const float& playerVelocityY) const;

	// 衝突判定Yの下部に衝突しているかをboolで返す
	const bool& IsColYUnderside(const AABB& playerAABB, const float& playerVelocityY) const;

	LenXZ GetLenXZPos(const AABB& playerAABB, const Vector3& playerVelocity) const;

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
	const uint32_t GetCollisionListSize() const { return collisionListPlate.size(); }

	// カメラ用の衝突判定
	const Vector3& UpdateCameraCollision(const AABB& cameraAABB, const AABB& playerAABB, const Vector3& cameraVelocity, const Vector3& cameraOffset) const;

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

	const bool& CollisionAABB(const AABB& a, const AABB& b) const;

	const bool& CollisionAABBMin(const AABB& a, const AABB& b) const;

	const bool& CollisionAABBMax(const AABB& a, const AABB& b) const;

	const bool& IsCollisionOBB(const OBB& obb1, const OBB& obb2) const;

	const bool& IsCollision(const AABB& aabb, const Segment& segment) const;

	// 衝突判定が計算される距離
	float collisionDistance = 1000.0f;

	std::vector<Plate> collisionListPlate;

	LenXZ lenXz = LenXZ::X;
};

