#pragma once
#include "Vector3.h"  
#include "Object3d.h"
#include "DirectXBase.h"
#include "ModelManager.h"

class Player;

class Star {
public:
	~Star();

	// 初期化
	void Initialize(Vector3 position, DirectXBase* dxc);

	// 更新
	void Update();

	// 描画
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

	// 当たり判定
	void OnCollision(const Player* player);

	bool IsCollected() const { return collected_; }
	const Vector3& GetPosition() const { return starPos_; }


private:
	DirectXBase* directX_;
	Object3d* starModel_;  // 星のモデル
	Vector3 starPos_;      // 星の位置

	bool collected_ = false; // 取得フラグ
};
