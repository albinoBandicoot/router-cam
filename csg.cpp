#include "csg.h"
using namespace std;

		BooleanOperation op;
		vector<CSGNode*> children;

BooleanNode::BooleanNode (BooleanOperation op) {
	this.op = op;
	children = vector<CSGNode *>();
}

BooleanNode::BooleanNode (BooleanOperation op, vector<CSGNode *> ch) {
	this.op = op;
	children = ch;
}

void BooleanNode::add  (CSGNode *n) {
	children.push_back (n);
}

double BooleanNode::operator() (float3 p) {
	if (children.size() == 0) return 1;
	double d = children[0](p);
	for (int i=1; i < children.size(); i++) {
		switch (op) {
			case UNION:
				d = fmin (d, children[i](p));	break;
			case DIFFERENCE:
				d = fmax (d, -children[i](p));	break;
			case INTERSECT:
				d = fmax (d, children[i](p));	break;
		}
	}
	return d;
}

double SphereNode::operator() (float3 p) {
	return s.distance(p);
}

#endif
