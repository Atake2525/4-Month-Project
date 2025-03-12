#include "Goal.h"
#include "kMath.h"

void Goal::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position)
{


	position_ = position;
	model_ = new Model("goal.obj");  // objテクスチャの読み込み
	Vector3(10.0f, 0.0f, 5.0f);

}

void Goal::Update()
{
	//ゴールの動きやアニメーション
	if (//ゴールに当たったら){
		StartEffect = ture;
}

void Goal::Draw()
{
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);

}

void Goal::GoalEffect()
{
}

AABB Goal::GetAABB() {
	Vector3 worldPos = GetworldPosition();

	AABB aabb;

	aabb.min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };

	return aabb;
}

void Goal::OnCollision(const Player* player) { (void)player; }