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
#include"Player.h"
#include"AABB.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>


class LightBlock
{
public:
	~LightBlock();

	void Initialize(Vector3, Camera*, DirectXBase*, Input*);

	void Update();
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>,bool);
	AABB GetAABB();
	
	
private:
	Object3d* BlockModel;
	Transform transform;
	Input* input_;
	DirectXBase* dxcCommon;
	Camera* camera_;
	

	Transform switchPosition;
	//==========当たり判定============

	


};
