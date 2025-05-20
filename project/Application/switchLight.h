#pragma once
#include"kMath.h"
#include"Object3dBase.h"
//#include"ModelBase.h"
#include"ModelManager.h"
#include"Object3d.h"
#include"Input.h"
#include"AABB.h"
#include"Transform.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class Player;

class switchLight
{
public:
	~switchLight();


	void Initialize(Transform,/* Camera*, DirectXBase*,*/ Input*, Player*);


	void Update();
	void Draw();
	bool GetFlag() { return switchFlag; }
	AABB GetAAbb();

	bool IsCollisionAABB(const AABB& a, const AABB& b);

private:
	/*モデル*/
	Object3d* switchModel;
	/*位置*/
	Transform switchTransform;
	/*オンオフのフラグ*/
	bool switchFlag;

	/*切り替えるために（仮）*/
	Input* input_;

	//==========スイッチ範囲指定===============

	Player* player_ = nullptr;

	float radius;

	// スイッチが反応するプレイヤーの最大距離
	float distance = 8.0f;

};
