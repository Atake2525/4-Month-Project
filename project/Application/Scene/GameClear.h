#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"

class GameClear
{
public:
	// 初期化
	void Initialize(int result);

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了処理
	void Finalize();

	// ゲーム開始フラグ
	const bool& isFinished() const { return finished; }

private:
	Sprite* clearSprite = nullptr;
	Sprite* starSprite = nullptr;
	Sprite* starSprite2 = nullptr;
	Sprite* starSprite3 = nullptr;
	Input* input = nullptr;
	bool finished = false;
	int starResult = 0;
};

