#include "PlayerS.h"

PlayerS::PlayerS()
{

}

PlayerS::~PlayerS()
{

}

void PlayerS::Initialize(Object3d* object3d, Camera* camera, Input* input)
{
	camera_ = camera;

	object3d_ = object3d;

	input_ = new Input();
	input_ = input;

	//camera
	cameraTransform_.translate = camera->GetTranslate();
	cameraTransform_.rotate = camera->GetRotate();
	// Model
	modelTransform_ = object3d_->GetTransform();
	modelTransform_.rotate = object3d_->GetRotateInDegree();
	modelColor_ = object3d_->GetColor();
	modelEnableLighting_ = object3d_->GetEnableLighting();
	shininess_ = object3d_->GetShininess();

}

void PlayerS::Update()
{
	//modelTransform_.translate += {0.0f, 0.0f, 1.0f};


	const float speed = 0.5f;
	Vector3 velocity(0.0f, 0.0f, speed);
	if (input_->PushKey(DIK_W)) {
		modelTransform_.translate += velocity;
		cameraTransform_.translate += velocity;
	}
	if (input_->PushKey(DIK_S)) {
		modelTransform_.translate -= velocity;
		cameraTransform_.translate -= velocity;
	}

	velocity = { speed, 0.0f, 0.0f };

	if (input_->PushKey(DIK_A)) {
		modelTransform_.translate -= velocity;
		cameraTransform_.translate -= velocity;
	}
	if (input_->PushKey(DIK_D)) {
		modelTransform_.translate += velocity;
		cameraTransform_.translate += velocity;
	}

	/*if (input_->PushKey(DIK_SPACE)) {
		modelTransform_.
	}*/


	object3d_->SetTransform(modelTransform_);
	object3d_->SetRotateInDegree(modelTransform_.rotate);
	object3d_->SetColor(modelColor_);
	object3d_->SetEnableLighting(modelEnableLighting_);
	object3d_->Update();

	camera_->SetTranslate(cameraTransform_.translate);
	camera_->SetRotate(cameraTransform_.rotate);
}

void PlayerS::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightData, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightData, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightData)
{
	object3d_->Draw(directionalLightData, pointLightData, spotLightData);
}

Camera* PlayerS::GetCamera()
{
	return camera_;
}


