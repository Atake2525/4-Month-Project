#pragma once
//#include <AABB.h> //後で追加
#include "Vector3.h" 

#include "Model.h" 
#include <Object3d.h>

#include <ModelManager.h>

#include <algorithm>
#include <numbers>

class Player;//後で追加

class Goal
{
public:
	// 初期化
	void Initialize(Model* model,const Vector3& position); //Camera* viewProjection, 
	// 更新
	void Update();
	//描画
	void Draw();

	//当たり判定
	void OnCollision(const Player* player);
	//当たり判定
	//AABB GetAABB();

	//エフェクト
	void GoalEffect();

private:

	Vector3 position_;  // ゴールの位置
	//Camera* viewProjection_ = nullptr;
	Model* GoalModel_ = nullptr;      // objモデルデータ
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 終了フラグ
	bool isFinished_ = false;

};

