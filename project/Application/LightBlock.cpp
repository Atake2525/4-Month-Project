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
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "lightblock.obj");




	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();
	transform.rotate = BlockModel->GetRotateInDegree();
	transform.scale = BlockModel->GetScale();

	BlockModel->SetModel("lightblock.obj");

}

void LightBlock::Update()
{



	BlockModel->Update();
	//位置を指定する
	BlockModel->SetTranslate(transform.translate);
	BlockModel->SetRotateInDegree(transform.rotate);
	BlockModel->SetScale(transform.scale);
	BlockModel->SetModel("lightblock.obj");
}

void LightBlock::Draw()
{




	BlockModel->Draw();

}

const AABB& LightBlock::GetAABB()
{

	return BlockModel->GetAABB();
}

