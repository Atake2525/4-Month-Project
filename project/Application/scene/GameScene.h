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
#include "UI.h"
#include "Audio.h"

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
	void Initialize(int stage);

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

	//ポーズ画面の表示
	bool ShouldRestart() const { return goToRestart; }
	bool ShouldReturnToTitle() const { return goToTitle; }

	//ポーズ画面の表示
	void PauseUpdate();

	int StarResult() const { return Result; }
private:
	Object3d* object3d = nullptr;

	Object3d* sky = nullptr;

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

	UI* button = nullptr;

	bool showCursor = false;

	LightBlock* lightBlock = nullptr;
	switchLight* lightSwitch = nullptr;
	Transform switchTransform;

	SoundData soundData;


	// ポーズ用
	bool isPaused = false;
	bool tabReleased = true;  // TABキーを離したかどうか
	UI resumeButton;
	UI restartButton;
	UI returnToTitleButton;
	bool goToRestart = false;
	bool goToTitle = false;
	//ポーズ画面の背景
	Sprite* pauseBg = nullptr;
	//ESCキーを押したときのヒント表示用
	Sprite* escHintSprite = nullptr;

	//UIの点滅用
	float blinkTimer = 0.0f;
	UI* prevHoveredButton = nullptr;  // 前フレームでカーソルが乗っていたボタン

	// フェード用
	Sprite* fadeSprite = nullptr;       // 黒フェード用スプライト
	bool isFadingOut = false;          // フェード開始フラグ
	bool isFadingIn = false; // フェードインを開始したか
	float fadeAlpha = 0.0f;


	UI* hoveredPauseButton = nullptr;
	UI* prevHoveredPauseButton = nullptr;

	int pauseSelectedIndex = 0;
	int prevPauseSelectedIndex = -1;
	int pauseButtonCount = 3;

	float pauseBlinkTimer = 0.0f;
	bool pauseInputLocked = false;

	// 星アイコンのスプライト
	Sprite* starIcon = nullptr;
	std::vector<Sprite*> starIcons;  // 星のUIを最大3つ分持つ



	//クリア時に星の取得情報を送るよう
	int Result = 0;
	int stage_ = 0;;
};

