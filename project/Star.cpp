#include "Star.h"
//#include "Player.h"
#include "externels/imgui/imgui.h"

// デストラクタ
Star::~Star() {
    delete starModel_;
}

void Star::Initialize(Vector3 position, DirectXBase* dxc) {
    directX_ = dxc;
    starPos_ = position;

    ModelManager::GetInstance()->Initialize(directX_);

    // モデル読み込み
    ModelManager::GetInstance()->LoadModel("Resources/Model", "star.obj");

    // Object3dの初期化
    starModel_ = new Object3d();
    starModel_->Initialize();
    starModel_->SetModel("star.obj");
    starModel_->SetTranslate(starPos_);
}

void Star::Update() {
    starModel_->Update();

    // ImGuiで位置確認
    ImGui::Begin("Star Debug");
    ImGui::DragFloat3("Star Position", &starPos_.x, 0.01f);
    ImGui::Checkbox("Collected", &collected_);
    ImGui::End();
}

void Star::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource,
    Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource,
    Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource) {
    if (!collected_) {
        starModel_->Draw(directionalLightResource, pointLightResource, spotLightResource);
    }
}

void Star::OnCollision(const Player* player) {
    (void)player;  // 未使用警告を回避
    collected_ = true;  // プレイヤーが取得
}
