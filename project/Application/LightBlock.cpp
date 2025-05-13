#include "LightBlock.h"
#include"ModelManager.h"

LightBlock::~LightBlock()
{
	if (BlockModel)
	{
		delete BlockModel;
	}
}

void LightBlock::Initialize(const std::string& directoryPath, const std::string& filename)
{
	//ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	//ModelManager::GetInstance()->LoadModel(directoryPath, filename);


	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();

	BlockModel->SetModel(directoryPath, filename);

}

void LightBlock::Update()
{
	BlockModel->Update();
}

void LightBlock::Draw(const bool& switchFlag)
{

	if (switchFlag)
	{
		BlockModel->Draw();
	}

}


const AABB& LightBlock::GetAABB()
{

	return BlockModel->GetAABB();
}





