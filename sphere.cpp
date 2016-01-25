#include "common.h"
double Sphere::distance (float3 p) const {
	return (p - center).length() - radius;
}

Interpoint<CommonShape> Sphere::intersection (Ray) const {
}
