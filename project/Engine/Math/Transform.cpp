#include "Transform.h"
#include "kMath.h"
#include "WinApp.h"

void WorldTransform::Initialize() {
	Map();
	transform = {
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};
	parent = nullptr;
}

void WorldTransform::Update() {
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	
	if (parent != NULL)
	{
		Matrix4x4 parentWorldMatrix = MakeAffineMatrix(parent->transform.scale, parent->transform.rotate, parent->transform.translate);
		worldMatrix = Multiply(worldMatrix, parentWorldMatrix);
	}

	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}

void WorldTransform::Map() {
	// �������ނ��߂̃A�h���X���擾
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));

	// �P�ʍs�����������ł���
	transformationMatrix->WVP = MakeIdentity4x4();
	transformationMatrix->World = MakeIdentity4x4();
}