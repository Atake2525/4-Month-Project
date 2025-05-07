// GameClear.h
#pragma once
#include "SpriteBase.h"
#include "Sprite.h"
#include "Input.h"

class GameClear {
public:
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    const bool& isFinished() const { return finished; }

private:
    Sprite* clearSprite = nullptr;
    Input* input = nullptr;
    bool finished = false;

};