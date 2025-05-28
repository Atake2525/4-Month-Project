#include "Title.h"
#include "TextureManager.h"
#include <dinput.h>
#include "Object3dBase.h"

void Title::Initialize() {

	camera = new Camera();

	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	Object3dBase::GetInstance()->SetDefaultCamera(camera);


	// -------------------------
	// お化け初期化
	// -------------------------

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Player", "Player.obj");

	ghostObj = new Object3d();
	ghostObj->Initialize();
	ghostObj->SetModel("Player.obj");
	ghostObj->SetScale({ 0.2f, 0.2f, 0.2f });
	ghostObj->SetCamera(camera);

	ghostPos = { -1.5f, 1.0f, 5.0f };
	ghostObj->SetTranslate(ghostPos);
	ghostObj->Update();


	// -------------------------
	// タイトル画像とボタン
	// -------------------------
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/title.png");
	titleSprite = new Sprite();
	titleSprite->Initialize("Resources/Sprite/scene/title.png");

	gameStartButton.CreateButton({ 200, 350 }, Origin::Center, "Resources/Sprite/titleUI/startButton.png");
	settingButton.CreateButton({ 200, 550 }, Origin::Center, "Resources/Sprite/titleUI/settingButton.png");
	ruleButton.CreateButton({ 900, 350 }, Origin::Center, "Resources/Sprite/titleUI/tyu-to.png");
	finishButton.CreateButton({ 900, 550 }, Origin::Center, "Resources/Sprite/titleUI/finish.png");

	// -------------------------
	// フェードスプライト
	// -------------------------
	fadeSprite = new Sprite();
	fadeSprite->Initialize("Resources/black1x1.png");
	fadeSprite->SetPosition({ 0.0f, 0.0f });
	fadeSprite->SetScale({ 1280.0f, 720.0f });
	fadeSprite->SetAnchorPoint({ 0.0f, 0.0f });
	fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}


void Title::Update() {
	input->Update();

	// ふわふわアニメーション
	floatTime += 1.0f / 60.0f;
	ghostPos.y = 1.0f + sinf(floatTime * 2.0f) * 0.3f;
	ghostObj->SetTranslate(ghostPos);
	ghostObj->Update();


	// --- フェードアウト中 ---
	if (isFadingOut) {
		fadeAlpha += 1.0f / (60.0f * 3.0f);
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
	}

	// --- 点滅アニメーション更新 ---
	blinkTimer += 1.0f / 60.0f;
	float blinkAlpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

	// --- 十字キー／コントローラー操作（カーソルがないときだけ） ---
	if (!hoveredButton) {
		prevSelectedIndex = selectedIndex;  // 前回の選択を保存

		if (!inputLocked) {
			if (input->TriggerKey(DIK_DOWN) || input->TriggerXButton(DPad::Down)) {
				selectedIndex = (selectedIndex + 1) % buttonCount;
				inputLocked = true;
			}
			else if (input->TriggerKey(DIK_UP) || input->TriggerXButton(DPad::Up)) {
				selectedIndex = (selectedIndex - 1 + buttonCount) % buttonCount;
				inputLocked = true;
			}
		}
		else {
			if (!input->TriggerXButton(DPad::Up) && !input->TriggerXButton(DPad::Down)) {
				inputLocked = false;
			}
		}

		// 選択変更があったら点滅リセット
		if (selectedIndex != prevSelectedIndex) {
			blinkTimer = 0.0f;
		}
	}

	// --- Enterキー / Aボタンによる決定処理 ---
	if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::A)) {
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
		goToGame = true;
		isFadingOut = true;
		fadeAlpha = 0.0f;
		fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
		return;
	}
	if (settingButton.OnButton()) {
		goToSetting = true;
		finished = true;
	}
	if (ruleButton.OnButton()) {
		goToRule = true;
		finished = true;
	}
	if (finishButton.OnButton()) {
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
	titleSprite->Draw();
	gameStartButton.Draw();
	settingButton.Draw();
	ruleButton.Draw();
	finishButton.Draw();

	Object3dBase::GetInstance()->ShaderDraw();
	// 3Dモデル
	ghostObj->Draw();


	// フェード
	if (fadeSprite) {
		fadeSprite->Draw();
	}





}

void Title::Finalize() {


	delete ghostObj;
	ghostObj = nullptr;


	delete titleSprite;
	titleSprite = nullptr;

	if (fadeSprite) {
		delete fadeSprite;
		fadeSprite = nullptr;
	}
}
