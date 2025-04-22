#include "GameScene.h"
#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

void GameScene::Initialize() {

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "stage.obj");

	//TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	camera = new Camera();
	camera->SetRotate(Vector3(0.36f, 0.0f, 0.0f));

	input = Input::GetInstance();
	input->ShowMouseCursor(showCursor);

	Object3dBase::GetInstance()->SetDefaultCamera(camera);

	object3d = new Object3d();
	object3d->Initialize();
	object3d->SetModel("stage.obj");

	sprite = new Sprite();
	sprite->Initialize("Resources/uvChecker.png");

	cameraTransform.scale = { 1.0f, 1.0f, 1.0f };

	player = new Player();
	player->Initialize(camera);

	button = new Button();
	button->CreateButton({ 0.0f, 0.0f }, Origin::LeftTop, "Resources/Sprite/endButton.png");

	modelTransform = object3d->GetTransform();

	goal = new Goal();
	goal->Initialize({ 8.0f,4.0f,11.0f });

	star = new Star();
	star->Initialize({ 0.0f,0.0f,0.0f });

	starResultManager = new starResult();
	starResultManager->Initialize(); //{ 0.0f,0.0f,0.0f },


	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/clear.png");
	clearSprite = new Sprite();
	clearSprite->Initialize("Resources/Sprite/clear.png");
	//Vector3(0.0f, 0.0f, 0.0f)
	

}

void GameScene::Update() {

	ImGui::Begin("State");
	if (ImGui::TreeNode("Camera")) {
		ImGui::DragFloat3("Tranlate", &cameraTransform.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &cameraTransform.rotate.x, 0.1f);
		ImGui::DragFloat3("Scale", &cameraTransform.scale.x, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Model")) {
		ImGui::DragFloat3("Tranlate", &modelTransform.translate.x, 0.1f);
		ImGui::DragFloat3("Rotate", &modelTransform.rotate.x, 0.1f);
		ImGui::DragFloat3("Scale", &modelTransform.scale.x, 0.1f);
		if (ImGui::TreeNode("AABB")) {
			ImGui::DragFloat3("Min", &aabb.min.x, 0.1f);
			ImGui::DragFloat3("Max", &aabb.max.x, 0.1f);

			ImGui::TreePop();
		}
		ImGui::Checkbox("EnableLihting", &enableLighting);
		ImGui::TreePop();
	}
	ImGui::End();

	if (input->TriggerKey(DIK_ESCAPE))
	{
		finished = true;
	}
	if (button->OnButton())
	{
		finished = true;
	}
	const float speed = 0.7f;
	Vector3 velocity(0.0f, 0.0f, speed);
	velocity = TransformNormal(velocity, camera->GetWorldMatrix());
	if (input->PushKey(DIK_W)) {
		cameraTransform.translate += velocity;
	}
	if (input->PushKey(DIK_S)) {
		cameraTransform.translate -= velocity;
	}
	velocity = { speed, 0.0f, 0.0f };
	velocity = TransformNormal(velocity, camera->GetWorldMatrix());
	if (input->PushKey(DIK_A)) {
		cameraTransform.translate -= velocity;
	}
	if (input->PushKey(DIK_D)) {
		cameraTransform.translate += velocity;
	}
	if (input->PushKey(DIK_SPACE)) {
		cameraTransform.translate.y += 1.0f;
	}
	if (input->PushKey(DIK_LSHIFT)) {
		cameraTransform.translate.y -= 1.0f;
	}
	if (input->PushKey(DIK_LEFT)) {
		cameraTransform.rotate.y -= 0.03f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraTransform.rotate.y += 0.03f;
	}
	if (input->PushKey(DIK_UP)) {
		cameraTransform.rotate.x -= 0.03f;
	}
	if (input->PushKey(DIK_DOWN)) {
		cameraTransform.rotate.x += 0.03f;
	}
	if (input->PushKey(DIK_Q)) {
		cameraTransform.rotate.z -= 0.01f;
	}
	if (input->PushKey(DIK_E)) {
		cameraTransform.rotate.z += 0.01f;

	}
	if (input->TriggerKey(DIK_LCONTROL))
	{
		showCursor = !showCursor;
		input->ShowMouseCursor(showCursor);
	}
	if (input->TriggerKey(DIK_0))
	{
		button->SetSprite("Resources/Sprite/button.png");
	}
	if (input->TriggerKey(DIK_1))
	{
		button->SetSprite("Resources/Sprite/endButton.png");
	}
	if (input->TriggerKey(DIK_2))
	{
		Transform ta = button->GetTransform();
		ta.scale.x += 5.0f;
		ta.scale.y += 5.0f;
		button->SetTransform(ta);
	}

	player->Update();
	//camera->SetTranslate(cameraTransform.translate);
	//camera->SetRotate(cameraTransform.rotate);
	camera = player->GetCamera();
	camera->Update();
	object3d->SetTransform(modelTransform);
	object3d->SetEnableLighting(enableLighting);
	object3d->Update();
	aabb = object3d->GetAABB();
	sprite->Update();

	input->Update();


	goal->Update();
	//clearSprite->Update();

	// ゴールの当たり判定
	if (!isGoal && goal->OnCollision(player->GoalObject3d())) {
		isGoal = true;

		if (isGoal) {
			clearSprite->Update();
		}

		return;

	}

	star->Update();
	if (starResultManager) {
		starResultManager->Update();  // プレイヤー情報を渡す player
	}

	// 星の当たり判定
	if (star->OnCollision(player->StarObject3d())) {
		return;
	}



}

void GameScene::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();

	//sprite->Draw();
  
	button->Draw();

	Object3dBase::GetInstance()->ShaderDraw();

	object3d->Draw();

	player->Draw();

	goal->Draw();
	clearSprite->Draw();

	star->Draw();
	// starResultManager とその中の星を描画
	if (starResultManager) {
		starResultManager->Draw();
	}


}

void GameScene::Finalize() {

	delete camera;

	delete object3d;

	delete sprite;

	delete player;


	delete goal;

	delete clearSprite;

	delete star;
	if (starResultManager) {
		delete starResultManager;
	}


	delete button;

}