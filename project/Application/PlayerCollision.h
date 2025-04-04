#include "Vector3.h"
#include "Vector4.h"
#include "OBB.h"
#include "AABB.h"
#include "vector"
#include <string>

#pragma once


class PlayerCollision {
public:

	//~PlayerCollision();

	//初期化
	//void Initialize(Player* player);

	// 衝突判定の相手を設定して更新(必要な分移動させる)
	const Vector3& UpdateCollision(const AABB& playerAABB) const;

	/// <summary>
	/// 衝突判定の追加(壁)
	/// </summary>
	/// <param name="directoryPath">ディレクトリのパス</param>
	/// <param name="filename">ファイルの名前</param>
	void AddCollision(const std::string& directoryPath, const std::string& filename);

private:

	struct VertexData {
		Vector4 position;
		Vector3 normal;
	};

	struct Plate
	{
		AABB aabb;
		Vector3 normal;
	};

	const bool& CollisionAABB(const AABB& a, const AABB& b) const;

	const bool& IsCollisionOBB(const OBB& obb1, const OBB& obb2) const;

	// 衝突判定が計算される距離
	float collisionDistance = 1000.0f;
	
	std::vector<Plate> collisionListPlate;
};

