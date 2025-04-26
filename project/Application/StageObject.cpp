#include "StageObject.h"
#include "Object3d.h"

StageObject::~StageObject() {
	for (Object3d* stage : stageObject)
	{
		delete stage;
	}
	stageObject.clear();
}

void StageObject::AddObject(const std::string& directoryPath, const std::string& filename, const bool& enableLighting) {
	Object3d* object = new Object3d();
	object->Initialize();
	object->SetModel(directoryPath, filename, enableLighting);
	stageObject.push_back(object);
}

void StageObject::AddObject(const std::string& filename) {
	Object3d* object = nullptr;
	object = new Object3d();
	object->Initialize();
	object->SetModel(filename);
	stageObject.push_back(object);
}

void StageObject::Update() {
	for (Object3d* stage : stageObject)
	{
		stage->Update();
	}
}

void StageObject::Draw() {
	for (Object3d* stage : stageObject)
	{
		stage->Draw();
	}
}