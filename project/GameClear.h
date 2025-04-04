// GameClear.h
#pragma once
#include "SpriteBase.h"
#include "Sprite.h"
#include "Input.h"
#include "DirectXBase.h"

class GameClear {
public:
    void Initialize(DirectXBase* dxc);
    void Update();
    void Draw();
    void Finalize();

    const bool& isFinished() const { return finished; }

private:
    DirectXBase* directX;
    Sprite* clearSprite = nullptr;
    Input* input = nullptr;
    bool finished = false;
};
