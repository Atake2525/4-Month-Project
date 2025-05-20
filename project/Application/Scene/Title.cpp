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

// 点滅タイマー更新
	blinkTimer += 1.0f / 60.0f;
	float alpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

	// UIボタンに透明度を適用（SetSpriteAlphaがUIにある前提）
	gameStartButton.SetSpriteAlpha(alpha);
	settingButton.SetSpriteAlpha(alpha);
	ruleButton.SetSpriteAlpha(alpha); 

	//// UIボタンに透明度を適用（SetSpriteAlphaがUIにある前提）
	//if (gameStartButton.OnButton()) {
	//	gameStartButton.SetSpriteAlpha(alpha);
	//}
	//else {
	//	gameStartButton.SetSpriteAlpha(1.0f);
	//}
	//if (settingButton.OnButton()) {
	//	settingButton.SetSpriteAlpha(alpha);
	//}
	//else {
	//	settingButton.SetSpriteAlpha(1.0f);
	//}
	//if (ruleButton.OnButton()) {
	//	ruleButton.SetSpriteAlpha(alpha);
	//}
	//else {
	//	ruleButton.SetSpriteAlpha(1.0f);
	//}

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
