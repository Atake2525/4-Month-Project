#include "GameClear.h"
#include "TextureManager.h"
#include <dinput.h>

void GameClear::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/clear.png");

	// スプライト生成
	clearSprite = new Sprite();
	clearSprite->Initialize("Resources/Sprite/scene/clear.png");

}

void GameClear::Update()
{
	input->Update();

	// Enterキーでタイトル終了（タイトル画面へ）
	if (input->TriggerKey(DIK_RETURN)) {
		finished = true;
	}

}

void GameClear::Draw()
{
	SpriteBase::GetInstance()->ShaderDraw();

	if (clearSprite) {
		clearSprite->Draw();
	}

}

void GameClear::Finalize()
{
	if (clearSprite) {
		delete clearSprite;
		clearSprite = nullptr;
	}
}
