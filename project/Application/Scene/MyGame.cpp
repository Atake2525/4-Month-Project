#include "MyGame.h"

void MyGame::Initialize() {

	FrameWork::Initialize();

#pragma region 基盤システムの初期化

	WinApp::GetInstance()->Initialize(1280, 720, WindowMode::Window, L"3143_光沢");

	directxBase = new DirectXBase();
	directxBase->Initialize();

	SpriteBase::GetInstance()->Initialize(directxBase);

	Object3dBase::GetInstance()->Initialize(directxBase);

	WireFrameObjectBase::GetInstance()->Initialize(directxBase);

	ModelBase::GetInstance()->Initialize(directxBase);

	TextureManager::GetInstance()->Initialize(directxBase);

	ModelManager::GetInstance()->Initialize(directxBase);

	Audio::GetInstance()->Initialize();

	Light::GetInstance()->Initialize(directxBase);

	Input::GetInstance()->Initialize();
	Input::GetInstance()->ShowMouseCursor(true);

	//// ↓---- シーンの初期化 ----↓ ////

	 // タイトルシーンの初期化
	title = new Title();
	title->Initialize();

	// ルールシーンの初期化
	rule = new Rule();
	rule->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	// ゲームクリアシーンの初期化
	gameClear = new GameClear();
	gameClear->Initialize();

	//// ↑---- シーンの初期化 ----↑ ////
}

void MyGame::Update() {
	FrameWork::Update();

	if (WinApp::GetInstance()->ProcessMessage()) {
		finished = true;
	}
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 入力の更新
	switch (currentScene) {
		// タイトルシーンの更新
	case Scene::Title:
		title->Update();
		if (title->isFinished()) {
			// タイトル終了 → ルール開始
			title->Finalize();
			delete title;
			title = nullptr;
			// ルールシーンの初期化
			rule = new Rule();
			rule->Initialize();
			currentScene = Scene::Rule;
		}
		break;
		// ルールシーンの更新
	case Scene::Rule:
		rule->Update();
		if (rule->isFinished()) {
			// ルール終了 → ゲームへ
			rule->Finalize();
			delete rule;
			rule = nullptr;
			// ゲームシーンの初期化
			gameScene = new GameScene();
			gameScene->Initialize();
			currentScene = Scene::Game;
		}
		break;
		// ゲームシーンの更新
	case Scene::Game:
		gameScene->Update();
		if (gameScene->isFinished()) {
			// ゲーム終了 → ゲームクリアへ
			gameScene->Finalize();
			delete gameScene;
			gameScene = nullptr;
			// ゲームクリアシーンの初期化
			gameClear = new GameClear();
			gameClear->Initialize();
			currentScene = Scene::GameClear;
		}
		break;

		// ゲームクリアシーンの更新
	case Scene::GameClear:
		gameClear->Update();
		if (gameClear->isFinished()) {
			// ゲームクリア終了 → タイトルへ
			gameClear->Finalize();
			delete gameClear;
			gameClear = nullptr;
			// タイトルシーンの初期化
			title = new Title();
			title->Initialize();
			currentScene = Scene::Title;
		}
		break;



	}


}

void MyGame::Draw() {

	// ImGuiの内部コマンドを生成する
	ImGui::Render();

	directxBase->PreDraw();

	switch (currentScene) {
		// タイトルシーンの描画
	case Scene::Title:
		title->Draw();
		break;
		// ルールシーンの描画
	case Scene::Rule:
		rule->Draw();
		break;
		// ゲームシーンの描画
	case Scene::Game:
		gameScene->Draw();
		break;
		// ゲームクリアシーンの描画
	case Scene::GameClear:
		gameClear->Draw();
		break;
	}

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directxBase->GetCommandList().Get());

	directxBase->PostDraw();
}

void MyGame::Finalize() {

	WinApp::GetInstance()->Finalize();

	directxBase->Finalize();
	delete directxBase;

	SpriteBase::GetInstance()->Finalize();

	Object3dBase::GetInstance()->Finalize();

	WireFrameObjectBase::GetInstance()->Finalize();

	ModelBase::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize();

	Light::GetInstance()->Finalize();

	Input::GetInstance()->Finalize();

	//// ↓---- シーンの解放 ----↓ ////

	// タイトルシーンの解放
	if (title) {
		title->Finalize();
		delete title;
	}
	// ルールシーンの解放
	if (rule) {
		rule->Finalize();
		delete rule;
	}
	// ゲームシーンの解放
	if (gameScene) {
		gameScene->Finalize();
		delete gameScene;
	}
	// ゲームクリアシーンの解放
	if (gameClear) {
		gameClear->Finalize();
		delete gameClear;
	}

	//// ↑---- シーンの解放 ----↑ ////

	FrameWork::Finalize();
}