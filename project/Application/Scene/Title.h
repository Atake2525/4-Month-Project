#pragma once

#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include <vector>

#include "UI.h"
#include "Camera.h"
#include "Object3d.h"
#include "ModelManager.h"


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

	// 各ボタンが押されたかどうか
	bool IsGameStartSelected() const { return goToGame; }
	bool IsSettingSelected() const { return goToSetting; }
	bool IsRuleSelected() const { return goToRule; }

private:
	Camera* camera = nullptr;

	Sprite* titleSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;


	// UIボタン用
	UI gameStartButton;
	UI settingButton;
	UI ruleButton;
	UI finishButton;

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

	UI* hoveredButton = nullptr;      // 現在マウスカーソルが乗っているUI

	int selectedIndex = 0; // 0:ゲーム 1:設定 2:ルール 3:終了
	int prevSelectedIndex = -1;
	int buttonCount = 4;   // ボタン数
	bool inputLocked = false; // キーの連続押し防止

	Object3d* ghostObj = nullptr;
	Vector3 ghostPos = { 0.0f, 3.0f, 0.0f };
	float floatTime = 0.0f;

	Object3d* titleBlockObj = nullptr;
	Transform blockTransform{};

};
