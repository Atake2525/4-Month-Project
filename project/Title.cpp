#include "Title.h"
#include "TextureManager.h"

void Title::Initialize() {

	input = Input::GetInstance();

	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/title.png");

	titleSprite = new Sprite();
	titleSprite->Initialize("Resources/Sprite/title.png"); //
	titleSprite->SetPosition({ 0.0f, 0.0f });


}

void Title::Update() {

	titleSprite->Update();

	if (input->TriggerKey(DIK_RETURN)) {
		finished = true;
	}
}

void Title::Draw() {

	SpriteBase::GetInstance()->ShaderDraw();
	titleSprite->Draw();
}

void Title::Finalize() {
	delete titleSprite;

}