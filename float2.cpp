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

template<typename T>
float2 operator* (float2 a, const T& b/* const float2 &b */) {
	a *= b;
	return a;
}

template<typename T>
float2 operator/ (float2 a, const T& b) {
	a /= b;
	return a;
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

std::ostream& operator<< (std::ostream& o, const float2& p) {
	return o << "(" << p.x << ", " << p.y << ")";
}
