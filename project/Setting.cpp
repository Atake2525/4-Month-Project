#include "Setting.h"
#include "TextureManager.h"
#include <dinput.h>

void Setting::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// �e�N�X�`���ǂݍ���
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/setting.png");

	// �X�v���C�g����
	settingSprite = new Sprite();
	settingSprite->Initialize("Resources/Sprite/setting.png");
}

void Setting::Update()
{
	input->Update();

	// SPACE�L�[�Ń^�C�g���I���i�Q�[����ʂցj
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
