#include "Goal.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"


Goal::~Goal() {
	delete goalModel_;
}

void Goal::Initialize(Vector3 position, Camera* camera, DirectXBase* dxc)
{
	directX = dxc;
	goalCamera_ = camera;
	goalPos_ = position;

	ModelManager::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->SetDefaultCamera(goalCamera_);
	ModelBase::GetInstance()->Initialize(directX);
	//TextureManager::GetInstance()->Initialize(directX);

	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	// object3dの初期化(KamataEngineで言うところのModel)
	goalModel_ = new Object3d();
	goalModel_->Initialize();

	//位置を指定する
	goalModel_->SetTranslate(goalPos_);

}

void Goal::Update()
{

	//ゴールするとtrueになる => アニメーションの処理追加
	if (!goalFlag_) {
		if () { //playerと当たったら
			goalFlag_ = true;

		}
	}

	// ImGuiウィンドウの中にチェックボックスを追加
	ImGui::Begin("Goal Window");
	ImGui::Checkbox("Goal Flag", &goalFlag_); // フラグの状態を表示＆変更
	ImGui::End();


}

ModelManager::GetInstance()->LoadModel("Resources/Model", "goal.obj");



void Goal::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource) {
	if (goalFlag_) {
		goalModel_->SetModel("box.obj");
		goalModel_->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
	else {
		goalModel_->SetModel("axis.obj");
		goalModel_->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}

}


//Goal* goal = new Goal(Vector3(0, 5, 0)); // ゴールの位置を設定
//
//// 描画処理
//goal->Draw(); ステージのとこに追加する