#include "Vector3.h"

#pragma once

struct OBB
{
	Vector3 center;
	Vector3 orientation[3];
	Vector3 size;
};

OBB OBBChange(Vector3 min,Vector3 max,Vector3 rotate);