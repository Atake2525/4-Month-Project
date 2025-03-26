#include "LightBlock.h"

LightBlock::~LightBlock()
{
	delete BlockModel;

	delete Light;


}

void LightBlock::Initialize(Vector3 position, DirectXBase* dxbase, Input* input)
{
	dxcCommon = dxbase;
	input_ = input;
	blockPosition = position;


	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "box.obj", true);

	/*switch*/
	Light = new switchLight();
	Light->Initialize({ 0,0,0 }, dxcCommon, input_);

	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();

	//位置を指定する
	BlockModel->SetTranslate(blockPosition);
}

void LightBlock::Update()
{
	Light->Update();
	BlockModel->Update();

}

void LightBlock::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	Light->Draw(directionalLightResource, pointLightResource, spotLightResource);

	if (Light->GetFlag()) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
}