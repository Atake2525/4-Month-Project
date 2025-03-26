#pragma once
#include "Star.h"
#include <vector>

class starResult {
public:
	~starResult();

	void Initialize( DirectXBase* dxc);
	void Update();
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

	//int GetScore() const { return score_; }

private:
	std::vector<Star*> stars_;  // 星のリスト
	DirectXBase* directX_;
	//int score_ = 0;
};
