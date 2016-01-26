#ifndef CSG_H
#define CSG_H

#include <ostream>
#include <vector>
#include "common.h"
#include "gpoly.h"
using namespace std;

class CSGNode {
	public:
		virtual double operator() (float3) const =0;
};


class BooleanNode : public CSGNode {
	public:
		BooleanOperation op;
		vector<CSGNode*> children;

		BooleanNode (BooleanOperation);
		BooleanNode (BooleanOperation, CSGNode *, CSGNode *);
		BooleanNode (BooleanOperation, vector<CSGNode*>);

		double operator() (float3) const;
		void add (CSGNode*);

};

class ExtrusionNode : public CSGNode {
	public:
		GPoly *base;
		float3 U, V, N;	// U,V form basis for plane of base. Normal N is U x V
		double offset;
		double length;

		ExtrusionNode (GPoly *b, float3 u = float3(1,0,0), float3 v = float3(0,1,0), double off=0, double len=1) : base(b), U(u.normalize()), V(v.normalize()), N(U.cross(V)), offset(off), length(len) {};

		double operator() (float3) const;
};

template <class S>
class ShapeNode : public CSGNode {
	public:
		S sh;

		ShapeNode (S s) : sh(s) {};

		double operator() (float3 p) const {
			return sh.distance(p);
		}
};

#endif
