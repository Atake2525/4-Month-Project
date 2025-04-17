#pragma once
#include"kMath.h"
//#include"Object3dBase.h"
//#include"ModelBase.h"
#include"Object3d.h"
//#include"Camera.h"
//#include"Input.h"
//#include"switchLight.h"
//#include"Player.h"
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
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>directionalLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>pointLightResource, Microsoft::WRL::ComPtr<ID3D12Resource>spotLightResource);
	const AABB& GetAABB() ;
	
	
private:
	Object3d* BlockModel;
	Transform transform;
	AABB aabb_;
	//Input* input_;
	//Camera* camera_;

	//switchLight* Light = nullptr;
	

	//Transform switchPosition;
	//==========当たり判定============

};
