#include "common.h"
#include <cmath>
#include <iostream>
using namespace std;

float3::float3() {
	x = y = z = 0;
}

float3::float3 (const float3 &a) {
	x = a.x;
	y = a.y;
	z = a.z;
}

float3::float3 (double a, double b, double c) {
	x = a;
	y = b;
	z = c;
}

double float3::operator[] (int idx) {
	return ((double *) this)[idx];
}

bool float3::operator>= (const float3& a) const{
	return x >= a.x && y >= a.y && z >= a.z;
}

bool float3::operator<= (const float3& a) const{
	return x <= a.x && y <= a.y && z <= a.z;
}

// should do something better here
bool float3::operator== (const float3& a) const{
	return x == a.x && y == a.y && z == a.z;
}


void float3::operator+= (const float3 &a) {
	x += a.x;
	y += a.y;
	z += a.z;
}	

void float3::operator-= (const float3 &a) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
}

void float3::operator*= (const float3& a) {
	x *= a.x;
	y *= a.y;
	z *= a.z;
}

void float3::operator*= (double a) {
	x *= a;
	y *= a;
	z *= a;
}

void float3::operator/= (const float3& a) {
	x /= a.x;
	y /= a.y;
	z /= a.z;
}

void float3::operator/= (double a) {
	double ar = 1.0/a;
	x *= ar;
	y *= ar;
	z *= ar;
}

float3 operator+ (const float3& a, const float3 &b) {
	float3 res(a);
	res += b;
	return res;
}

float3 operator- (const float3& a, const float3 &b) {
	float3 res(a);
	res -= b;
	return res;
}


double float3::dot (const float3 &a) const {
	return x*a.x + y*a.y + z*a.z;
}

double float3::length () const {
	return sqrt (x*x + y*y + z*z);
}

float3 float3::cross (const float3& a) const {
	double rx = (y*a.z - z*a.y);
	double ry = (z*a.x - x*a.z);
	double rz = (x*a.y - y*a.x);
	return float3 (rx, ry, rz);
}

float3 float3::normalize () const {
	return *this * (1.0/length());
}

float3 float3::rand() {
	return float3 (randf(), randf(), randf());
}

std::ostream& operator<< (std::ostream& o, const float3& p) {
	return o << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

