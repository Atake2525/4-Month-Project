#include "GameClear.h"
#include "TextureManager.h"
#include <dinput.h>

void GameClear::Initialize(int result)
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	starResult = result;

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Clear/ClearBG.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Clear/star.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/Clear/star2.png");
	// スプライト生成
	clearSprite = new Sprite();
	clearSprite->Initialize("Resources/Sprite/Clear/ClearBG.png");

	//星スプライト
	starSprite = new Sprite();
	starSprite->SetPosition({ 312.0f,296.0f });

	starSprite2 = new Sprite();
	starSprite2->SetPosition({ 576.0f,360.0f });

	starSprite3 = new Sprite();
	starSprite3->SetPosition({ 840.0f,296.0f });

	if (starResult == 3) {
		starSprite->Initialize("Resources/Sprite/Clear/star.png");
		starSprite2->Initialize("Resources/Sprite/Clear/star.png");
		starSprite3->Initialize("Resources/Sprite/Clear/star.png");
	}
	else if (starResult == 2) {
		starSprite->Initialize("Resources/Sprite/Clear/star.png");
		starSprite2->Initialize("Resources/Sprite/Clear/star.png");
		starSprite3->Initialize("Resources/Sprite/Clear/star2.png");
	}
	else if (starResult == 1) {
		starSprite->Initialize("Resources/Sprite/Clear/star.png");
		starSprite2->Initialize("Resources/Sprite/Clear/star2.png");
		starSprite3->Initialize("Resources/Sprite/Clear/star2.png");
	}
	else {
		starSprite->Initialize("Resources/Sprite/Clear/star2.png");
		starSprite2->Initialize("Resources/Sprite/Clear/star2.png");
		starSprite3->Initialize("Resources/Sprite/Clear/star2.png");
	}


}

void GameClear::Update()
{
	input->Update();

	// Enterキーでタイトル終了（タイトル画面へ）
	if (input->TriggerKey(DIK_RETURN) || input->PushButton(Controller::A)) {
		finished = true;
	}

}

void GameClear::Draw()
{
	SpriteBase::GetInstance()->ShaderDraw();

	if (clearSprite) {
		clearSprite->Draw();
	}

	starSprite->Draw();
	starSprite2->Draw();
	starSprite3->Draw();

}

void GameClear::Finalize()
{
	if (clearSprite) {
		delete clearSprite;
		clearSprite = nullptr;
	}

	delete starSprite;
	starSprite = nullptr;
}
