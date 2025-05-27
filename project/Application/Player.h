#pragma once
#include "Input.h"
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Transform.h"
#include "Camera.h"
#include "WinApp.h"
#include "PlayerCollision.h"
#include"JampEffect.h"

#include"LightBlock.h"
// 衝突判定で追加
#include "AABB.h"

class Player {
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	Player();

	/// <summary>
	/// 初期化
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// ステージの衝突判定の追加
	void AddStageCollision(const std::string& directoryPath, const std::string& filename);

	// ステージの衝突判定の削除
	void ClearStageCollision();

	// ライトブロックの衝突判定の追加
	void AddLightBlockCollision(const std::string& directoryPath, const std::string& filename);

	// ライトブロックの衝突判定の削除
	void ClearLightBlockCollision();

	// ステージの衝突判定更新
	void UpdateStageCollision();

	// ライトブロックの衝突判定更新
	void UpdateLightCollision();

	// カメラの衝突判定更新
	void UpdateCameraCollision();

	void SetSwitchFlag(const bool flag);

	Camera* GetCamera();


	// 衝突判定作成で追加した者たち
	bool GetOnGround() { return onGround_; }

	void SetOnGround(const bool& onGround) { onGround_ = onGround; }

	Vector3 GetVelocity() { return velocity; }

	void SetVelocity(const Vector3& vel) { velocity = vel; }

	Vector3 GetPosition();

	const AABB& GetAABB() const { return object3d_->GetAABB(); }

	void AddTranslate(const Vector3& translate) { 
		Vector3 result = translate;
		modelTransform_.translate += result; 
	}

	void CheckCollsion(LightBlock* block);
	
	//ゴールの当たり判定で追加
	 Object3d *GoalObject3d()  { return object3d_; }
	 // 星の当たり判定で追加
	 Object3d* StarObject3d() { return object3d_; }
	 bool IsCollisionAABB(const AABB& a, const AABB& b);

	 void EffectBorn(Vector3 position);

private:

	void Move();

	void Rotate();

	void Jump();

private: // メンバ変数

	Transform modelTransform_;

	Transform drawModel;

	Object3d* object3d_ = nullptr;

	Camera* camera_ = nullptr;

	Vector4 modelColor_;

	Input* input_ = nullptr;

	bool modelEnableLighting_;

	float shininess_;

	Transform cameraTransform_;

	bool onGround_ = true;

	Vector3 velocity{ 0.0f, 0.0f, 0.0f };

	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 20.0f;

	// 重力加速度（下方向）
	static inline const float kGravityAccleration = 0.98f;

	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.98f;

	static inline float JumpVelocity = 0.0f;


	// 追加したクラス
	PlayerCollision* collision = nullptr;

	bool mouseFlag = true;

	// ライト用の衝突判定用PlayerCollision
	PlayerCollision* lightCollision = nullptr;

	bool collisionLightBlock = false;

	bool switchFlag = false;

	// 初期のカメラオフセット
	Vector3 defaultCameraOffset = { 0.0f, 10.0f, -20.0f };
	// カメラオフセット(プレイヤーからのカメラ距離)
	Vector3 cameraOffset;
	// 変更時のカメラオフセット
	Vector3 beforCameraOffset;

	AABB cameraAABB;
	AABB firstCameraAABB;

	AABB defaultCameraAABB;
	Transform defaultCameraTransform;
	Vector3 defaultCameraVelocityPre = { 0.0f, 0.0f, 0.0f };
	Vector3 defaultCameraVelocity = { 0.0f, 0.0f, 0.0f };

	Vector3 cameraVelocityPre = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraVelocity = { 0.0f, 0.0f, 0.0f };

	bool cameraZoomIn = false;
	bool cameraZoomOut = false;

	float cameraEasingTime = 0.0f;
	//ジャンプエフェクトクラス
	std::list<JampEffect*>effects_;

	// プレイヤーの回転に必要な変数
	float plRotate = 0.0f;

	float prot = 0.0f;

	// 回転イージングのフラグ
	bool rotateEasing = false;

	// 回転イージングのフレーム
	float rotateFrame = 0.0f;

	// 回転イージングの終了フレーム
	const float rotateEndFrame = 0.4f;

	// 初期の回転角の格納
	float startRotate = 0.0f;

};

