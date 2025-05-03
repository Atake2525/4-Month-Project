#pragma once
#include "Star.h"
#include <vector>
#include "DirectXBase.h"


class starResult {
public:
	~starResult();

	void Initialize();
	void Update();
	void Draw();

	// starResult.h に追加
	std::vector<Star*>& GetStars() { return stars_; }


private:
	std::vector<Star*> stars_;  // 星のリスト
	DirectXBase* directX_;

};
