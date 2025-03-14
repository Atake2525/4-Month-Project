#include "WorldTransform.h"
#include "kMath.h"
#include "WinApp.h"
#include "Camera.h"
#include "DirectXBase.h"

void WorldTransform::Initialize(DirectXBase* directxBase) {
	dxBase = directxBase;
	Map();
	transform = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	parent = nullptr;
}

void WorldTransform::UpdateMatrix(Camera* camera) {
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	if (parent != NULL)
	{
		Matrix4x4 parentWorldMatrix = MakeAffineMatrix(parent->transform.scale, parent->transform.rotate, parent->transform.translate);
		worldMatrix = Multiply(worldMatrix, parentWorldMatrix);
	}

	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}

void WorldTransform::Map() {
	CreateTransformationMatrixResrouce();

	// 書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));

	// 単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
	transformationMatrix->World = MakeIdentity4x4();
}

void WorldTransform::CreateTransformationMatrixResrouce() {
	transformationMatrixResource = dxBase->CreateBufferResource(sizeof(TransformationMatrix));
}

const Vector3 WorldTransform::GetWorldTranslation() const {
	Vector3 result{ worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2] };
	return result;
}