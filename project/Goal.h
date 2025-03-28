#pragma once
#include "kMath.h"
#include "Vector3.h" 
#include "Model.h"
#include "Camera.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "ModelBase.h"
#include "ModelManager.h"
#include "DirectXBase.h"

class Player; // プレイヤークラスの前方宣言（後で追加）

// ゴールクラス
class Goal {
public:
	// デストラクタ
	~Goal();

	// 初期化処理
	// @param position: ゴールの位置
	// @param dxc: DirectXの基盤クラス
	void Initialize(Vector3 position, DirectXBase* dxc);

	// 更新処理
	void Update();

	// 描画処理
	// @param 第1引数: 平行光源のリソース
	// @param 第2引数: 点光源のリソース
	// @param 第3引数: スポットライトのリソース
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource,
		Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource,
		Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource);

	// 当たり判定
	// @param player: 衝突を確認するプレイヤーオブジェクト
	void OnCollision(const Player* player);

	// ゴール到達フラグを取得
	// @return ゴール到達状態（true: ゴール済み, false: 未到達）
	bool GetFlag() { return goalFlag_; }

private:
	DirectXBase* directX;   // DirectXの基盤クラス
	Object3d* goalModel_;   // ゴールの3Dオブジェクトモデル
	Vector3 goalPos_;       // ゴールの位置
	Camera* goalCamera_;    // ゴール用のカメラ

	bool goalFlag_ = false; // ゴール到達フラグ

	Goal* goal_ = nullptr;  // ゴールオブジェクト（※不要な可能性あり）
	Sprite* clearSprite_ = nullptr; // クリア画面用のスプライト
};
