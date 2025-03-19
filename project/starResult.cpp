#include "starResult.h"

starResult::~starResult()
{
}

starResult::starResult() : score(0) {
    // 3つの星を配置
    stars.push_back(Star(Vector3(1.0f, 0.0f, 1.0f)));
    stars.push_back(Star(Vector3(3.0f, 0.0f, 4.0f)));
    stars.push_back(Star(Vector3(5.0f, 0.0f, 2.0f)));
}

void starResult::Initialize(Vector3, DirectXBase*)
{
}

void starResult::Update(Vector3 playerPos) {
    for (auto& star : stars) {
        star.CheckCollision(playerPos, 1.0f);
        if (star.IsCollected()) {
            score += 10; // スコア加算
        }
    }
}

void starResult::Draw() {
    for (auto& star : stars) {
        star.Draw();
    }
}