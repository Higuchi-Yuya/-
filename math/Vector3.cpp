#include "Vector3.h"
#include <cmath>

float Vector3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= length();
	}

	return *this;
}
