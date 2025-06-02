
#include "Title.h"
#include "TextureManager.h"
#include <dinput.h>
#include "Object3dBase.h"
#include "Audio.h"

void Title::Initialize() {
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// クリック音読み込み
	Audio::GetInstance()->LoadMP3("Resources/Sound/click.mp3", "click", 0.8f); // 音量1.0f
	// bgm読み込み
	Audio::GetInstance()->LoadMP3("Resources/Sound/scene/goast.mp3", "goast", 1.0f); // 音量1.0f



	// -------------------------
	// お化け初期化
	// -------------------------

	camera = new Camera();

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Player", "Player.obj");

	ghostObj = new Object3d();
	ghostObj->Initialize();
	ghostObj->SetModel("Player.obj");
	ghostObj->SetScale({ 0.5f, 0.5f, 0.5f });
	ghostObj->SetCamera(camera);

	ghostObj->SetRotate({ 0.0f, 3.14f, 0.0f });

	ghostPos = { 0.0f, 1.0f, 5.0f };
	ghostObj->SetTranslate(ghostPos);
	ghostObj->Update();

	// -------------------------
	// タイトル画像とボタン
	// -------------------------
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/title.png");
	titleSprite = new Sprite();
	titleSprite->Initialize("Resources/Sprite/scene/title.png");

	gameStartButton.CreateButton({ 150, 350 }, Origin::Center, "Resources/Sprite/titleUI/serect.png");
	settingButton.CreateButton({ 130, 550 }, Origin::Center, "Resources/Sprite/titleUI/settingButton.png");
	ruleButton.CreateButton({ 880, 350 }, Origin::Center, "Resources/Sprite/titleUI/tyu-to.png");
	finishButton.CreateButton({ 880, 550 }, Origin::Center, "Resources/Sprite/titleUI/finish.png");

	// -------------------------
	// フェードスプライト
	// -------------------------
	fadeSprite = new Sprite();
	fadeSprite->Initialize("Resources/black1x1.png");
	fadeSprite->SetPosition({ 0.0f, 0.0f });
	fadeSprite->SetScale({ 1280.0f, 720.0f });
	fadeSprite->SetAnchorPoint({ 0.0f, 0.0f });
	fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });

	// -------------------------
	// タイトルブロック初期化
	// -------------------------
	ModelManager::GetInstance()->LoadModel("Resources/Model/title", "title.obj");
	// Initialize()
	titleBlockObj = new Object3d();
	titleBlockObj->Initialize();
	titleBlockObj->SetModel("title.obj");
	titleBlockObj->SetCamera(camera);

	blockTransform.scale = { 2.0f, 1.0f, 2.0f };
	blockTransform.translate = { 0.0f, -2.0f, 5.0f };
	blockTransform.rotate = { 0.0f, 0.0f, 0.0f };
	titleBlockObj->SetTransform(blockTransform);
	titleBlockObj->Update();


	Audio::GetInstance()->Play("goast", true); // ループ再生


}


void Title::Update() {


	input->Update();
	camera->Update();


	titleBlockObj->Update();

	// ふわふわアニメーション
	floatTime += 1.0f / 30.0f;

	ghostPos.y = -0.5f + sinf(floatTime * 2.0f) * 0.1f;
	ghostPos.x = sinf(floatTime * 0.5f) * 0.2f;

	ghostObj->SetTranslate(ghostPos);

	ghostObj->Update();


	// --- フェードアウト中 ---
	if (isFadingOut) {
		fadeAlpha += 1.0f / (60.0f*0.8f);
		if (fadeAlpha >= 1.0f) {
			fadeAlpha = 1.0f;
			finished = true;
		}
		fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
		return;
	}

	// --- カーソルによるUI選択 ---
	hoveredButton = nullptr;
	if (gameStartButton.InCursor()) hoveredButton = &gameStartButton;
	else if (settingButton.InCursor()) hoveredButton = &settingButton;
	else if (ruleButton.InCursor()) hoveredButton = &ruleButton;
	else if (finishButton.InCursor()) hoveredButton = &finishButton;

	// カーソル移動で点滅リセット
	if (hoveredButton != prevHoveredButton) {
		blinkTimer = 0.0f;
		prevHoveredButton = hoveredButton;
		//Audio::GetInstance()->Play("click"); // クリック音再生
	}

	// --- 点滅アニメーション更新 ---
	blinkTimer += 1.0f / 60.0f;
	float blinkAlpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

	// --- 十字キー／コントローラー操作（カーソルがないときだけ） ---
	if (!hoveredButton) {
		prevSelectedIndex = selectedIndex;

		if (!inputLocked) {
			// 十字キー
			if (input->TriggerKey(DIK_DOWN) || input->TriggerXButton(DPad::Down)) {
				Audio::GetInstance()->Play("click"); // クリック音再生
				selectedIndex = (selectedIndex + 1) % buttonCount;
				inputLocked = true;
			}
			else if (input->TriggerKey(DIK_UP) || input->TriggerXButton(DPad::Up)) {
				Audio::GetInstance()->Play("click"); // クリック音再生
				selectedIndex = (selectedIndex - 1 + buttonCount) % buttonCount;
				inputLocked = true;
			}

			// 左右キー
			else if (input->TriggerKey(DIK_LEFT) || input->TriggerXButton(DPad::Left) ||
				input->TriggerKey(DIK_RIGHT) || input->TriggerXButton(DPad::Right)) {
				Audio::GetInstance()->Play("click"); // クリック音再生
				if (selectedIndex == 0) { selectedIndex = 2; }       // ゲーム → チュートリアル
				else if (selectedIndex == 2) { selectedIndex = 0; }   // チュートリアル → ゲーム
				else if (selectedIndex == 1) { selectedIndex = 3; }   // 設定 → 終了
				else if (selectedIndex == 3) { selectedIndex = 1; }   // 終了 → 設定
				inputLocked = true;
			}
		}
		else {
			if (!input->TriggerXButton(DPad::Up) &&
				!input->TriggerXButton(DPad::Down) &&
				!input->TriggerXButton(DPad::Left) &&
				!input->TriggerXButton(DPad::Right)) {
				inputLocked = false;
			}
		}

		// 選択変更があったら点滅リセット
		if (selectedIndex != prevSelectedIndex) {
			blinkTimer = 0.0f;
		}
	}

	// --- Enterキー / Aボタンによる決定処理 ---
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::Y)) {

		Audio::GetInstance()->Play("click"); // クリック音再生

		if (hoveredButton) {
			// カーソルが乗っているUIを決定
			if (hoveredButton == &gameStartButton) {
				goToGame = true;
				isFadingOut = true;
				fadeAlpha = 0.0f;
				fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
				return;
			}
			else if (hoveredButton == &settingButton) {
				goToSetting = true;
				finished = true;
			}
			else if (hoveredButton == &ruleButton) {
				goToRule = true;
				finished = true;
			}
			else if (hoveredButton == &finishButton) {
				PostQuitMessage(0);
				return;
			}
		}
		else {
			// カーソルがどこにも乗っていない → selectedIndexで決定
			switch (selectedIndex) {
			case 0:
				goToGame = true;
				isFadingOut = true;
				fadeAlpha = 0.0f;
				fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
				return;
			case 1:
				goToSetting = true;
				finished = true;
				break;
			case 2:
				goToRule = true;
				finished = true;
				break;
			case 3:
				PostQuitMessage(0);
				return;
			}
		}
	}


	// --- マウスクリックでも遷移対応 ---
	if (gameStartButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		goToGame = true;
		isFadingOut = true;
		fadeAlpha = 0.0f;
		fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
		return;
	}
	if (settingButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		goToSetting = true;
		finished = true;
	}
	if (ruleButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		goToRule = true;
		finished = true;
	}
	if (finishButton.OnButton()) {
		Audio::GetInstance()->Play("click"); // クリック音再生
		PostQuitMessage(0);
		return;
	}

	// --- 全ボタン透明度初期化 ---
	gameStartButton.SetSpriteAlpha(1.0f);
	settingButton.SetSpriteAlpha(1.0f);
	ruleButton.SetSpriteAlpha(1.0f);
	finishButton.SetSpriteAlpha(1.0f);

	// --- 点滅処理：カーソル優先 ---
	if (hoveredButton) {
		hoveredButton->SetSpriteAlpha(blinkAlpha);
	}
	else {
		switch (selectedIndex) {
		case 0: gameStartButton.SetSpriteAlpha(blinkAlpha); break;
		case 1: settingButton.SetSpriteAlpha(blinkAlpha); break;
		case 2: ruleButton.SetSpriteAlpha(blinkAlpha); break;
		case 3: finishButton.SetSpriteAlpha(blinkAlpha); break;
		}
	}


}



/// <summary>
/// 
/// </summary>
void Title::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();
	// スプライト
	if (titleSprite) titleSprite->Draw();

	Object3dBase::GetInstance()->ShaderDraw();
	// タイトルブロック
	if (titleBlockObj) titleBlockObj->Draw();

	if (ghostObj) {
		ghostObj->Draw();
	}

	gameStartButton.Draw();
	settingButton.Draw();
	ruleButton.Draw();
	finishButton.Draw();

	// フェード
	if (fadeSprite) {
		fadeSprite->Draw();
	}




}

void Title::Finalize() {


	Audio::GetInstance()->StopAll();


	delete ghostObj;
	ghostObj = nullptr;


	delete titleBlockObj;
	titleBlockObj = nullptr;


	delete titleSprite;
	titleSprite = nullptr;

	if (fadeSprite) {
		delete fadeSprite;
		fadeSprite = nullptr;
	}
}
