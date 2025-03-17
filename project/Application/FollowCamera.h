#pragma once
#include "WorldTransform.h"
#include "Camera.h"
#include "kMath.h"

class FollowCamera{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	~FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }



private: // メンバ変数

	WorldTransform worldTransform_;

	Camera* camera_ = nullptr;

	const WorldTransform* target_ = nullptr;

	Transform cameratransform_;
};