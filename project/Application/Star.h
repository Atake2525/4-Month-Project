#pragma once

#include "Object3d.h"
#include "AABB.h"

class Star {
public:
    // 初期化
    void Initialize(const Vector3& pos);

    // 更新
    void Update();

    // 描画
    void Draw();

    // 衝突判定（プレイヤーとの）
    bool OnCollision(Object3d* target);

    // Object3d 取得
    Object3d* GetObject3d() { return object3d_; }

    // 取得済みかどうか
    bool IsCollected() const { return isCollected_; }

    // 星を取得済みにする
    void Collect() { isCollected_ = true; }

private:
    Object3d* object3d_ = nullptr;
    AABB aabb_;

    bool isCollected_ = false; // 星を取得したかどうかのフラグ
};
