#pragma once

#include "SpriteBase.h"
#include "Sprite.h"
#include"DirectXBase.h"

#include "Input.h"

class Title {
public:

	void Initialize(DirectXBase* dxc);
	void Update();
	void Draw();
	void Finalize();

	const bool& isFinished() const { return finished; }


private:
	DirectXBase* directX;
	Sprite* titleSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;


};


