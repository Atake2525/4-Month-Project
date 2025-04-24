#include "switchLight.h"
#include<iostream>
#include "Light.h"

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

#include"Player.h"

switchLight::~switchLight()
{
	if (switchModel)
	{
		delete switchModel;
	}
}


void switchLight::Initialize(Transform transform/*, Camera* camera, DirectXBase* dxc*/, Input* input, Player* player)
{
	switchTransform = transform;
	input_ = input;
	player_ = player;




	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "switchOff.obj", true);
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "switchOn.obj", true);


	// object3dの初期化(KamataEngineで言うところのModel)
	switchModel = new Object3d();
	switchModel->Initialize();
	switchModel->SetModel("Grid.obj");
	//位置を指定する
	switchModel->SetTranslate(switchTransform.translate);
}

void switchLight::Update()
{
	//if (IsCollisionAABB(player_->GetAABB(), GetAAbb())) {
		//falseの時におしたらtrueになる
	if (!switchFlag) {
		if (input_->TriggerKey(DIK_1)) {
			switchFlag = true;
			Light::GetInstance()->SetColorDirectionalLight({ 0.0f, 0.2f, 1.0f, 1.0f });
		}
	}
	else {

		if (input_->TriggerKey(DIK_1)) {
			switchFlag = false;
			Light::GetInstance()->SetColorDirectionalLight({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		//}
	}

	switchModel->Update();

	// ImGuiウィンドウの中にチェックボックスを追加
	//ImGui::Begin("Debug Window");
	//ImGui::Checkbox("Switch Flag", &switchFlag); // フラグの状態を表示＆変更
	//ImGui::End();
}


const bool& switchLight::IsCollisionAABB(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
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
//

AABB switchLight::GetAAbb()
{

	Vector3 worldPos = switchTransform.translate;
	AABB aabb;

	aabb.min = { worldPos.x - radius, worldPos.y - radius, worldPos.z - radius };
	aabb.max = { worldPos.x + radius, worldPos.y + radius, worldPos.z + radius };

	return aabb;

}