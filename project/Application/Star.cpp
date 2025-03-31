#include "Star.h"
//#include "Player.h"
#include "externels/imgui/imgui.h"

// デストラクタ
Star::~Star() {
	delete starModel_;
}

void Star::Initialize(const Transform& translate, DirectXBase* dxc) {
	
	this->transform_ = translate;
	this->directX_ = dxc;

	ModelManager::GetInstance()->Initialize(directX_);

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

	// ImGuiで位置確認
	ImGui::Begin("Star Debug");
	ImGui::DragFloat3("Star Position", &transform_.translate.x, 0.01f);
	ImGui::Checkbox("Collected", &collected_);
	ImGui::End();
}

void Star::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource) {
	if (!collected_) {
		starModel_->Draw();
	}

}

void Star::OnCollision(const Player* player) {
	(void)player;  // 未使用警告を回避
	collected_ = true;  // プレイヤーが取得
}
