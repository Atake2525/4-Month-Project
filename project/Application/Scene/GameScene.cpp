#include "GameScene.h"
#include "Light.h"
#include <algorithm>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"


void GameScene::Initialize() {

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Stage", "01Stage.obj", true);
	//ModelManager::GetInstance()->LoadModel("Resources/Debug", "test.obj", true);

	//TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	camera = new Camera();
	camera->SetRotate(Vector3(0.36f, 0.0f, 0.0f));

	input = Input::GetInstance();
	//input->ShowMouseCursor(showCursor);

	Object3dBase::GetInstance()->SetDefaultCamera(camera);

	object3d = new Object3d();
	object3d->Initialize();
	object3d->SetModel("01Stage.obj");

	Light::GetInstance()->SetSpecularColorDirectionalLight({ 0.0f, 0.0f, 0.0f });

	sprite = new Sprite();
	sprite->Initialize("Resources/uvChecker.png");

	cameraTransform.scale = { 1.0f, 1.0f, 1.0f };

	player = new Player();
	player->Initialize(camera);
	player->AddStageCollision("Resources/Model/collision", "01StageCollision.obj");
	//player->AddStageCollision("Resources/Debug", "test.obj");
	player->AddLightBlockCollision("Resources/Model/collision", "proStageLightCollision.obj");

	button = new UI();
	button->CreateButton({ 0.0f, 0.0f }, Origin::LeftTop, "Resources/Sprite/clearShift.png");

	modelTransform = object3d->GetTransform();

	goal = new Goal();
	goal->Initialize({ 12.0f,12.0f,10.0f });

	starResultManager = new starResult();
	starResultManager->Initialize(); //{ 0.0f,0.0f,0.0f },
	//==BLOCK===
	lightBlock = new LightBlock();
	lightBlock->Initialize("Resources/Model/obj/Stage", "proStageLightBlock.obj");
	//switch
	// 
	lightSwitch = new switchLight();
	switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 4.0f}
	};
	lightSwitch->Initialize(switchTransform/*, camera, directxBase*/, input, player);

	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/clearShift.png");
	//clearSprite = new Sprite();
	//clearSprite->Initialize("Resources/Sprite/clearShift.png");
	//Vector3(0.0f, 0.0f, 0.0f)

	resumeButton.CreateButton({ 540, 250 }, Origin::Center, "Resources/Sprite/gameUI/resume.png");
	restartButton.CreateButton({ 540, 320 }, Origin::Center, "Resources/Sprite/gameUI/restart.png");
	returnToTitleButton.CreateButton({ 540, 390 }, Origin::Center, "Resources/Sprite/gameUI/Gametitle.png");


	soundData = Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

	isPaused = false;
	//ポーズUIの背景
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/gameUI/bg.png");
	// スプライト生成
	pauseBg = new Sprite();
	pauseBg->Initialize("Resources/Sprite/gameUI/bg.png");
	pauseBg->SetScale({ 320.0f, 320.0f });               // 画像サイズ
	pauseBg->SetAnchorPoint({ 0.5f, 0.5f });             // 原点：中心
	pauseBg->SetTextureLeftTop({ 0.5f, 0.5f });
	pauseBg->SetPosition({ 640.0f, 360.0f });            // ウィンドウ中心
	pauseBg->SetColor({ 1.0f, 1.0f, 1.0f, 0.75f });      // 半透明
	pauseBg->Update();

	// テクスチャ読み込み（ESCアイコン）
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/gameUI/ESC.png");
	// スプライト生成
	escHintSprite = new Sprite();
	escHintSprite->Initialize("Resources/Sprite/gameUI/ESC.png");
	// 原点
	escHintSprite->SetAnchorPoint({ 0.0f, 0.0f });
	// 位置
	escHintSprite->SetPosition({ 50.0f, 50.0f });
	// スケール
	escHintSprite->SetScale({ 100.0f, 100.0f });
	escHintSprite->Update();

	//フェードアウト用スプライトの初期化
	fadeSprite = new Sprite();
	fadeSprite->Initialize("Resources/black1x1.png");
	fadeSprite->SetPosition({ 0.0f, 0.0f });
	fadeSprite->SetScale({ 1280.0f, 720.0f });
	fadeSprite->SetAnchorPoint({ 0.0f, 0.0f });
	fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha }); // 最初は真っ暗


	 soundData = Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

}

void GameScene::Update() {

	Vector2 posM2 = input->GetWindowMousePos2();

	Vector3 posM3 = input->GetWindowMousePos3();

	//#ifdef _DEBUGState

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

	ImGui::DragFloat2("M2", &posM2.x, 0.1f);
	ImGui::DragFloat3("M3", &posM3.x, 0.1f);
	ImGui::End();

	//#endif _DEBUGState

		/*if (input->TriggerKey(DIK_ESCAPE))
		{
			finished = true;
		}*/
		/*const float speed = 0.7f;
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

		}*/


	if (isFadingIn) {
		fadeAlpha -= 1.0f / (60.0f * 3.0f); // 3秒で明るく
		if (fadeAlpha <= 0.0f) {
			fadeAlpha = 0.0f;
			isFadingIn = false;
		}
		fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
	}

		// ポーズ切り替え
	if (input->PushKey(DIK_ESCAPE)) {
		if (tabReleased) {
			isPaused = !isPaused;
			tabReleased = false;  // 押された直後に反応したらフラグを下げる
		}
	}
	else {
		// TABが離されたらフラグを戻す
		tabReleased = true;
	}

	// ポーズ中の処理
	if (isPaused) {

		// ボタンの alpha を設定
		UI* hoveredButton = nullptr;
		// どのボタンにカーソルが当たっているか取得
		if (resumeButton.InCursor()) {
			hoveredButton = &resumeButton;
		}
		else if (restartButton.InCursor()) {
			hoveredButton = &restartButton;
		}
		else if (returnToTitleButton.InCursor()) {
			hoveredButton = &returnToTitleButton;

		}

		// 前回と違うボタンに乗ったらタイマーリセット
		if (hoveredButton != prevHoveredButton) {
			blinkTimer = 0.0f;
			prevHoveredButton = hoveredButton;  // 更新
		}

		// 毎フレーム進める 1/60秒
		blinkTimer += 1.0f / 60.0f;
		// アルファ値を 0.5 ～ 1.0 の範囲（周期 2秒）
		float alpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

		resumeButton.SetSpriteAlpha(1.0f);
		restartButton.SetSpriteAlpha(1.0f);
		returnToTitleButton.SetSpriteAlpha(1.0f);

		input->ShowMouseCursor(true);

// 対象ボタンだけ点滅
		if (hoveredButton) {
			hoveredButton->SetSpriteAlpha(alpha);
		}

		if (resumeButton.OnButton()) {
			isPaused = false;
			input->ShowMouseCursor(false);
		}
		if (restartButton.OnButton()) {
			goToRestart = true;
			isPaused = false;
			Update();
		}
		if (returnToTitleButton.OnButton()) {
			goToTitle = true;
		}

		

		return;  // ゲーム本体の更新を止める
	}


	if (input->TriggerKey(DIK_LCONTROL))
	{
		showCursor = !showCursor;
		input->ShowMouseCursor(showCursor);
	}
	/*if (input->TriggerKey(DIK_0))
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
	}*/

	if (input->TriggerKey(DIK_2))
	{
		Audio::GetInstance()->SoundPlayWave(soundData, 0.4f);
	}
	input->Update();

	player->Update();
	camera->Update();

	//camera->SetTranslate(cameraTransform.translate);
	//camera->SetRotate(cameraTransform.rotate);
	object3d->SetTransform(modelTransform);
	object3d->SetEnableLighting(enableLighting);
	object3d->Update();
	aabb = object3d->GetAABB();
	sprite->Update();

	lightBlock->Update();

	goal->Update();

	// ゴールの当たり判定
	if (!isGoal && goal->OnCollision(player->GoalObject3d())) {
		isGoal = true;
	}
	if (isGoal) {
		finished = true;
		if (input->TriggerKey(DIK_LSHIFT) || input->TriggerButton(Controller::Menu))
		{
			Finalize();
			showCursor = false;
			isGoal = false;
			Light::GetInstance()->SetIntensityDirectionalLight(0.0f);
			Initialize();
		}
	}
	lightSwitch->Update();
	player->SetSwitchFlag(lightSwitch->GetFlag());


	float di = Light::GetInstance()->GetIntensityDirectionalLight();
	di += 0.001f;
	di = std::clamp(di, 0.0f, 1.0f);
	Light::GetInstance()->SetIntensityDirectionalLight(di);


	//star->Update();
	if (starResultManager) {
		starResultManager->Update();  // プレイヤー情報を渡す player
	}

	// 星との当たり判定（取得）
	for (Star* s : starResultManager->GetStars()) {
		if (!s->IsCollected() && s->OnCollision(player->StarObject3d())) {
			s->Collect(); // 取得済みにする
			// TODO: ここで音やエフェクトなど入れても良い
		}
	}




}

void GameScene::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();

	Object3dBase::GetInstance()->ShaderDraw();

	object3d->Draw();

	player->Draw();

	goal->Draw();

	// starResultManager とその中の星を描画
	if (starResultManager) {
		starResultManager->Draw();
	}

	lightSwitch->Draw();

	lightBlock->Draw(lightSwitch->GetFlag());

	// ESCヒントスプライトの描画（
	if (escHintSprite) {
		escHintSprite->Draw();
	}

	//ポーズ中のUI描画
	if (isPaused) {
		if (pauseBg) {
			pauseBg->Draw();
		}
		resumeButton.Draw();
		restartButton.Draw();
		returnToTitleButton.Draw();
	}

	SpriteBase::GetInstance()->ShaderDraw();

	if (fadeSprite) fadeSprite->Draw();

}

void GameScene::Finalize() {

	delete camera;

	delete object3d;

	delete sprite;

	delete player;

	delete goal;

	if (starResultManager) {
		delete starResultManager;
	}

	delete button;

	delete lightBlock;

	if (pauseBg) {
		delete pauseBg;
		pauseBg = nullptr;
	}
}