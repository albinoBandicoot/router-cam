#include <iostream>
#include "gpoly.h"
using namespace std;

double GVertex::distance (float2 p) const {
	return (p - pos).length();
}

void GLineseg::recompute () {
	vec = end->pos - start->pos;
	norm = vec.perp_ccw().normalize();
	length = vec.length();
}

GLineseg::GLineseg (float2 s, float2 e) {
	start = new GVertex(s);
	end = new GVertex(e);
	recompute();
}

GLineseg::GLineseg (GVertex *a, GVertex *b) {
	start = a;
	end = b;
	recompute();
}

float2 GLineseg::operator() (double t) const {
	return start->pos + vec * t;
}

float2 GLineseg::normal (double t) const {
	return norm;
}

double GLineseg::project (float2 p) const {
	p -= start->pos;
	return p.dot(vec) / vec.dot(vec);
}

int GLineseg::side_test (float2 p) const {
	return (p - start->pos).dot(norm) > 0 ? 1 : -1;
}


