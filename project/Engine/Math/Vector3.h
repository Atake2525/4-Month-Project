#pragma once

struct Vector3
{
	float x;
	float y;
	float z;
};

const Vector3 operator*(const Vector3& v, const float f) {
	Vector3 result;
	result.x = v.x * f;
	result.y = v.y * f;
	result.z = v.z * f;
	return result;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

const Vector3 operator+(const Vector3& v1, const Vector3 v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

const Vector3 operator-(Vector3& v1, const Vector3 v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3& operator*=(Vector3& v1, const Vector3& v2) {
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
	return v1;
}

const Vector3 operator*(Vector3& v1, const Vector3 v2) {
	Vector3 result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}

Vector3& operator/=(Vector3& v1, const Vector3& v2) {
	v1.x /= v2.x;
	v1.y /= v2.y;
	v1.z /= v2.z;
	return v1;
}

const Vector3 operator/(const Vector3& v1, const Vector3 v2) {
	Vector3 result;
	result.x = v1.x / v2.x;
	result.y = v1.y / v2.y;
	result.z = v1.z / v2.z;
	return result;
}

const Vector3 operator+(const Vector3& v1, const float f) {
	Vector3 result;
	result.x = v1.x / f;
	result.y = v1.y / f;
	result.z = v1.z / f;
	return result;
}

const Vector3 operator-(const Vector3& v1, const float f) {
	Vector3 result;
	result.x = v1.x / f;
	result.y = v1.y / f;
	result.z = v1.z / f;
	return result;
}

const Vector3 operator/(const Vector3& v1, const float f) {
	Vector3 result;
	result.x = v1.x / f;
	result.y = v1.y / f;
	result.z = v1.z / f;
	return result;
}

const Vector3 operator-(const Vector3& v1) {
	Vector3 v;
	v -= v1;
	return v;
}