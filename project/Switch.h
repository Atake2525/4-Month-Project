#pragma once
#include"kMath.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"DirectXBase.h"
#include"Camera.h"
#include"Input.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class Switch
{
public:
	void Initialize(Vector3,Camera*,DirectXBase*,Input*);
	void Update();
	void Draw(Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>, Microsoft::WRL::ComPtr<ID3D12Resource>);

private:
	/*���f��*/
	Object3d* switchModel;
	/*�ʒu*/
	Vector3 switchPosition;
	/*�I���I�t�̃t���O*/
	bool switchFlag;
	/*DirectX*/
	DirectXBase* directX;
	/*�J����*/
	Camera* switchCamera;
	/*�؂�ւ��邽�߂Ɂi���j*/
	Input* input_;
};

