#pragma once
#include"kMath.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"DirectXBase.h"
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
	void Initialize(Vector3, Camera*, DirectXBase*, Input*);
	void Update();
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

private:
	Object3d* BlockModel;
	/*2âÒè¡ÇµÇΩÇÁèoÇÈÇ‚Ç¬*/
	Object3d* Light2BlockModel;
	/*3âÒè¡ÇµÇΩÇÁèoÇÈÇ‚Ç¬*/
	Object3d* Light3BlockModel;

	Camera* camera_;
	Vector3 blockPosition;
	Input* input_;
	DirectXBase* dxcCommon;
	switchLight* Light;
	


};

