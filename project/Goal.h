#pragma once
#include <AABB.h> //後で追加
#include "Vector3.h" 
#include "Model.h" 
#include <Object3d.h>

class Player;//後で追加

class Goal
{
public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	// 更新
	void Update();
	//描画
	void Draw();

	//当たり判定
	void OnCollision(const Player* player);
	//当たり判定
	AABB GetAABB();

	//エフェクト
	void GoalEffect();

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	Vector3 position_;  // ゴールの位置
	Model* model_ = nullptr;      // objモデルデータ


};

