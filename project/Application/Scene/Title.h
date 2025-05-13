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



};
