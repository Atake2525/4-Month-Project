#pragma once

#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include <vector>
#include "UI.h"

class StageSelect {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	// ゲーム開始フラグ
	const bool& isFinished() const { return finished; }

	// 各ボタンが押されたかどうか
	bool IsGameStartSelected() const { return goToStage; }
	bool IsTitleSelected() const { return goToTitle; }
	int SelectStageNumber() const { return buttonCount; }
private:
	Sprite* StageSelectSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;

	// UIボタン用
	UI stageSelct;
	UI stageSelct2;
	UI stageSelct3;
	UI stageSelct4;
	UI titleButton;

	// 押下フラグ
	bool goToStage = false;
	bool goToTitle = false;

	//文字点滅用
	float blinkTimer = 0.0f;
	UI* prevHoveredButton = nullptr;  // 前フレームでカーソルが乗っていたボタン

	// フェード用
	Sprite* fadeSprite = nullptr;       // 黒フェード用スプライト
	bool isFadingOut = false;          // フェード開始フラグ
	bool isFadingIn = false; // フェードインを開始したか
	float fadeAlpha = 0.0f;

	UI* hoveredButton = nullptr;      // 現在マウスカーソルが乗っているUI

	int selectedIndex = 0; // 0:ステージ 1:ステージ2 2:1ステージ3 3:1ステージ4 4:タイトル
	int prevSelectedIndex = -1;
	int buttonCount = 5;   // ボタン数
	bool inputLocked = false; // キーの連続押し防止
};