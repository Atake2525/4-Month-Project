#include "Goal.h"
#include <iostream>
#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

// デストラクタ
Goal::~Goal() {
	delete goalModel_;     // ゴールオブジェクトのメモリ解放
	delete clearSprite_;   // クリアスプライトのメモリ解放
}

// 初期化処理
// @param position: ゴールの位置
// @param dxc: DirectXの基盤クラス
void Goal::Initialize(Vector3 position, DirectXBase* dxc) {
	directX = dxc;
	goalPos_ = position;

	// 各マネージャーの初期化
	ModelManager::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->Initialize(directX);
	ModelBase::GetInstance()->Initialize(directX);

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "goal.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	// Object3dの初期化
	goalModel_ = new Object3d();
	goalModel_->Initialize();
	goalModel_->SetModel("goal.obj");
	goalModel_->SetTranslate(goalPos_); // ゴール位置を設定

	// クリアスプライトの初期化（コメントアウトされているが後で使用する可能性あり）
	// clearSprite_ = new Sprite();
	// clearSprite_->Initialize("Resources/Texture/clear.png");
	// clearSprite_->SetPosition({ 320, 180 });  // 画面中央に配置
}

// 更新処理
void Goal::Update() {
	goalModel_->Update(); // ゴールオブジェクトの更新

	// ImGuiを用いたデバッグ用UI
	ImGui::Begin("Goal Window");
	ImGui::DragFloat3("Goal Position", &goalPos_.x, 0.01f); // ゴールの位置確認
	ImGui::Checkbox("Goal Flag", &goalFlag_); // ゴールフラグの確認
	ImGui::End();
}

// 描画処理
// @param directionalLightResource: 平行光源のリソース
// @param pointLightResource: 点光源のリソース
// @param spotLightResource: スポットライトのリソース
void Goal::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource,
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource,
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource) {

	goalModel_->Draw(directionalLightResource, pointLightResource, spotLightResource); // ゴールオブジェクトの描画

	// ゴールフラグがtrueならクリアスプライトを描画（現在はコメントアウト）
	// if (goalFlag_) {
	//	clearSprite_->Draw();
	// }
}

// プレイヤーとの当たり判定
// @param player: 衝突を確認するプレイヤーオブジェクト
void Goal::OnCollision(const Player* player) {
	(void)player; // 未使用警告を回避するためのキャスト
	// goalFlag_ = true;  // プレイヤーと当たったらゴールフラグをtrueにする（現在コメントアウト）
}
