#include "Transform.h"
#include "kMath.h"

Transform& operator+=(const Transform& t1, const Transform& t2) {
	Transform result;
	result.rotate = t1.rotate + t2.rotate;
	result.scale = t1.scale + t2.scale;
	result.translate = t1.translate + t2.translate;
	return result;
}

Transform& operator-=(const Transform& t1, const Transform& t2) {
	Transform result;
	result.rotate = t1.rotate - t2.rotate;
	result.scale = t1.scale - t2.scale;
	result.translate = t1.translate - t2.translate;
	return result;
}
