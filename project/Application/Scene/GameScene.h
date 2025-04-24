#include "Object3d.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "Camera.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Input.h"
#include "WireFrameObjectBase.h"
#include "Sprite.h"
#include "AABB.h"
#include "Player.h"
#include "Button.h"
#include "LightBlock.h"

#include "Star.h"
#include "starResult.h"
#include "Goal.h"
#include"LightBlock.h"
#include"switchLight.h"

#pragma once

class GameScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	const bool& isFinished() const { return finished; }

private:
	Object3d* object3d = nullptr;

	Sprite* sprite = nullptr;

	Camera* camera = nullptr;

	bool finished = false;

	Transform cameraTransform;
	Transform modelTransform;

	Input* input = nullptr;

	AABB aabb;

	bool enableLighting = true;

	Player* player = nullptr;


	Star* star = nullptr;
	starResult* starResultManager = nullptr;
	Goal* goal = nullptr;
	bool isGoal = false;

	Sprite* clearSprite = nullptr;

	Button* button = nullptr;

	bool showCursor = false;


	LightBlock* lightBlock = nullptr;
	switchLight* lightSwitch = nullptr;
	Transform switchTransform;

};

