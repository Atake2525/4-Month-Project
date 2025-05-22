#include "SetScene.h"
#include "TextureManager.h"
#include <dinput.h>

void Setting::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/setting.png");
	settingSprite = new Sprite();
	settingSprite->Initialize("Resources/Sprite/scene/setting.png");
}

void Setting::Update()
{
	input->Update();

	// SPACEキーでタイトル終了
	if (input->TriggerKey(DIK_SPACE)) {
		finished = true;
	}
}

void Setting::Draw()
{
	SpriteBase::GetInstance()->ShaderDraw();

	if (settingSprite) {
		settingSprite->Draw();
	}
}

void Setting::Finalize()
{
	if (settingSprite) {
		delete settingSprite;
		settingSprite = nullptr;
	}
}
