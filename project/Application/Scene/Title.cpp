#include "Title.h"
#include "TextureManager.h"
#include <dinput.h>

void Title::Initialize() {
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/title.png");
	titleSprite = new Sprite();
	titleSprite->Initialize("Resources/Sprite/scene/title.png");

	// 各ボタンの画像をロード
	gameStartButton.CreateButton({ 200, 300 }, Origin::Center, "Resources/Sprite/titleUI/startButton.png");
	settingButton.CreateButton({ 200, 400 }, Origin::Center, "Resources/Sprite/titleUI/settingButton.png");
	ruleButton.CreateButton({ 200, 500 }, Origin::Center, "Resources/Sprite/titleUI/ruleButton.png");

}

void Title::Update() {
	input->Update();

	// ボタンの alpha を設定
	UI* hoveredButton = nullptr;
	// どのボタンにカーソルが当たっているか取得
	if (gameStartButton.InCursor()) {
		hoveredButton = &gameStartButton;
	}
	else if (settingButton.InCursor()) {
		hoveredButton = &settingButton;
	}
	else if (ruleButton.InCursor()) {
		hoveredButton = &ruleButton;

	}
	// 前回と違うボタンに乗ったらタイマーリセット
	if (hoveredButton != prevHoveredButton) {
		blinkTimer = 0.0f;
		prevHoveredButton = hoveredButton; 
	}

	// 点滅タイマー更新
	blinkTimer += 1.0f / 60.0f;
	float alpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

	// 最初にすべてのボタンの alpha を 1.0f に戻
	gameStartButton.SetSpriteAlpha(1.0f);
	settingButton.SetSpriteAlpha(1.0f);
	ruleButton.SetSpriteAlpha(1.0f);

	// 対象ボタンだけ点滅
	if (hoveredButton) {
		hoveredButton->SetSpriteAlpha(alpha);
	}

	// スプライト更新
	if (gameStartButton.OnButton()) {
		goToGame = true;
	}
	if (settingButton.OnButton()) {
		goToSetting = true;
	}
	if (ruleButton.OnButton()) {
		goToRule = true;
	}

	// とりあえずどれか押されたら終了
	if (goToGame || goToSetting || goToRule) {
		finished = true;
	}



}

void Title::Draw() {
	SpriteBase::GetInstance()->ShaderDraw();

	if (titleSprite) {
		titleSprite->Draw();
	}
	gameStartButton.Draw();
	settingButton.Draw();
	ruleButton.Draw();

}

void Title::Finalize() {
	if (titleSprite) {
		delete titleSprite;
		titleSprite = nullptr;
	}
}
