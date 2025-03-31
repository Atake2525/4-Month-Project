#define NOMINMAX
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	// 追加したクラス
	delete collision;
}

void Player::Initialize(Object3d* object3d, Camera* camera, Input* input)
{
	camera_ = camera;

	object3d_ = object3d;

	// 追加したクラス

	collision = new PlayerCollision();
	collision->AddCollision(AABB{ {-12.0f, 0.0f, -50.0f}, {-12.0f, 10.0f, 50.0f} }, Vector3{ 1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, -24.0f}, {12.0f, 10.0f, -24.0f} }, Vector3{ 0.0f, 0.0f, 1.0f });
	collision->AddCollision(AABB{ {12.0f, 0.0f, -50.0f}, {12.0f, 10.0f, 50.0f} }, Vector3{ -1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, 24.0f}, {12.0f, 10.0f, 24.0f} }, Vector3{ 0.0f, 0.0f, -1.0f });

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
	modelTransform_.translate.z = 0.0f;

}

void Player::Update()
{

	Move();

	Jump();

	Rotate();

	object3d_->SetTransform(modelTransform_);
	object3d_->SetRotateInDegree(modelTransform_.rotate);
	object3d_->SetColor(modelColor_);
	object3d_->SetEnableLighting(modelEnableLighting_);
	object3d_->Update();
	// 衝突判定をするためのもの
	modelTransform_.translate += collision->UpdateCollision(object3d_->GetAABB());

	object3d_->SetTranslate(modelTransform_.translate);
	object3d_->Update();

	camera_->SetTranslate(cameraTransform_.translate);
	camera_->SetRotate(cameraTransform_.rotate);



}

void Player::Draw(Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightData, Microsoft::WRL::ComPtr<ID3D12Resource> pointLightData, Microsoft::WRL::ComPtr<ID3D12Resource> spotLightData)
{
	object3d_->Draw(directionalLightData, pointLightData, spotLightData);
}

Camera* Player::GetCamera()
{
	return camera_;
}

void Player::Move()
{
	velocity.x = 0.0f;
	velocity.z = 0.0f;
	const float speed = 0.5f;
	Vector3 offSet = { 0.0f,10.0f,-20.0f };

	if (input_->PushKey(DIK_W)) {
		velocity.z = speed;
	}
	if (input_->PushKey(DIK_S)) {
		velocity.z = -speed;
	}
	if (input_->PushKey(DIK_A)) {
		velocity.x = -speed;
	}
	if (input_->PushKey(DIK_D)) {

		velocity.x = speed;
	}

	velocity = Normalize(velocity);

	modelTransform_.translate += velocity * speed;

	offSet = TransformNormal(offSet,
		Multiply(Multiply(
			MakeRotateXMatrix(modelTransform_.rotate.x),
			MakeRotateYMatrix(modelTransform_.rotate.y)),
			MakeRotateZMatrix(modelTransform_.rotate.z)
		));

	cameraTransform_.translate = modelTransform_.translate + offSet;


}

void Player::Rotate()
{
	const float rotate = 0.05f;

	if (input_->PushKey(DIK_RIGHTARROW)) {
		modelTransform_.rotate.y += rotate;
	}

	if (input_->PushKey(DIK_LEFTARROW)) {
		modelTransform_.rotate.y -= rotate;
	}

}

void Player::Jump()
{

	if (onGround_) {
		if (input_->PushKey(DIK_SPACE)) {
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