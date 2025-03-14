#include "Goal.h"
#include "kMath.h"

using namespace Microsoft::WRL;

void Goal::Initialize(Model* model, const Vector3& position)
{
    GoalModel_ = model;  // 外部から渡されたモデルを使う
    position_ = position;

    // モデルの読み込みはここでは不要
    // ModelManager::GetInstance()->LoadModel("Resources/Model", "goal.obj"); は削除
}

void Goal::Update()
{
    // ゴールのアニメーションや動作処理
}

void Goal::Draw()
{
    if (GoalModel_) {
        // 位置を考慮した描画
        GoalModel_->Draw(position_);
    }
}

void Goal::GoalEffect()
{
    // ゴール時のエフェクト処理
}

void Goal::OnCollision(const Player* player)
{
    (void)player;
    isFinished_ = true;  // ゴールフラグを立てる
}
