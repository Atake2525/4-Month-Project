#include "Setting.h"
#include "TextureManager.h"
#include <dinput.h>
#include "Audio.h"

void Setting::Initialize()
{

	// クリック音読み込み
	Audio::GetInstance()->LoadMP3("Resources/Sound/mouse/click.mp3", "click", 1.0f); // 音量1.0f
	// bgm読み込み
	Audio::GetInstance()->LoadMP3("Resources/Sound/scene/goast.mp3", "goast", 1.0f); // 音量1.0f

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
	if (settingSprite) {
		delete settingSprite;
		settingSprite = nullptr;
	}
}
