#pragma once
#include "Vector3.h" // 位置情報用 (独自のベクトルクラス)


#include"DirectXBase.h"

class Star {
public:
    ~Star();
    void Initialize(Vector3, DirectXBase*);
    void Update();   // 更新処理（当たり判定など）
    void Draw();     // 描画処理

    bool IsCollected() const { return IsCheck; }
    Vector3 GetPosition() const { return position; }

    void CheckCollision(Vector3 playerPos, float radius); // プレイヤーとの当たり判定

private:

    Vector3 position; // 星の位置
    bool IsCheck;   // 取得済みかどうか

    //星
    Star* star = nullptr;


};
