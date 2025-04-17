#pragma once
#include"kMath.h"
#include "Vector3.h" 

#include "Model.h"
#include"Camera.h"
#include "Sprite.h"

#include"Object3d.h"
#include"Object3dBase.h"
#include"ModelBase.h"
#include"ModelManager.h"

#include"DirectXBase.h"


class Player;//後で追加

class Goal
{
public:

    ~Goal();
    // 初期化
    void Initialize(Vector3); //Object3d*
    void Update();
    //描画
    void Draw();

    //当たり判定
    void OnCollision(const Player* player);

    bool GetFlag() { return goalFlag; }

private:

    //DirectX
    DirectXBase* directX;
    Object3d* goalModel;    // objモデルデータ
    Vector3 goalPos;        // ゴールの位置
    Camera* goalCamera;    //カメラ

    //終了フラグ
    bool goalFlag = false;

    //ゴール
    Goal* goal = nullptr;

    Sprite* clearSprite_ = nullptr; // クリアスプライト
};