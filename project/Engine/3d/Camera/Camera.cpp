#include "Camera.h"
#include "kMath.h"
#include "WinApp.h"

Camera::Camera() 
	: transform({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} })
	, fovY(0.45f)
	, aspect(float(WinApp::GetInstance()->GetkClientWidth()) / float(WinApp::GetInstance()->GetkClientHeight()))
	, nearClipDistance(0.1f)
	, farClipDistance(100.0f)
	, worldMatrix(MakeAffineMatrix(transform.scale, transform.rotate, transform.translate))
	, viewMatrix(Inverse(worldMatrix))
	, projectionMatrix(MakePrespectiveFovMatrix(fovY, aspect, nearClipDistance, farClipDistance))
	, viewProjectionMatrix(Multiply(viewMatrix, projectionMatrix)) 
{}

void Camera::Update() {

	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	if (isParent)
	{
		worldMatrix = Multiply(worldMatrix, parent);
	}
	else if (isTranslateParent)
	{
		Matrix4x4 translate = Multiply(worldMatrix, translateParent);
		worldMatrix.m[3][0] = translate.m[3][0];
		worldMatrix.m[3][1] = translate.m[3][1];
		worldMatrix.m[3][2] = translate.m[3][2];
	}
	else if (isRotateParent)
	{
		Matrix4x4 rotate = Multiply(worldMatrix, rotateParent);
		worldMatrix.m[0][0] = rotate.m[0][0];
		worldMatrix.m[0][1] = rotate.m[0][1];
		worldMatrix.m[0][2] = rotate.m[0][2];
		worldMatrix.m[1][0] = rotate.m[1][0];
		worldMatrix.m[1][1] = rotate.m[1][1];
		worldMatrix.m[1][2] = rotate.m[1][2];
		worldMatrix.m[2][0] = rotate.m[2][0];
		worldMatrix.m[2][1] = rotate.m[2][1];
		worldMatrix.m[2][2] = rotate.m[2][2];
	}

	viewMatrix = Inverse(worldMatrix);
	projectionMatrix = MakePrespectiveFovMatrix(fovY, aspect, nearClipDistance, farClipDistance);
	// ここがエラーの可能性あり
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}