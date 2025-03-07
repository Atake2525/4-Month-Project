#include "MyCoilision.h"

bool IsCollision(const OBB& obb1, const OBB& obb2)
{
	// 分離軸リスト
	Vector3 axes[15];
	for (int i = 0; i < 3; ++i) {
		axes[i] = obb1.orientations[i]; // obb1の軸
		axes[3 + i] = obb2.orientations[i]; // obb2の軸
	}

	// obb1 と obb2 の軸のクロス積
	int k = 6;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			axes[k++] = Cross(obb1.orientations[i], obb2.orientations[j]);
		}
	}

	// 各分離軸について調べる
	for (int i = 0; i < 15; ++i) {
		Vector3 axis = axes[i];
		if (axis.x == 0 && axis.y == 0 && axis.z == 0) continue; // ゼロベクトルは無視

		axis = Normalize(axis); // 軸を正規化

		// 各OBBの半径を計算
		float r1 =
			std::abs(Dot(axis, obb1.orientations[0] * obb1.size.x)) +
			std::abs(Dot(axis, obb1.orientations[1] * obb1.size.y)) +
			std::abs(Dot(axis, obb1.orientations[2] * obb1.size.z));

		float r2 =
			std::abs(Dot(axis, obb2.orientations[0] * obb2.size.x)) +
			std::abs(Dot(axis, obb2.orientations[1] * obb2.size.y)) +
			std::abs(Dot(axis, obb2.orientations[2] * obb2.size.z));

		// 投影距離
		Vector3 distance = obb2.center - obb1.center;
		float projectedDistance = std::abs(Dot(distance, axis));

		if (projectedDistance > r1 + r2) {
			return false; // 投影が重ならない -> 当たり判定なし
		}
	}

	return true; // すべての軸で投影が重なる -> 当たり判定あり
}
