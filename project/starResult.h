#pragma once
#include "Star.h"
#include <vector>

class starResult {
public:
    ~starResult();
    void Initialize(DirectXBase* dxc);
    void Update(Vector3 playerPos);
    void Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource,
        Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource,
        Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource);

    int GetScore() const { return score_; }

private:
    std::vector<Star*> stars_;  // 星のリスト
    DirectXBase* directX_;
    int score_ = 0;
};
