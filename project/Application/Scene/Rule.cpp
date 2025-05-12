#include "Rule.h"
#include "TextureManager.h"
#include <dinput.h>

void Rule::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/rule.png");

	// スプライト生成
	ruleSprite = new Sprite();
	ruleSprite->Initialize("Resources/Sprite/rule.png");

}

void Rule::Update() {
	input->Update();

	// SPACEキーでタイトル終了（ゲーム画面へ）
	if (input->TriggerKey(DIK_SPACE)) {
		finished = true;
	}
}

void Rule::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();

	if (ruleSprite) {
		ruleSprite->Draw();
	}
}

void Rule::Finalize() {
	if (ruleSprite) {
		delete ruleSprite;
		ruleSprite = nullptr;
	}
}
