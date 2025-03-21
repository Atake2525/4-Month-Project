#include "switchLight.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

switchLight::~switchLight()
{
	delete switchModel;
}
void switchLight::Initialize(Vector3 position, DirectXBase* dxc, Input* input)
{
	directX = dxc;
	switchPosition = position;
	input_ = input;

	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "box.obj", true);


	// object3dの初期化(KamataEngineで言うところのModel)
	switchModel = new Object3d();
	switchModel->Initialize();

	//位置を指定する
	switchModel->SetTranslate(switchPosition);
}

void switchLight::Update()
{
	//falseの時におしたらtrueになる
	if (!switchFlag) {
		if (input_->TriggerKey(DIK_1)) {
			switchFlag = true;

		}
	}
	else {

		if (input_->TriggerKey(DIK_1)) {
			switchFlag = false;

		}
	}
	switchModel->Update();
	// ImGuiウィンドウの中にチェックボックスを追加
	ImGui::Begin("Debug Window");
	ImGui::Checkbox("Switch Flag", &switchFlag); // フラグの状態を表示＆変更
	ImGui::End();
}

void switchLight::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	if (switchFlag) {
		switchModel->SetModel("box.obj");
		switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
	if (!switchFlag) {
		switchModel->SetModel("axis.obj");
		switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
}
