#include "Star.h"
//#include "Player.h"
#include "externels/imgui/imgui.h"

// デストラクタ
Star::~Star() {
	delete starModel_;
}

void Star::Initialize(const Transform& translate) {
	
	this->transform_ = translate;

	// モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "starResult.obj");

	// Object3dの初期化
	starModel_ = new Object3d();
	starModel_->Initialize();
	starModel_->SetModel("starResult.obj");
	starModel_->SetTranslate(transform_.translate);

}

void Star::Update() {

	starModel_->Update();

	// 星のY軸回転
	transform_.rotate.y += 0.01f;
	starModel_->SetTransform(transform_);

	
}

void Star::Draw() {
	if (!collected_) {
		starModel_->Draw();
	}

}

bool Star::OnCollision(Object3d* object3d) {

	if (starModel_->CheckCollision(object3d)) {

		return true;
	}
	return false;
}
