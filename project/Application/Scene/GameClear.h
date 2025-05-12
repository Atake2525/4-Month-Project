#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"

class GameClear
{
public:
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �I������
	void Finalize();

	// �Q�[���J�n�t���O
	const bool& isFinished() const { return finished; }

private:
	Sprite* clearSprite = nullptr;
	Input* input = nullptr;
	bool finished = false;

};

