#include "starResult.h"
//#include "Player.h"

// デストラクタ
starResult::~starResult() {
    for (Star* star : stars_) {
        delete star;
    }
    stars_.clear();
}

void starResult::Initialize() { //Vector3 position,
    //directX_ = dxc;
    //starPos_ = position;

    // 3つの星を配置
    Vector3 positions[] = {
        {1.0f, 1.0f, 1.0f},
        {3.0f, 1.0f, 5.0f},
        {5.0f, 1.0f, 8.0f}
    };

    for (const auto& pos : positions) {
        Star* newStar = new Star();
        Transform starTransform;
        starTransform.translate = pos;
        starTransform.rotate = Vector3(0.0f, 0.0f, 0.0f); // 回転の初期値
        starTransform.scale = Vector3(1.0f, 1.0f, 1.0f); // スケールの初期値
        newStar->Initialize(starTransform);
        stars_.push_back(newStar);
    }


   /* for (int i = 0; i < 3; i++) {
        Star* newStar = new Star();
        newStar->Initialize(positions[i], directX_);
        stars_.push_back(newStar);
    }*/
}

void starResult::Update() {

    for (Star* star : stars_) {
        star->Update();
    }

    //for (Star* star : stars_) {
    //    star->Update();
    //    if (!star->IsCollected()) {
    //        star->OnCollision(nullptr); // 衝突処理
    //        if (star->IsCollected()) {
    //            score_ += 10; // スコア加算
    //        }
    //    }
    //}

}

void starResult::Draw() {
    for (Star* star : stars_) {
        star->Draw();
    }

}
