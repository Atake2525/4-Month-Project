#pragma once

struct Vector3
{
	float x;
	float y;
	float z;

	// ���Z���Z�q�̃I�[�o�[���[�h
	Vector3 operator-(const Vector3& other) const;
};