#include "GameClear.h"
#include "TextureManager.h"
#include <dinput.h>

void GameClear::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// �e�N�X�`���ǂݍ���
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/clear.png");

	// �X�v���C�g����
	clearSprite = new Sprite();
	clearSprite->Initialize("Resources/Sprite/clear.png");

}

void GameClear::Update()
{
	input->Update();

	// Enter�L�[�Ń^�C�g���I���i�^�C�g����ʂցj
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
