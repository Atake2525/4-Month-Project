#include "Vector3.h"
#include "AABB.h"
#include "vector"

#pragma once


class PlayerCollision {
public:

	~PlayerCollision();

	//初期化
	//void Initialize(Player* player);

	// 衝突判定の相手を設定して更新
	const Vector3& UpdateCollision(const AABB& playerAABB) const;

	// 衝突判定の追加
	//void AddCollision(const AABB& collisionAABB);

	// 衝突判定の追加(壁)
	void AddCollision(const AABB& AABB, const Vector3& collisionNormal);

	const bool& CollisionAABB(const AABB& a, const AABB& b) const;

private:
	struct Plate
	{
		AABB aabb;
		Vector3 normal;
	};
	//AABB playerAABB;

	//Player* player_ = nullptr;

	//std::vector<AABB> collisionListAABB;
	std::vector<Plate> collisionListPlate;
};

