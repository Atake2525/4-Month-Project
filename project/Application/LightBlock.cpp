#include "LightBlock.h"

LightBlock::~LightBlock()
{
	delete BlockModel;
	delete Light2BlockModel;
	delete Light3BlockModel;
	delete Light;
	

}

void LightBlock::Initialize(Vector3 position, Camera*camera, DirectXBase*dxbase, Input*input)
{
	camera_ = camera;
	dxcCommon = dxbase;
	input_ = input;
	blockPosition = position;

	


	//���f���ǂݍ���
	// �Ō��true����͂����enableLighting��true�ɂȂ�(���Ƃ���ł��ύX�\)���͂͂��Ȃ��Ă�����
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	/*switch*/
	Light = new switchLight();
	Light->Initialize({ 0,0,0 }, camera_, dxcCommon, input_);

	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize(); 

	Light2BlockModel = new Object3d();
	Light2BlockModel->Initialize();

	Light3BlockModel = new Object3d();
	Light3BlockModel->Initialize();

	//�ʒu���w�肷��
	BlockModel->SetTranslate(blockPosition);
}

void LightBlock::Update()
{
	Light->Update();
	

}

void LightBlock::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
	Light->Draw(directionalLightResource, pointLightResource, spotLightResource);

	if (Light->GetFlag()) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
	if (Light->GetSwitchFlag2()) {
		Light2BlockModel->SetModel("box.obj");
		Light2BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
	if (Light->GetSwitchFlag3()) {
		Light3BlockModel->SetModel("box.obj");
		Light3BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
}
