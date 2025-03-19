#include "Goal.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"


//デストラクタ
Goal::~Goal() {
	delete goalModel_;
}

void Goal::Initialize(Vector3 position, DirectXBase* dxc)
{
	directX = dxc;
	goalPos_ = position;

	ModelManager::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->Initialize(directX);

	ModelBase::GetInstance()->Initialize(directX);
	//TextureManager::GetInstance()->Initialize(directX);

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "goal.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);


	// object3dの初期化
	goalModel_ = new Object3d();
	goalModel_->Initialize();
	goalModel_->SetModel("goal.obj");

	//位置を指定する
	goalModel_->SetTranslate(goalPos_);

}

void Goal::Update()
{
	goalModel_->Update();

	////ゴールするとtrueになる => アニメーションの処理追加
	//if (!goalFlag_) {
	//	if () { //playerと当たったら
	//		goalFlag_ = true;

	//	}
	//}

	// ImGuiウィンドウの中にチェックボックスを追加
	ImGui::Begin("Goal Window");
	ImGui::DragFloat3("Goal Position", &goalPos_.x, 0.01f); //ゴールの位置の確認
	ImGui::Checkbox("Goal Flag", &goalFlag_);
	ImGui::End();


}



void Goal::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource) {


	goalModel_->Draw(directionalLightResource, pointLightResource, spotLightResource);

}


//Goal* goal = new Goal(Vector3(0, 5, 0)); // ゴールの位置を設定
//
//// 描画処理
//goal->Draw(); ステージのとこに追加する