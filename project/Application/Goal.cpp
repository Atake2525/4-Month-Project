#pragma once
#include "Goal.h"
#include<iostream>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
//#include "externels/imgui/imgui_implwin32.h"


//デストラクタ
Goal::~Goal() {
	delete goalModel;
	delete clearSprite_;

}
void Goal::Initialize(Vector3 position)
{
	//directX = dxc;
	goalPos = position;

	//モデル読み込み
	//ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "goal.obj");
	//ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "box.obj", true);


	// object3dの初期化
	goalModel = new Object3d();
	goalModel->Initialize();
	goalModel->SetModel("goal.obj");
	goalModel->SetTranslate(goalPos);//位置を指定する

	//// クリアスプライトの初期化
	//clearSprite = new Sprite();
	//clearSprite->Initialize("Resources/Texture/clear.png");
	//clearSprite->SetPosition({ 320, 180 });  // 画面中央

}

void Goal::Update()

{
	goalModel->Update();

	////ゴールすると trueになる => アニメーションの処理追加
	//if (!goalFlag) {
	//    if () {            //playerと当たったら
	//        goalFlag = true;
	//    }            // ゴールフラグが trueならクリアスプライトを描画
	//    if (goalFlag) {
	//        clearSprite->Draw();
	//    }
	//}

	// ImGuiウィンドウの中にチェックボックスを追加
	ImGui::Begin("Goal Window");
	ImGui::DragFloat3("Goal Position", &goalPos.x, 0.01f); //ゴールの位置の確認
	ImGui::Checkbox("Goal Flag", &goalFlag);
	ImGui::End();


}



void Goal::Draw() {

	goalModel->Draw();

	//// ゴールフラグが trueならクリアスプライトを描画
	//if (goalFlag) {
	//    clearSprite->Draw();
	//}

}


bool Goal::OnCollision(Object3d* object3d) {

	if (goalModel->CheckCollision(object3d)) {



		return true;
	}
	return false;

	//goalFlag_ = true;  // プレイヤーと当たったらゴールフラグを trueにする

}