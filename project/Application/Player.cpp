#define NOMINMAX
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	// 追加したクラス
	delete collision;
	delete object3d_;
	delete camera_;
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

	input_->GetInstance();

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
	const float speed = 0.5f;
	Vector3 offSet = { 0.0f,10.0f,-20.0f };

	move = input_->GetLeftJoyStickPos2();
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
	if (input_->IsMoveLeftJoyStick()) {
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
	velocity = TransformNormal(velocity, camera_->GetWorldMatrix());
	velocity.y = 0;

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
	Vector3 move{ 0,0 };
	move = input_->GetRightJoyStickPos3();
	if (move.x >= 0.05f) {
		move.x = 0.05f;
	}
	if (move.x <= -0.05f) {
		move.x = -0.05f;
	}
	if (move.x == 0.0f) {
		if (input_->PushKey(DIK_RIGHTARROW)) {
			modelTransform_.rotate.y += rotate;
			cameraTransform_.rotate.y += rotate;
		}

		if (input_->PushKey(DIK_LEFTARROW)) {
			modelTransform_.rotate.y -= rotate;
			cameraTransform_.rotate.y -= rotate;
		}
	}
	modelTransform_.rotate.y += move.x;
	cameraTransform_.rotate.y += move.x;
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