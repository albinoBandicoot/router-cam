#include "csg.h"
using namespace std;

BooleanNode::BooleanNode (BooleanOperation op) {
	this->op = op;
	children = vector<CSGNode *>();
}

BooleanNode::BooleanNode (BooleanOperation op, CSGNode *a, CSGNode *b) {
	this->op = op;
	children = vector<CSGNode *>();
	children.push_back(a);
	children.push_back(b);
}

BooleanNode::BooleanNode (BooleanOperation op, vector<CSGNode *> ch) {
	this->op = op;
	children = ch;
}

void BooleanNode::add  (CSGNode *n) {
	children.push_back (n);
}

double BooleanNode::operator() (float3 p) const {
	if (children.size() == 0) return 1;
	double d = (*children[0])(p);
	for (int i=1; i < children.size(); i++) {
		switch (op) {
			case UNION:
				d = fmin (d, (*children[i])(p));	break;
			case DIFFERENCE:
				d = fmax (d, -(*children[i])(p));	break;
			case INTERSECTION:
				d = fmax (d, (*children[i])(p));	break;
		}
	}
	return d;
}

double ExtrusionNode::operator() (float3 p) const {
	float3 q = float3 (p.dot(U), p.dot(V), p.dot(N));
	// q is now in the local coordinate system.
	q.z -= offset;
	double projdist = base->distance (float2 (q.x, q.y));
	double dz = 0;
	if (q.z <= 0) {
		dz = - q.z;
	} else if (q.z >= length) {
		dz = q.z - length;
	}
	if (dz == 0) return projdist;
	return sqrt (projdist*projdist + dz*dz);
}

