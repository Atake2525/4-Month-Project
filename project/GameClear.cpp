// GameClear.cpp
#include "GameClear.h"
#include "TextureManager.h"
#include "Sprite.h"

void GameClear::Initialize() {
    input = Input::GetInstance();

    TextureManager::GetInstance()->LoadTexture("Resources/Sprite/game_clear.png");

    clearSprite = new Sprite();
    clearSprite->Initialize("Resources/Sprite/game_clear.png");
    clearSprite->SetPosition({ 0.0f, 0.0f });
}

void GameClear::Update() {
    clearSprite->Update();

    if (input->TriggerKey(DIK_RETURN)) {
        finished = true; // エンターキーで終了
    }
}

void GameClear::Draw() {
    SpriteBase::GetInstance()->ShaderDraw();
    clearSprite->Draw();
}

void GameClear::Finalize() {

    delete clearSprite;
}