#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "AABB.h"
#include "Plane.h"
#include "Sphere.h"
#include "OBB.h"

//enum class CollisionType {
//	Sphere,
//	AABB,
//	OBB,
//	Plat,
//};


class CollisionManager {
//private:
//	// シングルトンパターンの適用
//	static CollisionManager* instance;
//
//	// コンストラクタ、デストラクタの隠蔽
//	CollisionManager() = default;
//	~CollisionManager() = default;
//	// コピーコンストラクタ、コピー代入演算子の封印
//	CollisionManager(CollisionManager&) = delete;
//	CollisionManager& operator=(CollisionManager&) = delete;

public:

	//static CollisionManager* GetInstance();

	void Initialize();

	void Update();

	//void Finalize();

	// 当たり判定の追加
	const bool CheckCollision(const AABB& target1, const AABB& target2);
	const bool CheckCollision(AABB& target1, Sphere& target2);
	const bool CheckCollision(AABB& target1, OBB& target2);
};

