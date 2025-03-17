#include "FollowCamera.h"

FollowCamera::FollowCamera()
{
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Initialize()
{
	camera_ = new Camera();


}

void FollowCamera::Update()
{
	if (target_) {
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		worldTransform_.transform.translate = target_->GetTranslate() + offset;

	}


	worldTransform_.UpdateMatrix();

}
