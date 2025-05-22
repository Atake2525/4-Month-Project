#include "JampEffect.h"
#include<cassert>
#include<algorithm>
#include"kMath.h"



void JampEffect::Intialize(Vector3 position, Vector3 velocity)
{

	ModelManager::GetInstance()->LoadModel("Resources/Model/obj", "effectParticle.obj");
	position_ = position;
	velocity_ = velocity;
	
	model_= new Object3d();
	model_->Initialize();
	model_->SetModel("effectParticle.obj");

	color = { 1.0f,1.0f,1.0f ,1.0f };
}

void JampEffect::Update()
{
	
	if (isFinished) {
		return;
	}
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished = true;
	}

	position_ -= velocity_;

	color.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	scale_ = { 5.0f,5.0f,5.0f };

	model_->SetColor(color);
	model_->SetTranslate(position_);
	model_->SetRotate(rotarion_);
	model_->SetScale(scale_);
	model_->Update();
}

void JampEffect::Draw()
{
	model_->Draw();
}

