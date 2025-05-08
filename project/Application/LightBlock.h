#pragma once
#include"kMath.h"
//#include"Object3dBase.h"
//#include"ModelBase.h"
#include"Object3d.h"
#include"AABB.h"


#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


class LightBlock
{
public:
	~LightBlock();


	void Initialize(const Vector3& position/*, Camera*, Input**/);

	void Update();
	void Draw(const bool& switchFlag);
	const AABB& GetAABB();
	void MakeAABB(const std::string& directoryPath, const std::string& filename);

private:
	struct VertexData {
		Vector4 position;
		Vector3 normal;
	};
	Object3d* BlockModel;
	Transform transform;
	AABB aabb_;
	//Input* input_;
	//Camera* camera_;

	//switchLight* Light = nullptr;


};