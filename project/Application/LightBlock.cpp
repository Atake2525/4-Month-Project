#include "LightBlock.h"
#include"ModelManager.h"

LightBlock::~LightBlock()
{
	if (BlockModel)
	{
		delete BlockModel;
	}
}

void LightBlock::Initialize(const Vector3& position/*, Camera*camera, Input*input*/)
{
	transform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		position
	};
	//input_ = input;
	//transform.translate = position;
	//camera_ = camera;
	
	

	//ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
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
	bool flag = Flag;
	if (flag) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw(directionalLightResource, pointLightResource, spotLightResource);
	}
}

const AABB& LightBlock::GetAABB() const
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


