#include "LightBlock.h"

LightBlock::~LightBlock()
{
	delete BlockModel;
	
	delete Light;
	

}

void LightBlock::Initialize(Vector3 position, Camera*camera, DirectXBase*dxbase, Input*input)
{
	camera_ = camera;
	dxcCommon = dxbase;
	input_ = input;
	blockPosition = position;

	

	ModelManager::GetInstance()->Initialize(dxcCommon);
	Object3dBase::GetInstance()->Initialize(dxcCommon);
	Object3dBase::GetInstance()->SetDefaultCamera(camera_);
	ModelBase::GetInstance()->Initialize(dxcCommon);

	//モデル読み込み
	// 最後にtrueを入力するとenableLightingがtrueになる(あとからでも変更可能)入力はしなくても動く
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	/*switch*/
	Light = new switchLight();
	Light->Initialize({ 0,0,0 }, camera_, dxcCommon, input_);

	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();

	//位置を指定する
	BlockModel->SetTranslate(blockPosition);
}

void LightBlock::Update()
{
	Light->Update();
	if (Light->GetFlag()) {
		light2BlockHp -= 1;
		light3BlockHp -= 1;
		if (light2BlockHp == 0)

		
	}

}

void LightBlock::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	Light->Draw(directionalLightResource, pointLightResource, spotLightResource);

	if (Light->GetFlag()) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
}
