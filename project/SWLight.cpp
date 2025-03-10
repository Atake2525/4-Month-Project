#include "SWLight.h"
#include "SwitchLight1.h"
SwitchLight1::~SwitchLight1()
{
}
void SwitchLight1::Initialize(Vector3 position, Camera* camera, DirectXBase* dxc, Input* input)
{
	directX = dxc;
	switchCamera = camera;
	switchPosition = position;
	input_ = input;

	ModelManager::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->Initialize(directX);
	Object3dBase::GetInstance()->SetDefaultCamera(switchCamera);
	ModelBase::GetInstance()->Initialize(directX);


	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	switchFlag = false;
	// object3dの初期化(KamataEngineで言うところのModel)
	switchModel = new Object3d();
	switchModel->Initialize();
	// Modelを指定する
	switchModel->SetModel("box.obj");
	//位置を指定する
	switchModel->SetTranslate(switchPosition);
}

void SwitchLight1::Update()
{
	//falseの時におしたらtrueになる
	if (!switchFlag) {
		if (input_->TriggerKey(DIK_SPACE)) {
			switchFlag = true;
		}
	}
	//trueの時におしたらfalseになる
	if (switchFlag) {
		if (input_->TriggerKey(DIK_SPACE)) {
			switchFlag = false;
		}
	}
}

void SWLight::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	if (switchFlag) {
		switchModel->SetModel("box.obj");
		switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
	else if (!switchFlag) {
		switchModel->SetModel("axis.obj");
		switchModel->Draw(directionalLightResource, pointLightResource, spotLightResource);

	}
}

