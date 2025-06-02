#include "Setting.h"
#include "TextureManager.h"
#include <dinput.h>
#include "Audio.h"

void Setting::Initialize()
{

	// クリック音読み込み
	// bgm読み込み

	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// テクスチャ読み込み
	//TextureManager::GetInstance()->LoadTexture("Resources/Sprite/setting.png");
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/set.png");
	settingSprite = new Sprite();
	settingSprite->Initialize("Resources/Sprite/scene/set.png");

	Audio::GetInstance()->Play("goast", true); // ループ再生

}

void Setting::Update()
{
	input->Update();

	// SPACEキーでタイトル終了
	if (input->TriggerKey(DIK_RETURN)|| input->TriggerButton(Controller::Y)) {
		Audio::GetInstance()->Play("click"); // クリック音再生
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
	Audio::GetInstance()->StopAll();

	if (settingSprite) {
		delete settingSprite;
		settingSprite = nullptr;
	}
}
