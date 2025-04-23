#include "LightBlock.h"

LightBlock::~LightBlock()
{
	delete BlockModel;

	delete Light;


}

void LightBlock::Initialize(Vector3 position)
{
	input_ = Input::GetInstance();
	blockPosition = position;


	ModelManager::GetInstance()->LoadModel("Resources/Debug", "Grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "box.obj", true);

	/*switch*/
	Light = new switchLight();
	Light->Initialize({ 0,0,0 });

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

void LightBlock::Draw()
{
	Light->Draw();

	if (Light->GetFlag()) {
		BlockModel->SetModel("box.obj");
		BlockModel->Draw();
	}
}