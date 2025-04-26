#include <vector>
#include <string>

#pragma once

class Object3d;

class StageObject
{
public:

	~StageObject();

	void AddObject(const std::string& directoryPath, const std::string& filename, const bool& enableLighting = false);

	void AddObject(const std::string& filename);

	void Update();

	void Draw();


private:
	std::vector<Object3d*> stageObject;

};

