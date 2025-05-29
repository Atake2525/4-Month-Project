#pragma once
#include"Object3d.h"
#include"Vector3.h"
#include"Vector4.h"
#include"ModelManager.h"



class JampEffect
{
public:
	void Intialize(Vector3 position,Vector3 velocity,Vector3 rotate);
	void Update();
	void Draw();
	bool IsFinished() { return isFinished; };
private:
	Object3d* model_=nullptr;
	Vector3 position_;
	Vector3 scale_;
	Vector3 rotarion_;
	Vector3 velocity_;
	Vector4 color;
	float counter_;
	static inline const float kDuration = 0.5f;
	bool isFinished = false;
};

