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


/* GCircle */

float2 GCircle::operator() (double t) const {
	t *= 2 * M_PI;
	return c.center + float2(cos(t), sin(t))*c.radius;
}

float2 GCircle::normal (double t) const {
	t *= 2 * M_PI;
	return float2(sin(t), cos(t));
}

double GCircle::project (float2 p) const {
	return (p-c.center).arg();
}

int GCircle::side_test (float2 p) const {
	return c.distance(p) > c.radius ? 1 : -1;
}

/* GArc */

GArc::GArc (Circle circ, double th1, double th2, bool clockwise) {
	arg_start = th1 / (2*M_PI);
	arg_end = th2 / (2*M_PI);
	c = circ;
	cw = clockwise;
	if (!cw && arg_end < arg_start) arg_end += 1;
	if (cw && arg_end > arg_start) arg_start += 1;
}

GArc::GArc (Circle circ, GVertex *a, GVertex *b, bool clockwise) {
	c = circ;
	cw = clockwise;
	if (c.on_surface(a->pos) && c.on_surface(b->pos)) {
		start = a;
		end = b;
		arg_start = a->pos.arg();
		arg_end = b->pos.arg();
		if (!cw && arg_end < arg_start) arg_end += 1;
		if (cw && arg_end > arg_start) arg_start += 1;
	} else {
		// error!
	}
}

float2 GArc::operator() (double t) const {
	// t is 0 to 1 on arc (0 is start, 1 end). Convert to arg on circle:
	double arg = arg_start + (arg_end - arg_start)*t;
	if (arg > 1) arg -= 1;
	return GCircle::operator()(arg);
}

float2 GArc::normal (double t) const {
	double arg = arg_start + (arg_end - arg_start)*t;
	if (arg > 1) arg -= 1;
	return GCircle::normal(arg);
}

double GArc::project (float2 p) const {
	double a =  (p-c.center).arg();
	cout << "a = " << a << "; arg_start = " << arg_start << "; arg_end = " << arg_end << endl;
	if (cw) {
		if (arg_start > 1){
		   if (a < arg_start-1)	a += 1;
		}
		if (a < arg_end || a > arg_start) return -1;
	} else {
		if (arg_end > 1) {	// boundary spanning
			if (a < arg_end-1) a += 1;
		}
		if (a > arg_end || a < arg_start) return -1;
	}
	return (a - arg_start) / (arg_end - arg_start);
}

int main () {
	GVertex *a = new GVertex(float2(0,1));
	GVertex *b = new GVertex(float2(1,3));

	Circle c = Circle (float2(1,0), 2);
	GArc line = GArc (c, M_PI/4, 3*M_PI/4, false);
	cout << "Start: " << line(0) << "; mid: " << line(0.5) << "; end: " << line(1) << "; normal: " << line.normal(0) << endl;
	cout << "Projection of (1,1): " << line.project(float2(1,1)) << "; project 0,0: " << line.project(float2()) << endl;
	cout << "Side of (1,1): " << line.side_test(float2(1,1)) << "; side of (0,5): " << line.side_test(float2(0,5)) << endl;
}
