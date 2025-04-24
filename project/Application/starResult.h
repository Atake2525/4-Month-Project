#pragma once
#include "Star.h"
#include <vector>

class starResult {
public:
	~starResult();

	void Initialize();
	void Update();
	void Draw();


private:
	std::vector<Star*> stars_;  // 星のリスト
	DirectXBase* directX_;
	//int score_ = 0;
};
