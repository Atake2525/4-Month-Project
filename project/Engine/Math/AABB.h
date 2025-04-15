#include "Vector3.h"

#pragma once

struct AABB final
{
	Vector3 min;
	Vector3 max;

	AABB operator+(const AABB& aabb1) const;
	AABB operator-(const AABB& aabb1) const;
};
