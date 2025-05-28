#define NOMINMAX
#include "Player.h"

#include <iostream>
#include <algorithm>
#include<random>


#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

std::random_device seedGenerator;
std::mt19937 randomEngine(seedGenerator());
std::uniform_real_distribution<float> distrubution(0.0f, 3.0f);


Player::Player()
{

}

Player::~Player()
{
	delete object3d_;

	// 追加したクラス
	delete collision;

	delete lightCollision;
}

void Player::AddStageCollision(const std::string& directoryPath, const std::string& filename) {
	collision->AddCollision(directoryPath, filename);
}

void Player::ClearStageCollision() {
	collision->ClearCollisionList();
}

void Player::AddLightBlockCollision(const std::string& directoryPath, const std::string& filename) {
	lightCollision->AddCollision(directoryPath, filename);
}

void Player::ClearLightBlockCollision() {
	lightCollision->ClearCollisionList();
}

void Player::Initialize(Camera* camera)
{
	camera_ = camera;
	//camera->SetTranslate({ 0.0f, 10.0f, -20.0f });
	cameraTransform_ = camera->GetTransform();
	cameraTransform_.translate = cameraOffset;
	cameraOffset = defaultCameraOffset;

	// 追加したクラス

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Player", "Player.obj");

	//TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	object3d_ = new Object3d();
	object3d_->Initialize();
	object3d_->SetModel("Player.obj");
	object3d_->Update();
	firstCameraAABB = object3d_->GetAABB();
	cameraAABB = object3d_->GetAABB();

	collision = new PlayerCollision();

	lightCollision = new PlayerCollision();
	/*collision->AddCollision(AABB{ {-12.0f, 0.0f, -50.0f}, {-12.0f, 10.0f, 50.0f} }, Vector3{ 1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, -24.0f}, {12.0f, 10.0f, -24.0f} }, Vector3{ 0.0f, 0.0f, 1.0f });
	collision->AddCollision(AABB{ {12.0f, 0.0f, -50.0f}, {12.0f, 10.0f, 50.0f} }, Vector3{ -1.0f, 0.0f, 0.0f });
	collision->AddCollision(AABB{ {-12.0f, 0.0f, 24.0f}, {12.0f, 10.0f, 24.0f} }, Vector3{ 0.0f, 0.0f, -1.0f });*/

	input_ = Input::GetInstance();

	//camera
	//cameraTransform_.translate = camera->GetTranslate();
	//cameraTransform_.rotate = camera->GetRotate();
	// Model
	modelTransform_ = object3d_->GetTransform();
	modelTransform_.rotate = object3d_->GetRotateInDegree();
	modelColor_ = object3d_->GetColor();
	modelEnableLighting_ = object3d_->GetEnableLighting();
	shininess_ = object3d_->GetShininess();
	/*modelTransform_.translate.z = -5.0f;
	modelTransform_.translate.x = 10.0f;*/
	modelTransform_.translate.y = 1.0f;

	drawModel = object3d_->GetTransform();
	drawModel.rotate = object3d_->GetRotateInDegree();

	onGround_ = true;
}

void Player::Update()
{

	Rotate();

	Move();

	Jump();

	object3d_->SetTranslate(modelTransform_.translate);
	object3d_->Update();
	drawModel.translate = modelTransform_.translate;


	UpdateStageCollision();

	UpdateLightCollision();

	//ImGui::Begin("onGround");
	//ImGui::Checkbox("onGround", &onGround_);
	//ImGui::End();

	//camera_->SetTranslate(cameraTransform_.translate);
	//camera_->SetRotate(cameraTransform_.rotate);

	/*object3d_->SetTransform(drawModel);
	object3d_->SetRotateInDegree(drawModel.rotate);
	object3d_->SetColor(modelColor_);
	object3d_->SetEnableLighting(modelEnableLighting_);
	object3d_->Update();*/


	drawModel.translate = modelTransform_.translate;
	drawModel.rotate.y = prot;

	object3d_->SetRotate({ 0.0f, drawModel.rotate.y, 0.0f });

	object3d_->SetTranslate(drawModel.translate);

	//object3d_->SetTransform(drawModel);
	object3d_->Update();


	

	//camera_->SetTranslate(cameraTransform_.translate);

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
	if (ImGui::TreeNode("PlayerDrawModel")) {
		ImGui::DragFloat3("Tranlate", &drawModel.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &drawModel.rotate.x, 0.1f);
		ImGui::DragFloat3("Scale", &drawModel.scale.x, 0.1f);
		ImGui::TreePop();
	}
	ImGui::DragFloat3("cameraVelocity", &cameraVelocity.x, 0.1f);
	ImGui::End();

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
	// コントローラー用
	plRotate = std::atan2(move.x, move.y);
	if (input_->IsMoveLeftJoyStick() == false) {
		if (input_->PushKey(DIK_W)) {
			move.y = -speed;
			plRotate = 0.0f;
		}
		if (input_->PushKey(DIK_S)) {
			move.y = speed;
			plRotate = std::numbers::pi_v<float>;
		}
		if (input_->PushKey(DIK_A)) {
			move.x = -speed;
			plRotate = std::numbers::pi_v<float> * 3.0f / 2.0f;
		}
		if (input_->PushKey(DIK_D)) {
			move.x = speed;
			plRotate = std::numbers::pi_v<float> / 2.0f;
		}
	}
	velocity.z = -move.y;
	velocity.x = move.x;


	// 即席の移動速度安定化
	Vector3 rot = modelTransform_.rotate;
	rot.x = 0.0f;
	Matrix4x4 worldMatrix = MakeAffineMatrix(modelTransform_.scale, rot, modelTransform_.translate);
	velocity = TransformNormal(velocity, worldMatrix);

	//plRotateDegree = SwapDegree(plRotateDegree);
	// プレイヤーの移動に応じてモデルを回転させる

	prot = plRotate + modelTransform_.rotate.y;

	float rotp;
	if (plRotate > std::numbers::pi_v<float>)
	{
		rotp = SwapDegree(std::fmod(-plRotate, std::numbers::pi_v<float>));
	}
	else
	{
		rotp = SwapDegree(std::fmod(plRotate, 2 * std::numbers::pi_v<float>));
	}
	float ro = SwapDegree(std::abs(drawModel.rotate.y) - std::abs(SwapRadian(rotp)));


	if (modelTransform_.rotate.y != prot + modelTransform_.rotate.y && !rotateEasing)
	{
		rotateEasing = true;
		rotateFrame = 0.0f;
		startRotate = drawModel.rotate.y;
		//endRotate = modelTransform_
	}
	if (rotateEasing)
	{
		rotateFrame += 1.0f / 60.0f / rotateEndFrame;
		//drawModel.rotate.y = easeInOut(rotateFrame, startRotate, startRotate + SwapRadian(rotp));
		if (rotateFrame > 1.0f)
		{
			rotateEasing = false;
			rotateFrame = 0.0f;
			rotp = 0.0f;
		}
	}

	ImGui::Begin("rotateDegree");
	ImGui::DragFloat("rotp", &rotp);
	ImGui::DragFloat("pRot", &ro);
	ImGui::End();


	modelTransform_.translate += velocity * speed;


}

void Player::Rotate()
{
	
	if (input_->IsMoveRightJoyStick()) {
		modelTransform_.rotate.y += std::clamp(input_->GetRightJoyStickPos3().x, -0.05f, 0.05f);
		modelTransform_.rotate.x += std::clamp(input_->GetRightJoyStickPos3().y, -0.05f, 0.05f);
		//cameraTransform_.rotate.y += std::clamp(input_->GetRightJoyStickPos3().x, -0.05f, 0.05f);
		//cameraTransform_.rotate.x += std::clamp(input_->GetRightJoyStickPos3().y, -0.05f, 0.05f);
	}
	else {
		modelTransform_.rotate.y += input_->GetMouseVel3().x * 0.005f;
		modelTransform_.rotate.x += input_->GetMouseVel3().y * 0.005f;
		//cameraTransform_.rotate.y += input_->GetMouseVel3().x * 0.005;
		//cameraTransform_.rotate.x += input_->GetMouseVel3().y * 0.005;
	}
	modelTransform_.rotate.x = std::clamp(modelTransform_.rotate.x, -0.45f, 0.75f);
	//cameraTransform_.rotate.x = std::clamp(cameraTransform_.rotate.x, -0.1f, 0.9f);
	//cameraTransform_.translate.y = std::clamp(cameraTransform_.translate.y, 0.2f, 16.0f);
	//-0.1f 0.9f cameraRotate
	//0.2f 16.0f cameraTransfrom



	Matrix4x4 worldMatrix = MakeAffineMatrix(modelTransform_.scale, modelTransform_.rotate, modelTransform_.translate);

	// AABBの更新
	cameraAABB.min = firstCameraAABB.min + cameraTransform_.translate;
	cameraAABB.max = firstCameraAABB.max + cameraTransform_.translate;

	defaultCameraAABB.min = firstCameraAABB.min + defaultCameraTransform.translate;
	defaultCameraAABB.max = firstCameraAABB.max + defaultCameraTransform.translate;

	// TransformNormalの計算でcameraTransformにカメラの位置を代入
	// プレイヤーのTransformが向いている方向からcameraOffset分を足す
	Vector3 normalizeCamera = TransformNormal(cameraOffset, worldMatrix);
	Vector3 normalizeDefaultCamera = TransformNormal(defaultCameraOffset, worldMatrix);
	// カメラをプレイヤーの位置に合わせる
	cameraTransform_.translate = normalizeCamera + modelTransform_.translate;
	defaultCameraTransform.translate = normalizeDefaultCamera + modelTransform_.translate;

	// 前回の座標と照合して移動量を計算  std::abs(絶対値)
	defaultCameraVelocity.x = std::abs(defaultCameraTransform.translate.x) - std::abs(defaultCameraVelocityPre.x);
	defaultCameraVelocity.y = std::abs(defaultCameraTransform.translate.y) - std::abs(defaultCameraVelocityPre.y);
	defaultCameraVelocity.z = std::abs(defaultCameraTransform.translate.z) - std::abs(defaultCameraVelocityPre.z);
	defaultCameraVelocity.x *= -1;
	defaultCameraVelocity.z *= -1;

	// 前回の座標と照合して移動量を計算  std::abs(絶対値)
	cameraVelocity.x = std::abs(cameraTransform_.translate.x) - std::abs(cameraVelocityPre.x);
	cameraVelocity.y = std::abs(cameraTransform_.translate.y) - std::abs(cameraVelocityPre.y);
	cameraVelocity.z = std::abs(cameraTransform_.translate.z) - std::abs(cameraVelocityPre.z);
	cameraVelocity.x *= -1;
	cameraVelocity.z *= -1;


	// カメラの衝突判定を更新
	UpdateCameraCollision();


	// 高度制限clamp
	cameraTransform_.translate.y = std::clamp(cameraTransform_.translate.y, 1.0f, modelTransform_.translate.y + 21.0f);

	// 高度制限clamp
	defaultCameraTransform.translate.y = std::clamp(defaultCameraTransform.translate.y, 1.0f, modelTransform_.translate.y + 21.0f);

	// カメラのworldMatrixを更新
	Matrix4x4 cameraMatrix = MakeAffineMatrix(modelTransform_.scale, modelTransform_.rotate, cameraTransform_.translate);
	// カメラのworldMatrixを更新
	Matrix4x4 defaultCameraMatrix = MakeAffineMatrix(modelTransform_.scale, modelTransform_.rotate, defaultCameraTransform.translate);

	// 高度制限が発生しているときは平行移動のみカメラとparentする
	if (cameraTransform_.translate.y >= 1.0f && cameraTransform_.translate.y <= modelTransform_.translate.y + 21.0f)
	{
		camera_->SetParent(cameraMatrix);
	}
	else
	{
		camera_->SetTranslateParent(cameraMatrix);
	}

	// 前回フレームのカメラの位置を格納(CameraVelocity計算のため)
	cameraVelocityPre = cameraTransform_.translate;

	// 前回フレームのカメラの位置を格納(CameraVelocity計算のため)
	defaultCameraVelocityPre = defaultCameraTransform.translate;
}

void Player::Jump()
{
	if (onGround_) {
		//OutputDebugStringA("tex");
		if (input_->PushKey(DIK_SPACE) || input_->PushButton(Controller::A)) {
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
}

void Player::CheckCollsion(LightBlock* block)
{

	const AABB& blockAABB = block->GetAABB();
	const AABB& playerAABB = object3d_->GetAABB();

	if (IsCollisionAABB(playerAABB, blockAABB)) {
		//突き抜けたかについて
		float overlapLeft = std::abs(playerAABB.max.x - blockAABB.min.x);
		float overlapRight = std::abs(blockAABB.max.x - playerAABB.min.x);
		float overlapTop = std::abs(playerAABB.min.y - blockAABB.max.y);
		float overlapBottom = std::abs(blockAABB.min.y - playerAABB.max.y);
		float overlapBack = std::abs(playerAABB.max.z - blockAABB.min.z);
		float overlapFront = std::abs(blockAABB.max.z - playerAABB.min.z);


		/*もっとも小さいオーバーラップを優先的に解決*/
		float minOverlapX = std::min(overlapLeft, overlapRight);
		float minOverlapY = std::min(overlapTop, overlapBottom);
		float minOverlapZ = std::min(overlapBack, overlapFront);
		if (minOverlapX < minOverlapY && minOverlapX <= minOverlapZ) {
			if (overlapLeft < overlapRight) {

				/*左衝突*/
				modelTransform_.translate.x -= overlapLeft;
			}
			else {
				/*右衝突*/
				modelTransform_.translate.x += overlapRight;
			}
			velocity.x = 0;
		}
		else if (minOverlapY <= minOverlapX && minOverlapY <= minOverlapZ) {
			if (overlapTop < overlapBottom) {
				/*上から着地*/
				modelTransform_.translate.y -= overlapTop;
				velocity.y = 0;

			}
			else {
				/*下からぶつかった*/
				modelTransform_.translate.y += overlapBottom;
				velocity.y = 0;
			}
		}
		else {
			// Z軸方向で解決
			if (overlapBack < overlapFront) {
				modelTransform_.translate.z -= overlapBack;
			}
			else {
				modelTransform_.translate.z += overlapFront;
			}
			velocity.z = 0;
		}
	}
}

void Player::UpdateStageCollision() {
	//if (collision->GetCollisionListSize() > 0)
	//{
	//	LenXZ len = collision->GetLenXZ(object3d_->GetAABB(), velocity);

	//	if (collision->GetLenXZ(object3d_->GetAABB(), velocity) == LenXZ::X)
	//	{
	//		// 衝突判定をするためのもの
	//		modelTransform_.translate += collision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

	//		object3d_->SetTranslate(modelTransform_.translate);
	//		object3d_->Update();

	//		// 衝突判定をするためのもの
	//		modelTransform_.translate += collision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

	//		object3d_->SetTranslate(modelTransform_.translate);
	//		object3d_->Update();
	//	}
	//	else
	//		if (collision->GetLenXZ(object3d_->GetAABB(), velocity) == LenXZ::Z)
	//		{
	//			// 衝突判定をするためのもの
	//			modelTransform_.translate += collision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

	//			object3d_->SetTranslate(modelTransform_.translate);
	//			object3d_->Update();
	//			// 衝突判定をするためのもの
	//			modelTransform_.translate += collision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

	//			object3d_->SetTranslate(modelTransform_.translate);
	//			object3d_->Update();
	//		}

	//	// 衝突判定をするためのもの
	//	modelTransform_.translate += collision->UpdateCollisionY(object3d_->GetAABB(), JumpVelocity);

	//	if (!onGround_ && collision->IsColYUnderside(object3d_->GetAABB(), JumpVelocity)) {
	//		JumpVelocity = 0.0f;
	//	}
	//	if (!onGround_ && collision->IsColYUpside(object3d_->GetAABB(), JumpVelocity)) {
	//		JumpVelocity = 0.0f;
	//		onGround_ = true;
	//		collisionLightBlock = false;
	//	}
	//	else if (!collision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && !collisionLightBlock)
	//	{
	//		onGround_ = false;
	//	}
	//}
	bool X = false;
	bool Z = false;
	collision->GetLenXZVelocity(velocity);
	LenXZ len = collision->GetLenXZ();

	if (len == LenXZ::X)
	{
		Z = false;	
		X = true;
	}
	else
	{
		X = false;
		Z = true;
	}

	if (collision->GetCollisionListSize() > 0)
	{

		// 衝突判定をするためのもの
		modelTransform_.translate += collision->UpdateCollisionY(object3d_->GetAABB(), JumpVelocity);

		if (len == LenXZ::X)
		{
			// 衝突判定をするためのもの
			modelTransform_.translate += collision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();

			// 衝突判定をするためのもの
			modelTransform_.translate += collision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();
		}
		else
			if (len == LenXZ::Z)
			{
				// 衝突判定をするためのもの
				modelTransform_.translate += collision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

				object3d_->SetTranslate(modelTransform_.translate);
				object3d_->Update();
				// 衝突判定をするためのもの
				modelTransform_.translate += collision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

				object3d_->SetTranslate(modelTransform_.translate);
				object3d_->Update();
			}


		if (!onGround_ && collision->IsColYUnderside(object3d_->GetAABB(), JumpVelocity)) {
			JumpVelocity = 0.0f;
		}
		if (!onGround_ && collision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && JumpVelocity < 0.0f) {
			JumpVelocity = 0.0f;
			onGround_ = true;
			collisionLightBlock = false;
		}
		else if (!collision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && !collisionLightBlock)
		{
			onGround_ = false;
		}
		ImGui::Begin("Len");
		ImGui::Checkbox("X", &X);
		ImGui::Checkbox("Z", &Z);
		ImGui::End();
	}
}

void Player::SetSwitchFlag(const bool flag) { switchFlag = flag; }

void Player::UpdateLightCollision() {
	//if (lightCollision->GetCollisionListSize() > 0 && switchFlag == true)
	//{
	//	LenXZ len = lightCollision->GetLenXZ(object3d_->GetAABB(), velocity);

	//	if (lightCollision->GetLenXZ(object3d_->GetAABB(), velocity) == LenXZ::X)
	//	{
	//		// 衝突判定をするためのもの
	//		modelTransform_.translate += lightCollision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

	//		object3d_->SetTranslate(modelTransform_.translate);
	//		object3d_->Update();

	//		// 衝突判定をするためのもの
	//		modelTransform_.translate += lightCollision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

	//		object3d_->SetTranslate(modelTransform_.translate);
	//		object3d_->Update();
	//	}
	//	else
	//		if (lightCollision->GetLenXZ(object3d_->GetAABB(), velocity) == LenXZ::Z)
	//		{
	//			// 衝突判定をするためのもの
	//			modelTransform_.translate += lightCollision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

	//			object3d_->SetTranslate(modelTransform_.translate);
	//			object3d_->Update();
	//			// 衝突判定をするためのもの
	//			modelTransform_.translate += lightCollision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

	//			object3d_->SetTranslate(modelTransform_.translate);
	//			object3d_->Update();
	//		}

	//	// 衝突判定をするためのもの
	//	modelTransform_.translate += lightCollision->UpdateCollisionY(object3d_->GetAABB(), JumpVelocity);

	//	if (!onGround_ && lightCollision->IsColYUnderside(object3d_->GetAABB(), JumpVelocity)) {
	//		JumpVelocity = 0.0f;
	//	}
	//	if (!onGround_ && lightCollision->IsColYUpside(object3d_->GetAABB(), JumpVelocity)) {
	//		JumpVelocity = 0.0f;
	//		onGround_ = true;
	//		collisionLightBlock = true;
	//	}
	//	else if (!lightCollision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && collisionLightBlock)
	//	{
	//		onGround_ = false;
	//	}
	//}
	if (lightCollision->GetCollisionListSize() > 0 && switchFlag == true)
	{

		// 衝突判定をするためのもの
		modelTransform_.translate += lightCollision->UpdateCollisionY(object3d_->GetAABB(), JumpVelocity);

		lightCollision->GetLenXZVelocity(velocity);
		LenXZ len = collision->GetLenXZ();

		if (len == LenXZ::X)
		{
			// 衝突判定をするためのもの
			modelTransform_.translate += lightCollision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();

			// 衝突判定をするためのもの
			modelTransform_.translate += lightCollision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();
		}
		else if (len == LenXZ::Z)
		{
			// 衝突判定をするためのもの
			modelTransform_.translate += lightCollision->UpdateCollisionZ(object3d_->GetAABB(), velocity.z);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();
			// 衝突判定をするためのもの
			modelTransform_.translate += lightCollision->UpdateCollisionX(object3d_->GetAABB(), velocity.x);

			object3d_->SetTranslate(modelTransform_.translate);
			object3d_->Update();
		}


		if (!onGround_ && lightCollision->IsColYUnderside(object3d_->GetAABB(), JumpVelocity)) {
			JumpVelocity = 0.0f;
		}
		if (!onGround_ && lightCollision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && JumpVelocity < 0.0f) {
			JumpVelocity = 0.0f;
			onGround_ = true;
			collisionLightBlock = true;
		}
		else if (!lightCollision->IsColYUpside(object3d_->GetAABB(), JumpVelocity) && collisionLightBlock)
		{
			onGround_ = false;
		}


	}
}

void Player::UpdateCameraCollision() {
	if (collision->GetCollisionListSize() > 0)
	{

		collision->GetLenXZVelocity(cameraVelocity);
		LenXZ len = collision->GetLenXZ();

		bool X = false;
		bool Z = false;

		if (len == LenXZ::X)
		{
			Z = false;
			X = true;
		}
		else
		{
			X = false;
			Z = true;
		}


		// 衝突判定処理
		Vector3 off = collision->UpdateCameraCollision(cameraAABB, object3d_->GetAABB(), cameraVelocity, cameraOffset, defaultCameraOffset);
		// 初期のカメラオフセットからの判定処理も行っておく
		Vector3 defOff = collision->UpdateCameraCollision(defaultCameraAABB, object3d_->GetAABB(), defaultCameraVelocity, defaultCameraOffset, defaultCameraOffset);
		// offの値とcameraOffsetの値が違えばeasingを使用してoffの値に置換する
		if ( off.z != cameraOffset.z && !cameraZoomIn && !cameraZoomOut)
		{
			cameraZoomIn = true;
			cameraZoomOut = false;
			cameraEasingTime = 0.0f;
			beforCameraOffset = off;
		}
		else if (off.z >= defOff.z && !cameraZoomOut && !cameraZoomIn)
		{
			cameraZoomOut = true;
			cameraZoomIn = false;
			cameraEasingTime = 0.0f;
		}
		// イージング処理
		if (cameraZoomIn)
		{
			Vector3 camOff = cameraOffset;
			cameraEasingTime++;
			float time = cameraEasingTime / 60 / 1.8f;
			cameraOffset = easeInOut(time, camOff, off);
			// イージングの終了処理
			if (time > 1.0f)
			{
				cameraZoomIn = false;
				cameraEasingTime = 0.0f;
			}
		}
		else if (cameraZoomOut)
		{
			Vector3 camOff = cameraOffset;
			cameraEasingTime++;
			float time = cameraEasingTime / 60 / 3.4f;
			cameraOffset = easeInOut(time, camOff, defOff);
			// ズームアウトは少し長く
			if (time > 1.0f)
			{
				cameraZoomOut = false;
				cameraEasingTime = 0.0f;
			}
		}

		//cameraOffset += off;

		ImGui::Begin("CameraLen");
		ImGui::Checkbox("X", &X);
		ImGui::Checkbox("Z", &Z);
		ImGui::DragFloat3("cameraOffset", &cameraOffset.x, 0.1f);
		ImGui::DragFloat3("off", &off.x);
		ImGui::DragFloat3("defOff", &defOff.x);
		ImGui::DragFloat3("beforCamOff", &beforCameraOffset.x);
		ImGui::DragFloat3("cameraVelocity", &cameraVelocity.x);
		ImGui::DragFloat3("cameraMin", &cameraAABB.min.x);
		ImGui::DragFloat3("cameraMax", &cameraAABB.max.x);
		ImGui::DragFloat("easingTime", &cameraEasingTime);
		ImGui::End();
	}
}

// 衝突判定の実装で追加したもの
Vector3 Player::GetPosition() {
	Vector3 result;
	result.x = object3d_->GetWorldMatrix().m[3][0];
	result.y = object3d_->GetWorldMatrix().m[3][1];
	result.z = object3d_->GetWorldMatrix().m[3][2];
	return result;
}


bool Player::IsCollisionAABB(const AABB& a, const AABB& b) {
	if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}
	return false;
}

void Player::EffectBorn(Vector3 position)
{
	/*生成*/
	JampEffect* effect = new JampEffect();
	Vector3 velocity = { distrubution(randomEngine),distrubution(randomEngine),0.0f };
	Normalize(velocity);
	/*初期化*/
	effect;

}


