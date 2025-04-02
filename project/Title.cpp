#include "Title.h"
#include "TextureManager.h"
#include <DirectXBase.h>

void Title::Initialize(DirectXBase* dxc) {

	directX = dxc;
	input = Input::GetInstance();

	// MyGame などの DirectXBase 
	SpriteBase::GetInstance()->Initialize(directX);

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
	titleSprite = nullptr;

}
