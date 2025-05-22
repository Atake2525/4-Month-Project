#include "JampEffect.h"
#include<cassert>
#include<algorithm>
#include"kMath.h"



void JampEffect::Intialize(Vector3 position, Vector3 velocity, const std::string& directoryPath, const std::string& filename)
{
	position_ = position;
	velocity_ = velocity;
	color = { 1.0f,1.0f,1.0f ,1.0f};
	model_->Initialize();
	model_->SetModel(directoryPath, filename);
}

void JampEffect::Update()
{
	position_ += velocity_;
	
	if (isFinished) {
		return;
	}
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished = true;
	}
	color.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	model_->SetColor(color);
	model_->SetTranslate(position_);
	model_->SetRotate(rotarion_);
	model_->SetScale(scale_);
}

void JampEffect::Draw()
{
	model_->Draw();
}

