#include "Rule.h"
#include "Light.h"
#include <algorithm>

#include "Audio.h"

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

void Rule::Initialize()
{
	// クリック音読み込み
	Audio::GetInstance()->LoadMP3("Resources/Sound/mouse/click.mp3", "click", 1.0f); // 音量1.0f

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Stage3", "stage03.obj", true);

	camera = new Camera();
	camera->SetRotate(Vector3(0.36f, 0.0f, 0.0f));

	input = Input::GetInstance();
	input->ShowMouseCursor(showCursor);

	Object3dBase::GetInstance()->SetDefaultCamera(camera);

	object3d = new Object3d();
	object3d->Initialize();
	object3d->SetModel("stage03.obj");

	Light::GetInstance()->SetSpecularColorDirectionalLight({ 0.0f, 0.0f, 0.0f });

	sprite = new Sprite();
	sprite->Initialize("Resources/uvChecker.png");

	cameraTransform.scale = { 1.0f, 1.0f, 1.0f };

	player = new Player();
	player->Initialize(camera);
	player->AddStageCollision("Resources/Model/collision/Stage03", "stage03Collision.obj");
	player->AddLightBlockCollision("Resources/Model/collision/Stage03", "stage03LightCollision.obj");

	button = new UI();
	button->CreateButton({ 0.0f, 0.0f }, Origin::LeftTop, "Resources/Sprite/clearShift.png");

	modelTransform = object3d->GetTransform();

	goal = new Goal();
	goal->Initialize({ -10.0f,8.0f,10.0f });

	starResultManager = new starResult();
	starResultManager->Initialize(5);
	//==BLOCK===
	lightBlock = new LightBlock();
	lightBlock->Initialize("Resources/Model/obj/Stage3", "stage03Light.obj");
	//switch

	lightSwitch = new switchLight();
	switchTransform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.5f, 4.0f}
	};
	lightSwitch->Initialize(switchTransform/*, camera, directxBase*/, input, player);

	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/clearShift.png");

	resumeButton.CreateButton({ 540, 250 }, Origin::Center, "Resources/Sprite/gameUI/resume.png");
	restartButton.CreateButton({ 540, 320 }, Origin::Center, "Resources/Sprite/gameUI/restart.png");
	returnToTitleButton.CreateButton({ 540, 390 }, Origin::Center, "Resources/Sprite/gameUI/Gametitle.png");

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
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/set.png");
	// スプライト生成
	escHintSprite = new Sprite();
	escHintSprite->Initialize("Resources/Sprite/scene/set.png");
	// 原点
	escHintSprite->SetAnchorPoint({ 0.0f, 0.0f });
	// 位置
	escHintSprite->SetPosition({ 50.0f, 50.0f });
	// スケール
	escHintSprite->SetScale({ 430.0f, 240.0f });
	escHintSprite->Update();

	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/star.png");

	for (int i = 0; i < 3; ++i) {
		Sprite* icon = new Sprite();
		icon->Initialize("Resources/Sprite/star.png");
		icon->SetScale({ 32.0f, 32.0f });
		icon->SetAnchorPoint({ 0.0f, 0.0f });
		starIcons.push_back(icon);
	}

	// チュートリアル開始スプライトの初期化
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/target.png");
	targetSprite = new Sprite();
	targetSprite->Initialize("Resources/Sprite/scene/target.png");
	targetSprite->Update();


}

void Rule::PauseUpdate()
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
				Audio::GetInstance()->Play("click"); // クリック音再生
				pauseSelectedIndex = (pauseSelectedIndex + 1) % pauseButtonCount;
				pauseInputLocked = true;
			}
			else if (input->TriggerKey(DIK_UP) || input->TriggerXButton(DPad::Up)) {
				Audio::GetInstance()->Play("click"); // クリック音再生
				pauseSelectedIndex = (pauseSelectedIndex - 1 + pauseButtonCount) % pauseButtonCount;
				pauseInputLocked = true;
			}
		}
		else {
			if (!input->TriggerXButton(DPad::Up) && !input->TriggerXButton(DPad::Down)) {
				Audio::GetInstance()->Play("click"); // クリック音再生
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
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::Y)) {
		Audio::GetInstance()->Play("click"); // クリック音再生
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
		Audio::GetInstance()->Play("click"); // クリック音再生
		isPaused = false;
		input->ShowMouseCursor(false);
		return;
	}
	if (restartButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		goToRestart = true;
		isPaused = false;
		return;
	}
	if (returnToTitleButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		goToTitle = true;
		return;
	}
}


void Rule::Update() {

	// チュートリアルスプライト
	if (targetStart) {
		input->Update();
		// スプライトの更新
		targetSprite->Update();

		if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::Y)) {
			Audio::GetInstance()->Play("click"); // クリック音再生
			targetStart = false;
			input->ShowMouseCursor(false);
		}

	}

	if (input->TriggerKey(DIK_ESCAPE) || input->TriggerButton(Controller::Menu)) {
		Audio::GetInstance()->Play("click"); // クリック音再生
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


	camera = player->GetCamera();
	camera->Update();
	object3d->SetTransform(modelTransform);
	object3d->SetEnableLighting(enableLighting);
	object3d->Update();
	aabb = object3d->GetAABB();
	sprite->Update();

	lightBlock->Update();

	goal->Update();


	// ゴールの当たり判定 ステージセレクトへ飛ぶ。　
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

	input->Update();

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


	////ENTERでタイトルへ戻る
	//if (input->TriggerKey(DIK_RETURN)) {
	//	finished = true;
	//}
}

void Rule::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();

	if (targetStart && targetSprite) {
		targetSprite->Draw();
		return;
	}




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

void Rule::Finalize() {
	Light::GetInstance()->SetColorDirectionalLight({ 1.0f, 1.0f, 1.0f, 1.0f });

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

	if (targetSprite) {
		delete targetSprite;
		targetSprite = nullptr;
	}



}
