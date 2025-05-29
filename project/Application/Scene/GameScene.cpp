#include "GameScene.h"
#include "Light.h"
#include <algorithm>

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"


void GameScene::Initialize(int stage) {
	stage_ = stage;

	
	//ModelManager::GetInstance()->LoadModel("Resources/Debug", "test.obj", true);

	//TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");


	camera = new Camera();
	camera->SetRotate(Vector3(0.36f, 0.0f, 0.0f));

	input = Input::GetInstance();
	input->ShowMouseCursor(showCursor);

	Object3dBase::GetInstance()->SetDefaultCamera(camera);

	object3d = new Object3d();
	object3d->Initialize();

	//ステージ指定
	//
	if (stage_ == 1) {
		object3d->SetModel("stage03.obj");
	}
	else if (stage_ == 2) {
		object3d->SetModel("Stage2.obj");
	}
	else if (stage_ == 3) {
		object3d->SetModel("01Stage.obj");
	}
	else if (stage_ == 4) {
		object3d->SetModel("Stage2.obj");
	}

	Light::GetInstance()->SetSpecularColorDirectionalLight({ 0.0f, 0.0f, 0.0f });

	sprite = new Sprite();
	sprite->Initialize("Resources/uvChecker.png");

	cameraTransform.scale = { 1.0f, 1.0f, 1.0f };

	player = new Player();
	player->Initialize(camera);

	//ステージとプレイヤーの当たり判定
	//
	if (stage_ == 1) {
		player->AddStageCollision("Resources/Model/collision/Stage03", "stage03Collision.obj");
	}
	else if (stage_ == 2) {
		player->AddStageCollision("Resources/Model/collision", "Stage2Collision.obj");
	}
	else if (stage_ == 3) {
		player->AddStageCollision("Resources/Model/collision", "01StageCollision.obj");
	}
	else if (stage_ == 4) {
		player->AddStageCollision("Resources/Model/collision", "Stage2Collision.obj");
	}

	//player->AddStageCollision("Resources/Debug", "test.obj");
	// 
	//ライトブロックとプレイヤーの当たり判定
	//
	if (stage_ == 1) {
		player->AddLightBlockCollision("Resources/Model/collision/Stage03", "stage03LightCollision.obj");
	}
	else if (stage_ == 2) {
		player->AddLightBlockCollision("Resources/Model/collision", "Stage2LightCollision.obj");
	}
	else if (stage_ == 3) {
		player->AddLightBlockCollision("Resources/Model/collision", "proStageLightCollision.obj");
	}
	else if (stage_ == 4) {
		player->AddLightBlockCollision("Resources/Model/collision", "Stage2LightCollision.obj");
	}


	button = new UI();
	button->CreateButton({ 0.0f, 0.0f }, Origin::LeftTop, "Resources/Sprite/clearShift.png");

	modelTransform = object3d->GetTransform();

	goal = new Goal();
	Vector3 goalPos = { 0.0f,0.0f,0.0f };

	//ゴールの位置
	//
	if (stage_ == 1) {
		goalPos = { -7.0f, 5.0f,0.0f };
	}
	if (stage_ == 2) {
		goalPos = { 1.0f,8.0f,1.0f };
	}
	if (stage_ == 3) {
		goalPos = { -10.0f,8.0f,10.0f };
	}
	if (stage_ == 4) {
		goalPos = { -10.0f,8.0f,10.0f };
	}

	goal->Initialize(goalPos);

	starResultManager = new starResult();
	starResultManager->Initialize(stage_); //{ 0.0f,0.0f,0.0f },
	//==BLOCK===
	lightBlock = new LightBlock();
	//ライトブロックの指定
	//
	if (stage_ == 1) {
		lightBlock->Initialize("Resources/Model/obj/Stage3", "stage03Light.obj");
	}
	else if (stage_ == 2) {
		lightBlock->Initialize("Resources/Model/obj/Stage2", "Stage2LightBlock.obj");
	}
	else if (stage_ == 3) {
		lightBlock->Initialize("Resources/Model/obj/Stage", "proStageLightBlock.obj");
	}
	else if (stage_ == 4) {
		lightBlock->Initialize("Resources/Model/obj/Stage2", "Stage2LightBlock.obj");
	}

	//switch
	// 
	lightSwitch = new switchLight();
	//スイッチの位置
	//
	if (stage_ == 1) {
		switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{8.0f, 2.2f, 11.0f}
		};
	}
	if (stage_ == 2) {
		switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{4.0f, 1.0f, 4.0f}
		};
	}
	if (stage_ == 3) {
		switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 4.0f}
		};
	}
	if (stage_ == 4) {
		switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 4.0f}
		};
	}

	lightSwitch->Initialize(switchTransform/*, camera, directxBase*/, input, player);

	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/clearShift.png");

	resumeButton.CreateButton({ 540, 230 }, Origin::Center, "Resources/Sprite/gameUI/resume.png");
	restartButton.CreateButton({ 540, 300 }, Origin::Center, "Resources/Sprite/gameUI/restart.png");
	returnToTitleButton.CreateButton({ 540, 370 }, Origin::Center, "Resources/Sprite/gameUI/Gametitle.png");


	resumeButton.CreateButton({ 540, 250 }, Origin::Center, "Resources/Sprite/gameUI/resume.png");
	restartButton.CreateButton({ 540, 320 }, Origin::Center, "Resources/Sprite/gameUI/restart.png");
	returnToTitleButton.CreateButton({ 540, 390 }, Origin::Center, "Resources/Sprite/gameUI/Select.png");

	//クリア時に星の取得情報を送るよう
	Result = 0;

	//soundData = Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");


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


	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/star.png");

	for (int i = 0; i < 3; ++i) {
		Sprite* icon = new Sprite();
		icon->Initialize("Resources/Sprite/star.png");
		icon->SetScale({ 32.0f, 32.0f });
		icon->SetAnchorPoint({ 0.0f, 0.0f });
		starIcons.push_back(icon);
	}


}

void GameScene::PauseUpdate()
{
	input->Update();

	input->ShowMouseCursor(true);

	// --- カーソルによるUI選択 ---
	hoveredPauseButton = nullptr;
	if (resumeButton.InCursor()) hoveredPauseButton = &resumeButton;
	else if (restartButton.InCursor()) hoveredPauseButton = &restartButton;
	else if (returnToTitleButton.InCursor()) hoveredPauseButton = &returnToTitleButton;

	// 点滅タイマーリセット（カーソルが移動したとき）
	if (hoveredPauseButton != prevHoveredPauseButton) {
		pauseBlinkTimer = 0.0f;
		prevHoveredPauseButton = hoveredPauseButton;
	}

	// 点滅アニメーション進行
	pauseBlinkTimer += 1.0f / 60.0f;
	float blinkAlpha = 0.5f + 0.5f * sinf(pauseBlinkTimer * 3.14f);

	// 十字キー操作（カーソルが使われていないとき）
	if (!hoveredPauseButton) {
		prevPauseSelectedIndex = pauseSelectedIndex;

		if (!pauseInputLocked) {
			if (input->TriggerKey(DIK_DOWN) || input->TriggerXButton(DPad::Down)) {
				pauseSelectedIndex = (pauseSelectedIndex + 1) % pauseButtonCount;
				pauseInputLocked = true;
			}
			else if (input->TriggerKey(DIK_UP) || input->TriggerXButton(DPad::Up)) {
				pauseSelectedIndex = (pauseSelectedIndex - 1 + pauseButtonCount) % pauseButtonCount;
				pauseInputLocked = true;
			}
		}
		else {
			if (!input->TriggerXButton(DPad::Up) && !input->TriggerXButton(DPad::Down)) {
				pauseInputLocked = false;
			}
		}

		if (pauseSelectedIndex != prevPauseSelectedIndex) {
			pauseBlinkTimer = 0.0f;
		}
	}

	// 全ボタン透明度初期化
	resumeButton.SetSpriteAlpha(1.0f);
	restartButton.SetSpriteAlpha(1.0f);
	returnToTitleButton.SetSpriteAlpha(1.0f);

	// 点滅：カーソルが優先
	if (hoveredPauseButton) {
		hoveredPauseButton->SetSpriteAlpha(blinkAlpha);
	}
	else {
		switch (pauseSelectedIndex) {
		case 0: resumeButton.SetSpriteAlpha(blinkAlpha); break;
		case 1: restartButton.SetSpriteAlpha(blinkAlpha); break;
		case 2: returnToTitleButton.SetSpriteAlpha(blinkAlpha); break;
		}
	}

	// 決定：Enter / Aボタン
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::A)) {
		if (hoveredPauseButton) {
			if (hoveredPauseButton == &resumeButton) {
				isPaused = false;
				input->ShowMouseCursor(false);
				return;
			}
			else if (hoveredPauseButton == &restartButton) {
				goToRestart = true;
				isPaused = false;
				return;
			}
			else if (hoveredPauseButton == &returnToTitleButton) {
				goToTitle = true;
				return;
			}
		}
		else {
			switch (pauseSelectedIndex) {
			case 0: isPaused = false; input->ShowMouseCursor(false); return;
			case 1: goToRestart = true; isPaused = false; return;
			case 2: goToTitle = true; return;
			}
		}
	}

	// マウスクリック決定（OnButton）
	if (resumeButton.OnButton()) {
		isPaused = false;
		input->ShowMouseCursor(false);
		return;
	}
	if (restartButton.OnButton()) {
		goToRestart = true;
		isPaused = false;
		return;
	}
	if (returnToTitleButton.OnButton()) {
		goToTitle = true;
		return;
	}
}


void GameScene::Update() {

	Vector2 posM2 = input->GetWindowMousePos2();

	Vector3 posM3 = input->GetWindowMousePos3();

	/*ImGui::Begin("State");
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
	ImGui::End();*/

	if (input->TriggerKey(DIK_ESCAPE) || input->TriggerButton(Controller::Y)) {
		isPaused = !isPaused;
		tabReleased = false;

		// カーソルの表示状態をポーズに応じて変更
		input->ShowMouseCursor(isPaused);

		// 入力状態の初期化（ポーズ解除後に誤動作しないように）
		pauseInputLocked = false;
		pauseSelectedIndex = 0;
		prevPauseSelectedIndex = -1;
		hoveredPauseButton = nullptr;
		prevHoveredPauseButton = nullptr;
		pauseBlinkTimer = 0.0f;
	}


	// ポーズ中の処理
	if (isPaused) {

		PauseUpdate(); // ポーズ中のUI更新

		return;  // ゲーム本体の更新を止める
	}


	if (input->TriggerKey(DIK_LCONTROL))
	{
		showCursor = !showCursor;
		input->ShowMouseCursor(showCursor);
	}

	player->Update();

	// プレイヤーが場外に出ていたらリスタート
	if (player->IsDead())
	{
		goToRestart = true;
	}


	camera = player->GetCamera();
	camera->Update();
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
			Initialize(stage_);
		}
	}
	lightSwitch->Update();
	player->SetSwitchFlag(lightSwitch->GetFlag());

	input->Update();

	float di = Light::GetInstance()->GetIntensityDirectionalLight();
	di += 0.001f;
	di = std::clamp(di, 0.0f, 1.0f);
	Light::GetInstance()->SetIntensityDirectionalLight(di);

	if (starResultManager) {
		starResultManager->Update();  // プレイヤー情報を渡す player
	}

	// 星との当たり判定（取得）
	for (Star* s : starResultManager->GetStars()) {
		if (!s->IsCollected() && s->OnCollision(player->StarObject3d())) {
			s->Collect(); // 取得済みにする
			Result++;
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


	// ポーズ中のカーソル位置に応じたボタンの描画
	if (fadeSprite) fadeSprite->Draw();


	// 星アイコンの描画（星が取得された場合のみ）
	if (starResultManager) {
		int collectedCount = 0;
		for (Star* s : starResultManager->GetStars()) {
			if (s->IsCollected() && collectedCount < (int)starIcons.size()) {
				float x = 1280.0f - 40.0f - (collectedCount * 36.0f);
				float y = 20.0f;
				starIcons[collectedCount]->SetPosition({ x, y });
				starIcons[collectedCount]->Update();
				starIcons[collectedCount]->Draw();
				++collectedCount;
			}
		}
	}



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

	for (Sprite* s : starIcons) {
		delete s;
	}
	starIcons.clear();

}