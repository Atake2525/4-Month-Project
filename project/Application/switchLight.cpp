#include "switchLight.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

switchLight::~switchLight()
{
	delete switchModel;
}
void switchLight::Initialize(Vector3 position, Camera* camera, DirectXBase* dxc, Input* input)
{
	directX = dxc;
	switchCamera = camera;
	switchPosition = position;
	input_ = input;

	


	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);


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
			light2BlockHp = light2BlockHp - 1.0f;
			light3BlockHp= light3BlockHp - 1.0f;
			if (light2BlockHp == 0) {
				light2BlockFlag = true;
			}

			if (light3BlockHp == 0) {
				light3BlockFlag = true;
			}
		}
	}
	else {

		if (input_->TriggerKey(DIK_1)) {
			switchFlag = false;
			if (light2BlockHp == 0) {
				light2BlockHp = 2;
				light2BlockFlag = false;
			}
			if (light3BlockHp == 0) {
				light3BlockHp = 3;
				light3BlockFlag = false;
			}
		}
	}

	// ImGuiウィンドウの中にチェックボックスを追加
	ImGui::Begin("Resizable Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Switch Flag", &switchFlag); // フラグの状態を表示＆変更
	ImGui::Checkbox("Switch2 Flag", &light2BlockFlag); // フラグの状態を表示＆変更
	ImGui::Checkbox("Switch3 Flag", &light3BlockFlag); // フラグの状態を表示＆変更
	ImGui::End();
}

void switchLight::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	if (switchFlag) {
		switchModel->SetModel("box.obj");
		//switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
	if(!switchFlag){
		switchModel->SetModel("axis.obj");
		//switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
}

