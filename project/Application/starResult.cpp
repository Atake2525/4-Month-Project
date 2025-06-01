#include "starResult.h"
//#include "Player.h"

// デストラクタ
starResult::~starResult() {
    for (Star* star : stars_) {
        delete star;
    }
    stars_.clear();
}

void starResult::Initialize(int stage) { 

    Vector3 positions[] = {
        {0,0,0},
        {0,0,0},
        {0,0,0},
    };

    //ステージごとの星の位置
    Vector3 stage_[] = {
        {5.0f, 1.0f, 1.0f},
        {7.0f, 1.0f, 5.0f},
        {9.0f, 1.0f, 8.0f}
    };
    Vector3 stage2_[] = {
        {6.0f, 3.0f, -9.0f},
        {-8.0f, 3.0f, -9.0f},
        {3.0f, 6.0f, -7.0f}
    };
    Vector3 stage3_[] = {
        {-9.0f, 8.0f, 6.0f},
        {0.0f, 17.0f, 0.0f},
        {-12.0f, 17.0f, -13.5f}
    };
    Vector3 stage4_[] = {
        {0.0f, 1.0f, 4.0f},
        {1.0f, 1.0f, 9.0f},
        {1.0f, 1.0f, 2.0f}
    };
    //チュートリアル
    Vector3 stage5_[] = {
       {0.0f, 1.0f, 4.0f},
       {1.0f, 1.0f, 9.0f},
       {1.0f, 1.0f, 2.0f}
    };

    // 3つの星を配置
    if (stage == 1) {
        for (int i = 0; i < 3; i++) {
            positions[i] = stage_[i];
        }
    }
    if (stage == 2) {
        for (int i = 0; i < 3; i++) {
            positions[i] = stage2_[i];
        }
    }
    if (stage == 3) {
        for (int i = 0; i < 3; i++) {
            positions[i] = stage3_[i];
        }
    }
    if (stage == 4) {
        for (int i = 0; i < 3; i++) {
            positions[i] = stage4_[i];
        }
    }
    //チュートリアル
    if (stage == 5) {
        for (int i = 0; i < 3; i++) {
            positions[i] = stage4_[i];
        }
    }

    for (const auto& pos : positions) {
        Star* newStar = new Star();
        Transform starTransform;
        starTransform.translate = pos;
        starTransform.rotate = Vector3(0.0f, 0.0f, 0.0f); // 回転の初期値
        starTransform.scale = Vector3(1.0f, 1.0f, 1.0f); // スケールの初期値
        newStar->Initialize(starTransform.translate);

        stars_.push_back(newStar);
    }


  
}

void starResult::Update() {

    for (Star* star : stars_) {
        star->Update();
    }
  

}

void starResult::Draw() {
    for (Star* star : stars_) {
        star->Draw();
    }

}
