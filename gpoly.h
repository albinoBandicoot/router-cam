#ifndef GPOLY_H
#define GPOLY_H

#include <ostream>
#include <cmath>
#include <vector>
#include "common.h"
using namespace std;

class GSeg;

class GVertex {
	public:
		const float2 pos;

		GVertex (float2 p) : pos(p) {};
		double distance (float2) const;
};

class GSeg {
	public:
		GVertex *start;
		GVertex *end;

		// segments are parametric with t in [0,1]. This evaluates the curve.
		virtual float2 operator() (double) const =0;
		// normal is defined as pointing outwards, towards positive distance; the normal
		// should be the gradient of the distance function along the curve.
		virtual float2 normal (double) const =0;
		// find t-value of projection
		virtual double project (float2) const =0;

		// returns 1 if on positive distance side, -1 if not
		virtual int side_test (float2) const =0;

		double distance (float2 p) const {
			double t = project (p);
			if (t >= 0 && t <= 1) {
				return (p - (*this)(t)).length();
			} else {
				return fmin (start->distance (p), end->distance(p));
			}
		}
};

class GLineseg : public GSeg {
	private:
		float2 vec;
		float2 norm;
		double length;

		void recompute ();
	public:
		GLineseg (float2, float2);
		GLineseg (GVertex*, GVertex*);

		float2 operator() (double) const;
		float2 normal (double) const;
		double project (float2) const;
		int side_test (float2) const;
};

class GCircle : public GSeg {
	public:
		Circle c;

		float2 operator() (double) const;
		int side_test (float2) const;
};

class GArc : public GCircle {
	public:
		float2 operator() (double) const;
		int side_test (float2) const;
};

struct ovm_tree;

class GPoly : public CommonShape<float2> {
	private:
		vector<GVertex> pts;
		vector<GSeg> segs;
		vector<GPoly *> holes;

	public:
		GPoly ();
		GPoly (const GPoly&);
		GPoly (vector<GSeg>);
		GPoly (vector<float2>);
		GPoly (const ovm_tree&, float3 plane);	// expecting only one nonzero component, which defines the offset on that axis. Perhaps add a smoothing parameter

		GSeg operator[] (int) const;
		void add_hole (GPoly *);
		bool check() const;
		double distance (float2) const;
		void tesselateArcs (double max_error);

		GPoly inset (double) const;
		GPoly boolean_op (const GPoly&, BooleanOperation) const;
};
#endif
