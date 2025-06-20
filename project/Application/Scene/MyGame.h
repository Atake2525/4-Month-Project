#include "FrameWork.h"
#include "D3DResourceLeakChecker.h"
#include "WinApp.h"
#include "DirectXBase.h"
#include "Input.h"
#include "SpriteBase.h"
#include "Object3dBase.h"
#include "ModelBase.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "WireFrameObjectBase.h"
#include "Light.h"
#include "Audio.h"

#include "algorithm"
#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"
#include "externels/DirectXTex/DirectXTex.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#include "Title.h"
#include "Rule.h"
#include "Setting.h"
#include "GameScene.h"
#include "GameClear.h"
#include "starResult.h"

#include "StageSelect.h"
#pragma once

class MyGame : public FrameWork {
public:
	// 初期化
	void Initialize() override;

	// 終了処理
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// ループ終了
	bool RoopOut() override { return finished; }
	

	// ↑までシーンの作成に必須

private:
	D3DResourceLeakChecker d3dResourceLeakChecker;

	// メンバ変数宣言

#pragma region 基盤システム

	DirectXBase* directxBase = nullptr;

	bool finished = false;

	Title* title = nullptr;
	Rule* rule = nullptr;
	Setting* setting = nullptr;
	GameScene* gameScene = nullptr;
	GameClear* gameClear = nullptr;
	StageSelect* stageSelect = nullptr;

	enum class Scene {
		Title,
		Rule,
		Setting,
		Select,
		Game,
		GameClear,
	};
	Scene currentScene = Scene::Title; // 初期シーンはTitle

	int Result = 0;
	int stage;

};