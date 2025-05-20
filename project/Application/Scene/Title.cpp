#include "Title.h"
#include "TextureManager.h"
#include <dinput.h>

void Title::Initialize() {
    input = Input::GetInstance();
    input->ShowMouseCursor(true);

    // タイトル画像
    TextureManager::GetInstance()->LoadTexture("Resources/Sprite/scene/title.png");
    titleSprite = new Sprite();
    titleSprite->Initialize("Resources/Sprite/scene/title.png");

    // UIボタン
    gameStartButton.CreateButton({ 200, 300 }, Origin::Center, "Resources/Sprite/titleUI/startButton.png");
    settingButton.CreateButton({ 200, 400 }, Origin::Center, "Resources/Sprite/titleUI/settingButton.png");
    ruleButton.CreateButton({ 200, 500 }, Origin::Center, "Resources/Sprite/titleUI/ruleButton.png");

    // 黒フェードスプライト
    fadeSprite = new Sprite();
    fadeSprite->Initialize("Resources/black1x1.png");
    fadeSprite->SetPosition({ 0.0f, 0.0f });
    fadeSprite->SetScale({ 1280.0f, 720.0f });
    fadeSprite->SetAnchorPoint({ 0.0f, 0.0f });
    fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f }); // 初期透明
}

void Title::Update() {
    input->Update();

    // フェードアウト進行中（ボタン押下後）
    if (isFadingOut) {
        fadeAlpha += 1.0f / (60.0f); // 3秒で暗転
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            finished = true; // 完全に黒くなったらシーン切り替え
        }
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }

    // UIの点滅処理
    UI* hoveredButton = nullptr;
    if (gameStartButton.InCursor()) hoveredButton = &gameStartButton;
    else if (settingButton.InCursor()) hoveredButton = &settingButton;
    else if (ruleButton.InCursor()) hoveredButton = &ruleButton;

    if (hoveredButton != prevHoveredButton) {
        blinkTimer = 0.0f;
        prevHoveredButton = hoveredButton;
    }

    blinkTimer += 1.0f / 60.0f;
    float blinkAlpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

    gameStartButton.SetSpriteAlpha(1.0f);
    settingButton.SetSpriteAlpha(1.0f);
    ruleButton.SetSpriteAlpha(1.0f);
    if (hoveredButton) hoveredButton->SetSpriteAlpha(blinkAlpha);

    // ボタン押下 → フェードアウト開始
    if (gameStartButton.OnButton()) {
        goToGame = true;
        isFadingOut = true;           // フェードアウト開始
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }
    if (settingButton.OnButton()) {
        goToSetting = true;
        finished = true;
    }
    if (ruleButton.OnButton()) {
        goToRule = true;
        finished = true;
    }
}

void Title::Draw() {
    SpriteBase::GetInstance()->ShaderDraw();

    if (titleSprite) titleSprite->Draw();

    gameStartButton.Draw();
    settingButton.Draw();
    ruleButton.Draw();

    if (fadeSprite) {
        fadeSprite->Draw();
    }
}

void Title::Finalize() {
    if (titleSprite) {
        delete titleSprite;
        titleSprite = nullptr;
    }
    if (fadeSprite) {
        delete fadeSprite;
        fadeSprite = nullptr;
    }
}
