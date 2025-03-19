#pragma once
#include "Star.h"
#include <vector>

#include"DirectXBase.h"

class starResult {
public:
    starResult();
    ~starResult();

    void Initialize(Vector3, DirectXBase*);
    void Update(Vector3 playerPos); // 星の更新処理
    void Draw();                    // 星の描画
    int GetScore() const { return score; }

private:
    std::vector<Star> stars; // 星のリスト
    int score;



};
