#include "common.h"
#include <cmath>
#include <iostream>
using namespace std;

float2::float2() {
	x = y = 0;
}

float2::float2 (double a, double b) {
	x = a;
	y = b;
}

float2::float2 (const float2& p) {
	x = p.x;
	y = p.y;
}

double float2::operator[] (int idx) {
	return ((double *) this)[idx];
}

void float2::operator+= (const float2 &a) {
	x += a.x;
	y += a.y;
}	

void float2::operator-= (const float2 &a) {
	x -= a.x;
	y -= a.y;
}

void float2::operator*= (const float2& a) {
	x *= a.x;
	y *= a.y;
}

void float2::operator*= (double a) {
	x *= a;
	y *= a;
}

void float2::operator/= (const float2& a) {
	x /= a.x;
	y /= a.y;
}

void float2::operator/= (double a) {
	double ar = 1.0/a;
	x *= ar;
	y *= ar;
}

float2 operator+ (const float2& a, const float2 &b) {
	float2 res(a);
	res += b;
	return res;
}

float2 operator- (const float2& a, const float2 &b) {
	float2 res(a);
	res -= b;
	return res;
}

double float2::dot (const float2 &a) const {
	return x*a.x + y*a.y;
}

double float2::length () const {
	return sqrt (x*x + y*y);
}

float2 float2::normalize () const {
	return *this * (1.0/length());
}

double float2::arg () const {
	double res = atan2(y, x) / (2*M_PI);
	if (res < 0) res += 1;
	return res;
}

// 90 degrees clockwise from this
float2 float2::perp_cw () const {
	return float2 (y, -x);
}

float2 float2::perp_ccw () const {
	return float2 (-y, x);
}

float2 float2::rand() {
	return float2 (randf(), randf());
}

std::ostream& operator<< (std::ostream& o, const float2& p) {
	return o << "(" << p.x << ", " << p.y << ")";
}
