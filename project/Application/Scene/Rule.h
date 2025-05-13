#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"

class Rule
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

private:
	Sprite* ruleSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;
};
