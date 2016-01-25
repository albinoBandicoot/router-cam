#ifndef CSG_H
#define CSG_H

#include <ostream>
#include <vector>
#include "common.h"
#include "ovm.h"
using namespace std;

class CSGNode {
	public:

		virtual double operator() (float3) const =0;

		/*
		virtual bool contains (float3 p) =0;
		virtual bool on_surface (float3 p) =0;
		virtual float3 intersection (Ray r) =0;
		*/

		virtual ovm_tree getOVM (AABB, int) =0;
};


class BooleanNode : public CSGNode {
	public:
		BooleanOperation op;
		vector<CSGNode*> children;

		BooleanNode (BooleanOperation);
		BooleanNode (BooleanOperation, vector<CSGNode*>);

		double operator() (float3) const;
		void add (CSGNode*);

};

class ExtrusionNode : public CSGNode {
	public:
		float3 axis;
		double offset;
		double length;

		double operator() (float3) const;
};

class SphereNode : public CSGNode {
	public:
		Sphere s;

		double operator() (float3) const;
};
#endif
