#include "Title.h"
#include "TextureManager.h"
#include <dinput.h>

void Title::Initialize() {
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/title.png");

	// スプライト生成
	titleSprite = new Sprite();
	titleSprite->Initialize("Resources/Sprite/title.png");


}

void Title::Update() {
	input->Update();

	// Enterキーでタイトル終了（ルール画面へ）
	if (input->TriggerKey(DIK_RETURN)) {
		finished = true;
	}
}

void Title::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();

	if (titleSprite) {
		titleSprite->Draw();
	}
}

void Title::Finalize() {
	if (titleSprite) {
		delete titleSprite;
		titleSprite = nullptr;
	}
}
