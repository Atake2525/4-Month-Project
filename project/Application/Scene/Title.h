#pragma once

#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include <vector>

#include "UI.h"

class Title
{
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

	// 各ボタンが押されたかどうか取得
	bool IsGameStartSelected() const { return goToGame; }
	bool IsSettingSelected() const { return goToSetting; }
	bool IsRuleSelected() const { return goToRule; }

private:
	Sprite* titleSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;


	// UIボタン用
	UI gameStartButton;
	UI settingButton;
	UI ruleButton;

	// 押下フラグ
	bool goToGame = false;
	bool goToSetting = false;
	bool goToRule = false;

	//文字点滅用
	float blinkTimer = 0.0f;
	UI* prevHoveredButton = nullptr;  // 前フレームでカーソルが乗っていたボタン

	// フェード用
	Sprite* fadeSprite = nullptr;       // 黒フェード用スプライト
	bool isFadingOut = false;          // フェード開始フラグ
	bool isFadingIn = false; // フェードインを開始したか
	float fadeAlpha = 0.0f;  


};
