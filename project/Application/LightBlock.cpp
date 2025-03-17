#include "LightBlock.h"

void LightBlock::Initialize(Vector3 position, Camera*camera, DirectXBase*dxbase, Input*input)
{
	camera_ = camera;
	dxcCommon = dxbase;
	input_ = input;
	blockPosition = position;

	Light = new switchLight();
	Light->Initialize({ 0,0,0 }, camera_, dxcCommon, input_);

	ModelManager::GetInstance()->Initialize(dxcCommon);
	Object3dBase::GetInstance()->Initialize(dxcCommon);
	Object3dBase::GetInstance()->SetDefaultCamera(camera_);
	ModelBase::GetInstance()->Initialize(dxcCommon);

}

void LightBlock::Update()
{

}

void LightBlock::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource)
{
}
