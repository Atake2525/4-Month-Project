#pragma once
#include "Vector3.h" 

#include "Model.h"

#include"kMath.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"DirectXBase.h"
#include"Camera.h"

class Player;//後で追加

class Goal
{
public:

	~Goal();
	// 初期化
	void Initialize(Vector3, Camera*,DirectXBase*); //Camera* viewProjection, Model* model
	// 更新
	void Update();
	//描画
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

	//当たり判定
	void OnCollision(const Player* player);
	bool GetFlag() { return goalFlag_; }

private:

	Object3d* goalModel_;// objモデルデータ
	Vector3 goalPos_;// ゴールの位置

	/*DirectX*/
	DirectXBase* directX;
	//終了フラグ
	bool goalFlag_ = false;
	/*カメラ*/
	Camera* goalCamera_;
	//ゴール
	Goal* goal_ = nullptr;


};

