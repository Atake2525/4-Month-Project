#include "PlayerCollision.h"
//#include "Player.h"


//PlayerCollision::~PlayerCollision() {
//	collisionListPlate.clear();
//	//delete player_;
//}

//void PlayerCollision::Initialize(Player* player) {
//	player_ = player;
//}

// 衝突判定の相手を設定して更新
const Vector3& PlayerCollision::UpdateCollision(const AABB& playerAABB) const {
	Vector3 result = { 0.0f, 0.0f, 0.0f };
	/*playerAABB = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};*/
	//playerAABB = player_->GetAABB();
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
	//player_->AddTranslate(result);
	return result;
}

// 衝突判定の追加
//void PlayerCollision::AddCollision(const AABB& collisionAABB) {
//	collisionListAABB.push_back(collisionAABB);       

// 衝突判定の追加(壁、床)
void PlayerCollision::AddCollision(const AABB& AABB, const Vector3& collisionNormal) {
	Plate plate = { AABB, collisionNormal };
	collisionListPlate.push_back(plate);
}

const bool& PlayerCollision::CollisionAABB(const AABB& a, const AABB& b) const {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}