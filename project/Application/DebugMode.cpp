#include "DebugMode.h"

using namespace Microsoft::WRL;

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"

// ---- 説明 ---- //
// 
// -- 操作説明 --
// ・視点に向かってWASD操作
// ・マウスと十字キーで視点を移動
// ・Q,Eで視点の回転Zを変更
// ・LCONTROLでマウスカーソルを出す
// ・ESCAPEでゲーム終了
// 
// 
// -------------- //

void DebugMode::Initialize() {

	// 初期化処理(統一)

	FrameWork::Initialize();

#pragma region 基盤システムの初期化

	winApp = new WinApp();
	winApp->Initialize();

	directxBase = new DirectXBase();
	directxBase->Initialize(winApp);

	camera = new Camera();
	camera->SetRotate({ 0.36f, 0.0f, 0.0f });
	camera->SetTranslate({ 0.0f, 6.0f, -19.0f });

	SpriteBase::GetInstance()->Initialize(directxBase);

	Object3dBase::GetInstance()->Initialize(directxBase);
	Object3dBase::GetInstance()->SetDefaultCamera(camera);

	ModelBase::GetInstance()->Initialize(directxBase);

	TextureManager::GetInstance()->Initialize(directxBase);

	ModelManager::GetInstance()->Initialize(directxBase);

	Audio::GetInstance()->Initialize();

	input = new Input();
	input->Initialize(winApp);

	// ここまでの初期化は必須

	// デバッグ用、Modelをワイヤーフレーム表示にする
	WireFrameObjectBase::GetInstance()->Initialize(directxBase);
	WireFrameObjectBase::GetInstance()->SetDefaultCamera(camera);

#pragma endregion 基盤システムの初期化

	// テクスチャのロード DataはTextureManagerに保管される
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	// モデルのロード
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model", "stage.obj", true);
	ModelManager::GetInstance()->LoadModel("Resources/Model", "block.obj");

	// サウンドのロード soundData1にDataが返される
	soundData1 = Audio::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

	// Spriteの初期化
	sprite = new Sprite();
	// 使用するSpriteを
	sprite->Initialize("Resources/uvChecker.png");
	sprite->SetScale(Vector2{ 200.0f, 200.0f });

	// object3dの初期化(KamataEngineで言うところのModel)
	object3d = new Object3d();
	object3d->Initialize();
	// Modelを指定する
	object3d->SetModel("stage.obj");

	grid = new Object3d();
	grid->Initialize();
	grid->SetModel("block.obj");

	playerModel = new Object3d();
	playerModel->Initialize();
	playerModel->SetModel("Player.obj");

	// ライト関係の初期化
	directionalLightResource = directxBase->CreateBufferResource(sizeof(DirectionalLight));

	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	directionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData->intensity = 1.0f;
	directionalLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	pointLightResource = directxBase->CreateBufferResource(sizeof(PointLight));

	pointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));

	pointLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData->position = { 0.0f, 2.0f, 0.0f };
	pointLightData->intensity = 0.0f;
	pointLightData->radius = 5.0f;
	pointLightData->dacay = 5.0f;
	pointLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	spotLightResource = directxBase->CreateBufferResource(sizeof(SpotLight));

	spotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));

	spotLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightData->position = { 2.0f, 1.25f, 0.0f };
	spotLightData->distance = 7.0f;
	spotLightData->direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLightData->intensity = 0.0f;
	spotLightData->dacay = 2.0f;
	spotLightData->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 2.6f);
	spotLightData->specularColor = { 1.0f, 1.0f, 1.0f };

	// Transform変数を作る
	Transform transform{
		{1.0f, 1.0f,   1.0f},
		{0.0f, -1.58f, 0.0f},
		{0.0f, 0.0f,   0.0f}
	};

	// ImGuiメンバ変数の初期化

	// Sprite
	position = sprite->GetPosition();
	rotation = sprite->GetRotation();
	scale = sprite->GetScale();
	color = sprite->GetColor();
	anchorPoint = sprite->GetAnchorPoint();
	isFlipX = sprite->GetIsFlipX();
	isFlipY = sprite->GetIsFlipY();
	textureLeftTop = sprite->GetTextureLeftTop();
	textureSize = sprite->GetTextureSize();

	// Model
	modelTransform = object3d->GetTransform();
	modelTransform.rotate = object3d->GetRotateInDegree();
	modelColor = object3d->GetColor();
	modelEnableLighting = object3d->GetEnableLighting();
	shininess = object3d->GetShininess();

	player = new Player();
	player->Initialize(playerModel, camera, winApp);

	// Camera
	farClip = camera->GetFarClipDistance();
	fov = camera->GetfovY();
}

void DebugMode::Update() {

	FrameWork::Update();

	if (winApp->ProcessMessage()) {
		Finished = true;
	}
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Imguiの設定(Color&STR)
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.0f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.0f, 0.7f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("colorConfig");

	if (ImGui::TreeNode("ColorCode")) {
		ImGui::DragFloat4("RGB", &modelColor.x, 0.01f);
		ImGui::ColorEdit4("RGB", &modelColor.x);
		// ImGui::ColorEdit3("RGBSprite", &color.x);
		ImGui::SliderFloat("R", &modelColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &modelColor.y, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &modelColor.z, 0.0f, 1.0f);
		ImGui::SliderFloat("A", &modelColor.w, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("colorPreset")) {
		if (ImGui::Button("RED")) {
			modelColor.x = 1.0f;
			modelColor.y = 0.0f;
			modelColor.z = 0.0f;
		}
		if (ImGui::Button("GREEN")) {
			modelColor.x = 0.0f;
			modelColor.y = 1.0f;
			modelColor.z = 0.0f;
		}
		if (ImGui::Button("BLUE")) {
			modelColor.x = 0.0f;
			modelColor.y = 0.0f;
			modelColor.z = 1.0f;
		}
		if (ImGui::Button("YELLOW")) {
			modelColor.x = 1.0f;
			modelColor.y = 1.0f;
			modelColor.z = 0.0f;
		}
		if (ImGui::Button("WHITE")) {
			modelColor.x = 1.0f;
			modelColor.y = 1.0f;
			modelColor.z = 1.0f;
		}
		if (ImGui::Button("BLACK")) {
			modelColor.x = 0.0f;
			modelColor.y = 0.0f;
			modelColor.z = 0.0f;
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("STR")) {
		ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("ModelSTR")) {
		ImGui::DragFloat3("Scale", &modelTransform.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &modelTransform.rotate.x, 1.0f);
		ImGui::DragFloat3("Translate", &modelTransform.translate.x, 0.01f);
		ImGui::Checkbox("EnableLighting", &modelEnableLighting);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("SpriteSTR")) {
		ImGui::DragFloat2("Scale", &scale.x, 1.0f);
		ImGui::DragFloat("Rotate", &rotation, 0.01f);
		ImGui::DragFloat2("Translate", &position.x, 1.0f);
		ImGui::DragFloat2("AnchorPoint", &anchorPoint.x, 0.1f);
		ImGui::Checkbox("FlipX", &isFlipX);
		ImGui::Checkbox("FlipY", &isFlipY);
		ImGui::DragFloat2("TextureLeftTop", &textureLeftTop.x, 0.1f);
		ImGui::DragFloat2("TextureSize", &textureSize.x, 0.1f);
		// ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		// ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		// ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Lighting")) {
		ImGui::ColorEdit3("SpecularColor", &specularColor.x);
		ImGui::DragFloat("Shininess", &shininess, 0.1f);
		if (ImGui::TreeNode("DirectionalLight")) {
			ImGui::ColorEdit4("Color", &directionalLightData->color.x);
			ImGui::ColorEdit3("SpecularColor", &directionalLightData->specularColor.x);
			ImGui::SliderFloat3("Direction", &directionalLightData->direction.x, 1.0f, -1.0f);
			ImGui::DragFloat("Insensity", &directionalLightData->intensity, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PointLight")) {
			ImGui::ColorEdit4("Color", &pointLightData->color.x);
			ImGui::ColorEdit3("SpecularColor", &pointLightData->specularColor.x);
			ImGui::DragFloat3("Positoin", &pointLightData->position.x, 0.1f);
			ImGui::SliderFloat("radius", &pointLightData->radius, 0.0f, 100.0f);
			ImGui::SliderFloat("dacay", &pointLightData->dacay, 1.0f, 10.0f);
			ImGui::DragFloat("Insensity", &pointLightData->intensity, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("SpotLight")) {
			ImGui::ColorEdit4("Color", &spotLightData->color.x);
			ImGui::ColorEdit3("SpecularColor", &spotLightData->specularColor.x);
			ImGui::DragFloat3("Positoin", &spotLightData->position.x, 0.1f);
			ImGui::DragFloat3("Direction", &spotLightData->direction.x, 0.1f);
			ImGui::DragFloat("cosAngle", &spotLightData->cosAngle, 0.01f);
			ImGui::DragFloat("cosFalloffStart", &spotLightData->cosFalloffStart, 0.01f);
			ImGui::SliderFloat("distance", &spotLightData->distance, 0.0f, 10.0f);
			ImGui::SliderFloat("dacay", &spotLightData->dacay, 0.0f, 10.0f);
			ImGui::DragFloat("Insensity", &spotLightData->intensity, 1.0f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(1080, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("Camera");

	ImGui::DragFloat3("Rotate", &cameraTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &cameraTransform.translate.x, 0.01f);
	ImGui::DragFloat("FarClip", &farClip, 1.0f);
	ImGui::DragFloat("Fov", &fov, 0.01f);
	ImGui::DragFloat2("mousePos2", &mousePos2.x, 1.0f);
	ImGui::DragFloat3("mousePos3", &mousePos3.x, 1.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// cosFalloffStartがcosAngleより下にならないように調整
	spotLightData->cosFalloffStart = max(spotLightData->cosFalloffStart, spotLightData->cosAngle);

	pointLightData->dacay = std::clamp(pointLightData->dacay, 1.0f, 10.0f);

	// Shininessの変更関数
	object3d->SetShininess(shininess);

	// FarClip(描画距離を変更するための関数)
	camera->SetFarClipDistance(farClip);

	// FovY(視野角を変更するための関数)
	camera->SetFovY(fov);

	mousePos2 = input->GetMousePos2();
	mousePos3 = input->GetMousePos3();

#ifdef _DEBUG
	const float speed = 0.7f;
	Vector3 velocity(0.0f, 0.0f, speed);
	velocity = TransformNormal(velocity, camera->GetWorldMatrix());
	if (input->PushKey(DIK_W)) {
		cameraTransform.translate += velocity;
	}
	if (input->PushKey(DIK_S)) {
		cameraTransform.translate -= velocity;
	}
	velocity = { speed, 0.0f, 0.0f };
	velocity = TransformNormal(velocity, camera->GetWorldMatrix());
	if (input->PushKey(DIK_A)) {
		cameraTransform.translate -= velocity;
	}
	if (input->PushKey(DIK_D)) {
		cameraTransform.translate += velocity;
	}
	if (input->PushKey(DIK_SPACE)) {
		cameraTransform.translate.y += 1.0f;
	}
	if (input->PushKey(DIK_LSHIFT)) {
		cameraTransform.translate.y -= 1.0f;
	}
	if (input->PushKey(DIK_LEFT)) {
		cameraTransform.rotate.y -= 0.03f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		cameraTransform.rotate.y += 0.03f;
	}
	if (input->PushKey(DIK_UP)) {
		cameraTransform.rotate.x -= 0.03f;
	}
	if (input->PushKey(DIK_DOWN)) {
		cameraTransform.rotate.x += 0.03f;
	}
	if (input->PushKey(DIK_Q)) {
		cameraTransform.rotate.z -= 0.01f;
	}
	if (input->PushKey(DIK_E)) {
		cameraTransform.rotate.z += 0.01f;
	}

	if (input->TriggerKey(DIK_0)) {
		// 音声再生
		Audio::GetInstance()->SoundPlayWave(soundData1);
	}
	if (input->TriggerKey(DIK_ESCAPE)) {
		Finished = true;
	}
	if (input->TriggerKey(DIK_LCONTROL)) {
		showCursor = !showCursor;
		input->ShowMouseCursor(showCursor);
	}

	if (!showCursor)
	{
		mousePos3.z = mousePos3.y;
		mousePos3.y = mousePos3.x;
		mousePos3.x = mousePos3.z;
		mousePos3.z = 0.0f;
		mousePos3 = SwapRadian(mousePos3) / 10.0f;
		cameraTransform.rotate += mousePos3;
	}

	cameraTransform.rotate.x = std::clamp(cameraTransform.rotate.x, SwapRadian(-90.0f), SwapRadian(90.0f));
#endif // _DEBUG

	// 更新処理
	camera->SetRotate(cameraTransform.rotate);
	camera->SetTranslate(cameraTransform.translate);
	camera->Update();

	sprite->SetStatus(position, rotation, scale, color);
	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetIsFlip(isFlipX, isFlipY);
	sprite->SetTextureLeftTop(textureLeftTop);
	sprite->SetTextureSize(textureSize);
	sprite->Update();

	object3d->SetTransform(modelTransform);
	object3d->SetRotateInDegree(modelTransform.rotate);
	object3d->SetColor(modelColor);
	object3d->SetEnableLighting(modelEnableLighting);
	object3d->Update();

	grid->Update();

	player->Update();
}

void DebugMode::Draw() {
	// ImGuiの内部コマンドを生成する
	ImGui::Render();

	// 描画前処理(この後にDrawしないといけない
	directxBase->PreDraw();

	// 共通描画設定(SpriteのDrawに必要)
	SpriteBase::GetInstance()->ShaderDraw();

	sprite->Draw();

	// 共通描画設定(ModelのDrawに必要)
	Object3dBase::GetInstance()->ShaderDraw();

	// モデルの描画(各ライトを入れないといけない)
	object3d->Draw(directionalLightResource, pointLightResource, spotLightResource);

	player->Draw(directionalLightResource, pointLightResource, spotLightResource);

	// ここから下でDrawしたModelはグリッド表示される
	WireFrameObjectBase::GetInstance()->ShaderDraw();

	grid->Draw(directionalLightResource, pointLightResource, spotLightResource);

	// 実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directxBase->GetCommandList().Get());

	input->Update();

	directxBase->PostDraw();
}

void DebugMode::Finalize() {
	// 終了処理(今までと変わらない
	winApp->Finalize();
	delete winApp;

	directxBase->Finalize();
	delete directxBase;

	delete camera;

	SpriteBase::GetInstance()->Finalize();

	Object3dBase::GetInstance()->Finalize();

	ModelBase::GetInstance()->Finalize();

	TextureManager::GetInstance()->Finalize();

	ModelManager::GetInstance()->Finalize();

	delete sprite;

	Audio::GetInstance()->SoundUnload(&soundData1);
	Audio::GetInstance()->Finalize();

	delete object3d;

	delete grid;

	delete input;

	WireFrameObjectBase::GetInstance()->Finalize();

	FrameWork::Finalize();
}