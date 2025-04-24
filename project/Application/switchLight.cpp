#include "switchLight.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

switchLight::~switchLight()
{
	delete switchModel;
}
void switchLight::Initialize(Vector3 position)
{
	switchPosition = position;
	input_ = Input::GetInstance();


	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "switchOff.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "switchOn.obj");


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
	//ImGui::Begin("Debug Window");
	//ImGui::Checkbox("Switch Flag", &switchFlag); // フラグの状態を表示＆変更
	//ImGui::End();
}

void switchLight::Draw()
{
	if (switchFlag) {
		switchModel->SetModel("switchOn.obj");
		switchModel->Draw();

	}
	if (!switchFlag) {
		switchModel->SetModel("switchOff.obj");
		switchModel->Draw();

	}
}
