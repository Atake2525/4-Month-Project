#include "Rule.h"
#include "TextureManager.h"
#include <dinput.h>

void Rule::Initialize()
{
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	// �e�N�X�`���ǂݍ���
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/rule.png");

	// �X�v���C�g����
	ruleSprite = new Sprite();
	ruleSprite->Initialize("Resources/Sprite/rule.png");

}

void Rule::Update() {
	input->Update();

	// SPACE�L�[�Ń^�C�g���I���i�Q�[����ʂցj
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
