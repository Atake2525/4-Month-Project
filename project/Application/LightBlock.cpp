#include "LightBlock.h"
#include"ModelManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "kMath.h"
#include <cassert>
#define NOMINMAX

#include "externels/imgui/imgui.h"
#include "externels/imgui/imgui_impl_dx12.h"
#include "externels/imgui/imgui_impl_win32.h"


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
	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "lightBlock.obj");




	/*model*/
	BlockModel = new Object3d();
	BlockModel->Initialize();
	transform.rotate = BlockModel->GetRotateInDegree();
	transform.scale = BlockModel->GetScale();

	BlockModel->SetModel("lightBlock.obj");

}

void LightBlock::Update()
{



	BlockModel->Update();
	//位置を指定する
	BlockModel->SetTranslate(transform.translate);
	BlockModel->SetRotateInDegree(transform.rotate);
	BlockModel->SetScale(transform.scale);
	BlockModel->SetModel("lightBlock.obj");
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

void LightBlock::MakeAABB(const std::string& directoryPath, const std::string& filename)
{

	const float epsilon = 1e-6;

	std::vector<VertexData> vertices;
	AABB aabb;
	
	Vector3 collisionNormal = { 0.0f, 0.0f, 0.0f };
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュが無いのは対応しない

}





