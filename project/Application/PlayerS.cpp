#include "PlayerS.h"

PlayerS::PlayerS()
{

}

PlayerS::~PlayerS()
{

}

void PlayerS::Initialize(Object3d* object3d, Camera* camera, WinApp* winApp)
{
	camera_ = camera;

	object3d_ = object3d;

	input_ = new Input();
	input_->Initialize(winApp);

	// Model
	modelTransform_ = object3d_->GetTransform();
	modelTransform_.rotate = object3d_->GetRotateInDegree();
	modelColor_ = object3d_->GetColor();
	modelEnableLighting_ = object3d_->GetEnableLighting();
	shininess_ = object3d_->GetShininess();

}

void PlayerS::Update()
{
	if (input_->PushKey(DIK_LEFT)) {
		modelTransform_.translate.y -= 0.03f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		modelTransform_.translate.y += 0.03f;
	}
	if (input_->PushKey(DIK_UP)) {
		modelTransform_.translate.x -= 0.03f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		modelTransform_.translate.x += 0.03f;
	}

	object3d_->SetTransform(modelTransform_);
	object3d_->SetRotateInDegree(modelTransform_.rotate);
	object3d_->SetColor(modelColor_);
	object3d_->SetEnableLighting(modelEnableLighting_);
	object3d_->Update();
}

void PlayerS::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightData, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightData, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightData)
{
	object3d_->Draw(directionalLightData, pointLightData, spotLightData);
}
