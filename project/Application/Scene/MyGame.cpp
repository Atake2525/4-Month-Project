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
	TextureManager::GetInstance()->LoadTexture("Resources/Debug/white1x1.png");

	ModelManager::GetInstance()->Initialize(directxBase);

	Audio::GetInstance()->Initialize();

	Light::GetInstance()->Initialize(directxBase);

	Input::GetInstance()->Initialize();
	Input::GetInstance()->ShowMouseCursor(true);

	//ステージの読み込み
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Stage", "01Stage.obj", true);
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Stage2", "Stage2.obj", true);
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/stageTriangle", "stageTriangle.obj", true);
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj/Stage4", "stage4.obj", true);
	//// ↓---- シーンの初期化 ----↓ ////

	 // タイトルシーンの初期化
	title = new Title();
	title->Initialize();

	// ルールシーンの初期化
	rule = new Rule();
	rule->Initialize();

	// 設定シーンの初期化
	setting = new Setting();
	setting->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(stage);

	// ゲームクリアシーンの初期化
	gameClear = new GameClear();
	gameClear->Initialize(Result);

	//ステージセレクト
	stageSelect = new StageSelect();
	stageSelect->Initialize();

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

	case Scene::Title:
		title->Update();

		if (title->isFinished()) {

			if (title->IsGameStartSelected()) {
				// タイトル終了 → ステージセレクト
				title->Finalize();
				delete title;
				title = nullptr;

				stageSelect = new StageSelect();
				stageSelect->Initialize();
				stageSelect->Update();
				currentScene = Scene::Select;
			}
			else if (title->IsRuleSelected()) {
				// タイトル終了 → ルール説明へ
				title->Finalize();
				delete title;
				title = nullptr;

				rule = new Rule();
				rule->Initialize();
				rule->Update();
				currentScene = Scene::Rule;
			}
			else if (title->IsSettingSelected()) {
				// タイトル終了 → 設定へ
				title->Finalize();
				delete title;
				title = nullptr;

				setting = new Setting();
				setting->Initialize();
				currentScene = Scene::Setting;
			}
		}
		break;

	case Scene::Rule:
		rule->Update();

		// リスタートが要求されている場合
		if (rule->ShouldRestart()) {
			rule->Finalize();
			delete rule;
			//gameScene = nullptr;
			rule = new Rule();
			rule->Initialize();
			rule->Update();  // ゲームシーンの更新を行う
			break;  // 他の処理をせず終了
		}
		else if (rule->ShouldReturnToTitle()) {
			rule->Finalize();
			delete rule;
			rule = nullptr;

			title = new Title();
			title->Initialize();
			title->Update();
			currentScene = Scene::Title;
		}
		else if (rule->isFinished()) {
			// ゲーム終了 → クリア
			rule->Finalize();
			delete rule;
			rule = nullptr;

			gameClear = new GameClear();
			gameClear->Initialize(Result);
			gameClear->Update();
			currentScene = Scene::GameClear;
		}
		break;
	case Scene::Select:
		stageSelect->Update();

		if (stageSelect->IsGameStartSelected()) {
			//ゲーム開始

			if (stageSelect->SelectStageNumber() == 0) {
				stage = 1;
			}
			else if (stageSelect->SelectStageNumber() == 1) {
				stage = 2;
			}
			else if (stageSelect->SelectStageNumber() == 2) {
				stage = 3;
			}
			else if (stageSelect->SelectStageNumber() == 3) {
				stage = 4;
			}
			stageSelect->Finalize();
			delete stageSelect;
			stageSelect = nullptr;

			gameScene = new GameScene();
			gameScene->Initialize(stage);
			gameScene->Update();
			currentScene = Scene::Game;

		}

		else if (stageSelect->IsTitleSelected()) {
			//タイトルに戻る
			stageSelect->Finalize();
			delete stageSelect;
			stageSelect = nullptr;

			title = new Title();
			title->Initialize();
			title->Update();
			currentScene = Scene::Title;
		}
		
		break;
		//if (rule->isFinished()) {
		//	// ルール終了 → ゲーム開始
		//	rule->Finalize();
		//	delete rule;
		//	rule = nullptr;

		//	title = new Title();
		//	title->Initialize();
		//	title->Update();
		//	currentScene = Scene::Title;
		//}
		//break;

	case Scene::Game:
		gameScene->Update();

		// リスタートが要求されている場合
		if (gameScene->ShouldRestart()) {
			gameScene->Finalize();
			delete gameScene;
			//gameScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize(stage);
			gameScene -> Update();  // ゲームシーンの更新を行う
			break;  // 他の処理をせず終了
		}
		else if (gameScene->ShouldReturnToTitle()) {
			gameScene->Finalize();
			delete gameScene;
			gameScene = nullptr;

			stageSelect = new StageSelect();
			stageSelect->Initialize();
			stageSelect->Update();
			currentScene = Scene::Select;
		}
		else if (gameScene->isFinished()) {
			// ゲーム終了 → クリア
			Result = gameScene->StarResult();
			gameScene->Finalize();
			delete gameScene;
			gameScene = nullptr;

			gameClear = new GameClear();
			gameClear->Initialize(Result);
			gameClear->Update();
			currentScene = Scene::GameClear;
		}
		break;
	case Scene::GameClear:
		gameClear->Update();

		if (gameClear->isFinished()) {
			// ゲームクリア終了 → タイトルへ
			gameClear->Finalize();
			delete gameClear;
			gameClear = nullptr;

			stageSelect = new StageSelect();
			stageSelect->Initialize();
			stageSelect->Update();
			currentScene = Scene::Select;
		}
		break;

	case Scene::Setting:
		setting->Update();

		if (setting->isFinished()) {
			// 設定終了 → タイトルへ戻る
			setting->Finalize();
			delete setting;
			setting = nullptr;

			title = new Title();
			title->Initialize();
			title->Update();
			currentScene = Scene::Title;
		}
		break;
	}
	Audio::GetInstance()->Update();
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
	case Scene::Setting:
		setting->Draw();
		break;
	case Scene::Select:
		stageSelect->Draw();
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
	// Settingシーンの解放
	if (setting) {
		setting->Finalize();
		delete setting;
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