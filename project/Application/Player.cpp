#define NOMINMAX
#include "Player.h"

#include <iostream>
#include <algorithm>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

Player::Player()
{

}

Player::~Player()
{
	delete object3d_;

	// 追加したクラス
	delete collision;
}

void Player::Initialize(Camera* camera)
{
	camera_ = camera;

	// 追加したクラス

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "Player.obj");

	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");

	collision = new PlayerCollision();
	collision->AddCollision(AABB{ {-12.0f, 0.0f, -50.0f}, {-12.0f, 10.0f, 50.0f} }, Vector3{ 1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, -24.0f}, {12.0f, 10.0f, -24.0f} }, Vector3{ 0.0f, 0.0f, 1.0f });
	collision->AddCollision(AABB{ {12.0f, 0.0f, -50.0f}, {12.0f, 10.0f, 50.0f} }, Vector3{ -1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, 24.0f}, {12.0f, 10.0f, 24.0f} }, Vector3{ 0.0f, 0.0f, -1.0f });

	input_ = Input::GetInstance();

	//camera
	cameraTransform_.translate = camera->GetTranslate();
	cameraTransform_.rotate = camera->GetRotate();
	// Model
	modelTransform_ = object3d_->GetTransform();
	modelTransform_.rotate = object3d_->GetRotateInDegree();
	modelColor_ = object3d_->GetColor();
	modelEnableLighting_ = object3d_->GetEnableLighting();
	shininess_ = object3d_->GetShininess();
	modelTransform_.translate.z = 0.0f;

	drawModel = object3d_->GetTransform();
	drawModel.rotate = object3d_->GetRotateInDegree();
}

void Player::Update()
{
	ImGui::Begin("State");
	if (ImGui::TreeNode("PlayerCamera")) {
		ImGui::DragFloat3("Tranlate", &cameraTransform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &cameraTransform_.rotate.x, 0.1f);
		ImGui::DragFloat3("Scale", &cameraTransform_.scale.x, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("PlayerModel")) {
		ImGui::DragFloat3("Tranlate", &modelTransform_.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &modelTransform_.rotate.x, 0.1f);
		ImGui::DragFloat3("Scale", &modelTransform_.scale.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::End();

	Move();

	Rotate();

	Jump();

	drawModel.translate = modelTransform_.translate;

	camera_->SetTranslate(cameraTransform_.translate);
	camera_->SetRotate(cameraTransform_.rotate);

	object3d_->SetTransform(drawModel);
	object3d_->SetRotateInDegree(drawModel.rotate);
	object3d_->SetColor(modelColor_);
	object3d_->SetEnableLighting(modelEnableLighting_);
	object3d_->Update();
	// 衝突判定をするためのもの
	modelTransform_.translate += collision->UpdateCollision(object3d_->GetAABB());

	drawModel.translate = modelTransform_.translate;

	object3d_->SetTranslate(drawModel.translate);
	object3d_->Update();

}

void Player::Draw()
{
	object3d_->Draw();
}

Camera* Player::GetCamera()
{
	return camera_;
}

void Player::Move()
{

	Vector2 move{0,0};
	velocity.x = 0.0f;
	velocity.z = 0.0f;
	const float speed = 0.3f;

	if (input_->IsMoveLeftJoyStick()) {
		move = input_->GetLeftJoyStickPos2();
	}
	if (move.x >= 0.5f) {
		move.x = 0.5f;
	}
	else if (move.x <= -0.5f) {
		move.x = -0.5f;
	}

	if (move.y >= 0.5f) {
		move.y = 0.5f;
	}
	else if (move.y <= -0.5f) {
		move.y = -0.5f;
	}
	if (input_->IsMoveLeftJoyStick() == false) {
		if (input_->PushKey(DIK_W)) {
			move.y = -speed;
		}
		if (input_->PushKey(DIK_S)) {
			move.y = speed;
		}
		if (input_->PushKey(DIK_A)) {
			move.x = -speed;
		}
		if (input_->PushKey(DIK_D)) {
			move.x = speed;
		}
	}
	velocity.z = -move.y;
	velocity.x = move.x;
	velocity = Normalize(velocity);
	velocity.y = 0;

	modelTransform_.translate += velocity * speed;


}

void Player::Rotate()
{
	Vector3 offSet = { 0.0f,10.0f,-20.0f };
	
	if (input_->IsMoveRightJoyStick()) {
		modelTransform_.rotate.y += std::clamp(input_->GetRightJoyStickPos3().x, -0.05f, 0.05f);
		modelTransform_.rotate.x += std::clamp(input_->GetRightJoyStickPos3().y, -0.05f, 0.05f);
		cameraTransform_.rotate.y += std::clamp(input_->GetRightJoyStickPos3().x, -0.05f, 0.05f);
		cameraTransform_.rotate.x += std::clamp(input_->GetRightJoyStickPos3().y, -0.05f, 0.05f);
	}
	else {
		modelTransform_.rotate.y += input_->GetMouseVel3().x * 0.005;
		modelTransform_.rotate.x += input_->GetMouseVel3().y * 0.005;
		cameraTransform_.rotate.y += input_->GetMouseVel3().x * 0.005;
		cameraTransform_.rotate.x += input_->GetMouseVel3().y * 0.005;
	}
	
	offSet = TransformNormal(offSet,
		Multiply(Multiply(
			MakeRotateXMatrix(modelTransform_.rotate.x),
			MakeRotateYMatrix(modelTransform_.rotate.y)),
			MakeRotateZMatrix(modelTransform_.rotate.z)
		));

	cameraTransform_.translate = modelTransform_.translate + offSet;

	//cameraTransform_.rotate.x = std::clamp(cameraTransform_.rotate.x, -0.1f, 0.9f);
	//cameraTransform_.translate.y = std::clamp(cameraTransform_.translate.y, 0.2f, 16.0f);
	//-0.1f 0.9f cameraRotate
	//0.2f 16.0f cameraTransfrom
}

void Player::Jump()
{

	if (onGround_) {
		if (input_->PushKey(DIK_SPACE) || input_->PushButton(Button::A)) {
			JumpVelocity += kJumpAcceleration / 60.0f;
			onGround_ = false;
		}

	}
	else if (onGround_ == false)
	{
		JumpVelocity -= kGravityAccleration / 60.0f;
		JumpVelocity = std::max(JumpVelocity, -kLimitFallSpeed);
	}

	modelTransform_.translate.y += JumpVelocity;

	if (modelTransform_.translate.y <= 1.0f) {
		modelTransform_.translate.y = 1.0f;
		onGround_ = true;
	}

}


// 衝突判定の実装で追加したもの
const Vector3& Player::GetPosition() const {
	Vector3 result;
	result.x = object3d_->GetWorldMatrix().m[3][0];
	result.y = object3d_->GetWorldMatrix().m[3][1];
	result.z = object3d_->GetWorldMatrix().m[3][2];
	return result;
}