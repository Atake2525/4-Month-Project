#include "StageSelect.h"
#include "TextureManager.h"
#include <dinput.h>
void StageSelect::Initialize() {
	input = Input::GetInstance();
	input->ShowMouseCursor(true);

	//背景
	TextureManager::GetInstance()->LoadTexture("Resources/Sprite/gameUI/SelectBG.png");
	StageSelectSprite = new Sprite();
	StageSelectSprite->Initialize("Resources/Sprite/gameUI/SelectBG.png");

	//UIボタン
	stageSelct.CreateButton({ 350,200 }, Origin::Center, "Resources/Sprite/number/1.png");
	stageSelct2.CreateButton({ 800,200 }, Origin::Center, "Resources/Sprite/number/2.png");
	stageSelct3.CreateButton({ 350,450 }, Origin::Center, "Resources/Sprite/number/3.png");
	stageSelct4.CreateButton({ 800,450}, Origin::Center, "Resources/Sprite/number/4.png");
	titleButton.CreateButton({ 50,50 }, Origin::Center, "Resources/Sprite/gameUI/Gametitle.png");

	// 黒フェードスプライト
	fadeSprite = new Sprite();
	fadeSprite->Initialize("Resources/black1x1.png");
	fadeSprite->SetPosition({ 0.0f, 0.0f });
	fadeSprite->SetScale({ 1280.0f, 720.0f });
	fadeSprite->SetAnchorPoint({ 0.0f, 0.0f });
	fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f }); // 初期透明
}

void StageSelect::Update() {
	input->Update();

	// --- フェードアウト中 ---
	if (isFadingOut) {
		fadeAlpha += 1.0f / (60.0f * 3.0f);
		if (fadeAlpha >= 1.0f) {
			fadeAlpha = 1.0f;
			finished = true;
		}
		fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
		return;
	}

	// --- カーソルによるUI選択 ---
	hoveredButton = nullptr;
	if (stageSelct.InCursor()) hoveredButton = &stageSelct;
	else if (stageSelct2.InCursor()) hoveredButton = &stageSelct2;
	else if (stageSelct3.InCursor()) hoveredButton = &stageSelct3;
	else if (stageSelct4.InCursor()) hoveredButton = &stageSelct4;
	else if (titleButton.InCursor()) hoveredButton = &titleButton;

	// カーソル移動で点滅リセット
	if (hoveredButton != prevHoveredButton) {
		blinkTimer = 0.0f;
		prevHoveredButton = hoveredButton;
	}

	// --- 点滅アニメーション更新 ---
	blinkTimer += 1.0f / 60.0f;
	float blinkAlpha = 0.5f + 0.5f * sinf(blinkTimer * 3.14f);

    // --- 十字キー／コントローラー操作（カーソルがないときだけ） ---
    if (!hoveredButton) {
        prevSelectedIndex = selectedIndex;  // 前回の選択を保存

        if (!inputLocked) {
            if (input->TriggerKey(DIK_DOWN) || input->TriggerXButton(DPad::Down)) {
                selectedIndex = (selectedIndex + 1) % buttonCount;
                inputLocked = true;
            }
            else if (input->TriggerKey(DIK_UP) || input->TriggerXButton(DPad::Up)) {
                selectedIndex = (selectedIndex - 1 + buttonCount) % buttonCount;
                inputLocked = true;
            }
        }
        else {
            if (!input->TriggerXButton(DPad::Up) && !input->TriggerXButton(DPad::Down)) {
                inputLocked = false;
            }
        }

        // 選択変更があったら点滅リセット
        if (selectedIndex != prevSelectedIndex) {
            blinkTimer = 0.0f;
        }
    }

    // --- Enterキー / Aボタンによる決定処理 ---
    if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(Controller::Y)) {
        if (hoveredButton) {
            // カーソルが乗っているUIを決定
            if (hoveredButton == &stageSelct) {

                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                buttonCount = 0;
                return;
            }
            else if (hoveredButton == &stageSelct2) {
               
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                buttonCount = 1;
                return;
            }
            else if (hoveredButton == &stageSelct3) {
               
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                buttonCount = 2;
                return;
            }
            else if (hoveredButton == &stageSelct4) {
               
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                buttonCount = 3;
                return;
            }
            else if (hoveredButton == &titleButton) {

                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;
            }
        }
        else {
            // カーソルがどこにも乗っていない → selectedIndexで決定
            switch (selectedIndex) {
            case 0:
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                buttonCount = 0;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;
            case 1:
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                buttonCount = 1;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;
            case 2:
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                buttonCount = 2;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;
            case 3:
                goToStage = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                buttonCount = 3;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;
            case 4:
                goToTitle = true;
                isFadingOut = true;
                fadeAlpha = 0.0f;
                buttonCount = 4;
                fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
                return;

            }
        }
    }

    // --- マウスクリックでも遷移対応 ---
    if (stageSelct.OnButton()) {
        goToStage = true;
        buttonCount = 0;
        isFadingOut = true;
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }
    if (stageSelct2.OnButton()) {
        goToStage = true;
        buttonCount = 1;
        isFadingOut = true;
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }
    if (stageSelct3.OnButton()) {
        goToStage = true;
        buttonCount = 2;
        isFadingOut = true;
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }
    if (stageSelct4.OnButton()) {
        goToStage = true;
        buttonCount = 3;
        isFadingOut = true;
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }
    if (titleButton.OnButton()) {
        goToTitle = true;
        isFadingOut = true;
        fadeAlpha = 0.0f;
        fadeSprite->SetColor({ 0.0f, 0.0f, 0.0f, fadeAlpha });
        return;
    }

    // --- 全ボタン透明度初期化 ---
    stageSelct.SetSpriteAlpha(1.0f);
    stageSelct2.SetSpriteAlpha(1.0f);
    stageSelct3.SetSpriteAlpha(1.0f);
    stageSelct4.SetSpriteAlpha(1.0f);
    titleButton.SetSpriteAlpha(1.0f);


    // --- 点滅処理：カーソル優先 ---
    if (hoveredButton) {
        hoveredButton->SetSpriteAlpha(blinkAlpha);
    }
    else {
        switch (selectedIndex) {
        case 0: stageSelct.SetSpriteAlpha(blinkAlpha); break;
        case 1: stageSelct2.SetSpriteAlpha(blinkAlpha); break;
        case 2: stageSelct3.SetSpriteAlpha(blinkAlpha); break;
        case 3: stageSelct4.SetSpriteAlpha(blinkAlpha); break;
        case 4: titleButton.SetSpriteAlpha(blinkAlpha); break;
        }
    }
}

void StageSelect::Draw() {
    SpriteBase::GetInstance()->ShaderDraw();

    if (StageSelectSprite) StageSelectSprite->Draw();

    stageSelct.Draw();
    stageSelct2.Draw();
    stageSelct3.Draw();
    stageSelct4.Draw();
    titleButton.Draw();

    if (fadeSprite) {
        fadeSprite->Draw();
    }
}

void StageSelect::Finalize() {
    if (StageSelectSprite) {
        delete StageSelectSprite;
        StageSelectSprite = nullptr;
    }
    if (fadeSprite) {
        delete fadeSprite;
        fadeSprite = nullptr;
    }
}

