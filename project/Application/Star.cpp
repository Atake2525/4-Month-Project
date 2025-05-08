#include "Star.h"

void Star::Initialize(const Vector3& pos) {
    object3d_ = new Object3d();
    object3d_->Initialize();
    object3d_->SetModel("Resources/Model/obj", "starResult.obj");  // 使用するモデル名に応じて変更してください
    Transform t;
    t.translate = pos;
    t.scale = Vector3(0.5f, 0.5f, 0.5f); // 必要に応じてスケール調整
    t.rotate = Vector3(0.0f, 0.0f, 0.0f);
    object3d_->SetTransform(t);
    aabb_ = object3d_->GetAABB();

    isCollected_ = false;
}

void Star::Update() {
    if (isCollected_) {
        return; // 取得済みなら更新しない
    }

    object3d_->Update();

    // 星をくるくる回転させたい場合などに
    Transform t = object3d_->GetTransform();
    t.rotate.y += 0.05f;
    object3d_->SetTransform(t);

    aabb_ = object3d_->GetAABB();
}

void Star::Draw() {
    if (isCollected_) {
        return; // 取得済みなら描画しない
    }

    object3d_->Draw();
}

bool Star::OnCollision(Object3d* target) {
    if (isCollected_) {
        return false;
    }

    AABB targetAABB = target->GetAABB();
    bool isHit =
        aabb_.min.x <= targetAABB.max.x && aabb_.max.x >= targetAABB.min.x &&
        aabb_.min.y <= targetAABB.max.y && aabb_.max.y >= targetAABB.min.y &&
        aabb_.min.z <= targetAABB.max.z && aabb_.max.z >= targetAABB.min.z;

    if (isHit) {
        isCollected_ = true;
    }

    return isHit;
}
