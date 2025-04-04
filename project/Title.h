#pragma once

#include "SpriteBase.h"
#include "Sprite.h"

#include "Input.h"

class Title {
public:

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	const bool& isFinished() const { return finished; }


private:
	Sprite* titleSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;


};


