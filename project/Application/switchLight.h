#pragma once
#include"kMath.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"DirectXBase.h"
#include"Camera.h"
#include"Input.h"
#include"AABB.h"
#include"Player.h"
#include"Transform.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class switchLight
{
public:
	~switchLight();
	void Initialize(Transform, Camera*, DirectXBase*, Input*,Player*);
	void Update();
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);
	bool GetFlag() { return switchFlag; }
	AABB GetAAbb();

private:
	/*モデル*/
	Object3d* switchModel;
	/*位置*/
	Transform switchTransform;
	/*オンオフのフラグ*/
	bool switchFlag;
	/*DirectX*/
	DirectXBase* directX;
	/*カメラ*/
	Camera* switchCamera;
	/*切り替えるために（仮）*/
	Input* input_;

	//==========スイッチ範囲指定===============

	Player* player_;
	AABB* aabb_;
	float radius;

};

