#include "Star.h"
#include <cmath>

Star::Star(Vector3 position) : position(position), IsCheck(false) {}

Star::~Star()
{
}

void Star::Initialize(Vector3, DirectXBase*)
{



}

void Star::Update() {
    // 必要ならアニメーションなどを追加
}

void Star::Draw() {
    if (!IsCheck) {
        // 星の描画処理 (3Dモデルなど)
    }
}

void Star::CheckCollision(Vector3 playerPos, float radius) {
    float distance = std::sqrt(
        (position.x - playerPos.x) * (position.x - playerPos.x) +
        (position.y - playerPos.y) * (position.y - playerPos.y) +
        (position.z - playerPos.z) * (position.z - playerPos.z)
    );

    if (distance < radius) {
        IsCheck = true; // 取得
    }
}
