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
  

    // 3つの星を配置
    Vector3 positions[] = {
        {5.0f, 1.0f, 1.0f},
        {7.0f, 1.0f, 5.0f},
        {9.0f, 1.0f, 8.0f}
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
