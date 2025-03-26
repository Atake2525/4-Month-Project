#pragma once
#include"kMath.h"
#include "Vector3.h" 

#include "Model.h"
#include"Camera.h"
#include "Sprite.h"

#include"Object3d.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"

#include"DirectXBase.h"


class Player;//後で追加

class Goal
{
public:

	~Goal();
	// 初期化
	void Initialize(Vector3, DirectXBase*); //Object3d*
	void Update();
	//描画
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

	//当たり判定
	void OnCollision(const Player* player);

	bool GetFlag() { return goalFlag_; }

private:

	//DirectX
	DirectXBase* directX;
	Object3d* goalModel_;	// objモデルデータ
	Vector3 goalPos_;		// ゴールの位置
	Camera* goalCamera_;	//カメラ

	//終了フラグ
	bool goalFlag_ = false;

	//ゴール
	Goal* goal_ = nullptr;

	Sprite* clearSprite_ = nullptr; // クリアスプライト
};

