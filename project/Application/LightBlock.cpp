#include "LightBlock.h"

LightBlock::~LightBlock()
{
	delete BlockModel;

	


}


void LightBlock::Initialize(Vector3 position, Camera*camera, DirectXBase*dxbase, Input*input)

{
	dxcCommon = dxbase;
	input_ = input;
	transform.translate = position;
	camera_ = camera;
	
	

	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "box.obj", true);

	

	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();

	//位置を指定する
	BlockModel->SetTranslate(transform.translate);
}

void LightBlock::Update()
{
	

	BlockModel->Update();

}

void LightBlock::Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource,bool Flag)
{
	

	if (Flag) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
}

AABB LightBlock::GetAABB()
{

		AABB aabb;

		// ブロックの位置とサイズを取得
		Vector3 pos = transform.translate;
		Vector3 size = { transform.scale.x / 2.0f, transform.scale.y / 2.0f, transform.scale.z / 2.0f }; // 半サイズ

		// AABBの最小・最大座標を設定
		aabb.min = { pos.x - size.x, pos.y - size.y, pos.z - size.z };
		aabb.max = { pos.x + size.x, pos.y + size.y, pos.z + size.z };

		return aabb;
}


