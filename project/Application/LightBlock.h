#pragma once
#include"kMath.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"Camera.h"
#include"Input.h"
#include"switchLight.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


class LightBlock
{
public:
	~LightBlock();
	void Initialize(Vector3);
	void Update();
	void Draw();

private:
	Object3d* BlockModel;
	Vector3 blockPosition;
	Input* input_;
	switchLight* Light;


};
