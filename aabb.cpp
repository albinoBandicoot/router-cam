#include "common.h"
using namespace std;

double AABB::distance (float3) const {
	return 0;
}

bool AABB::contains (float3 p) const {
	return p >= origin && p <= origin+size;
}

float3 AABB::random_point () const {
	return origin + size*rand_float3();
}

Interpoint<CommonShape> AABB::intersection (Ray) const {

}
