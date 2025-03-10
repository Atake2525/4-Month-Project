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


	//���f���ǂݍ���
	// �Ō��true����͂����enableLighting��true�ɂȂ�(���Ƃ���ł��ύX�\)���͂͂��Ȃ��Ă�����
	ModelManager::GetInstance()->LoadModel("Resources/Model", "axis.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model", "box.obj", true);

	switchFlag = false;
	// object3d�̏�����(KamataEngine�Ō����Ƃ����Model)
	switchModel = new Object3d();
	switchModel->Initialize();
	// Model���w�肷��
	switchModel->SetModel("box.obj");
	//�ʒu���w�肷��
	switchModel->SetTranslate(switchPosition);
}

void SwitchLight1::Update()
{
	//false�̎��ɂ�������true�ɂȂ�
	if (!switchFlag) {
		if (input_->TriggerKey(DIK_SPACE)) {
			switchFlag = true;
		}
	}
	//true�̎��ɂ�������false�ɂȂ�
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

