#include "ovm.h"
#include <iostream>
#include <cmath>
using namespace std;

template<typename T>
string bits (T x) {
	string s;
	uint8_t *base = ((uint8_t *) &x);
	for (int i=sizeof(x) - 1; i >= 0; i--) {
		for (int j=7; j >= 0; j--) {
			s += base[i] & (1 << j) ? "1" : "0";
		}
	}
	return s;
}

/* OVM Index methods */
ovm_idx3::ovm_idx3 (const ovm_tree& t, float3 p) {
	*this = t.to_idx (p);
}

#define BIT(X,N) ((X >> N) & 1)
ovm_idx3 ovm_idx3::child (int i) const {
	ovm_idx3 res = *this;
	if (BIT(i, 2))	res.z |= (1 << res.level);
	if (BIT(i, 1))	res.y |= (1 << res.level);
	if (BIT(i, 0))	res.x |= (1 << res.level);
	res.down();
	return res;
}


uint8_t ovm_idx3::child_index () const {
	return (BIT(z,level) << 2) | (BIT(y, level) << 1) | BIT(x, level);
}

uint8_t ovm_idx3::leaf_zidx () const {
//	return z & 7;
	return (BIT(z,2) << 2) | (BIT(y, 2) << 1) | BIT(x, 2);
}

uint8_t ovm_idx3::leaf_xyidx () const {
	uint8_t rx = (BIT(z,1) << 2) | (BIT(y, 1) << 1) | BIT(x, 1);
	uint8_t ry = (z & 1) << 2 | (y & 1) << 1 | (x & 1);
	return (ry << 3) | rx;
//	return ((y & 7) << 3) | (x & 7);
}

ovm_idx3::operator float3 () const {
	return float3 (x >> level, y >> level, z >> level);
}

void ovm_idx3::up (int i) {
	level += i;
}

void ovm_idx3::down (int i) {
	level -= i;
}

std::ostream& operator<< (std::ostream& o, const ovm_idx3& p) {
	return o << "[" << bits(p.x) << ", " << bits(p.y) << ", " << bits(p.z) << "]@" << p.level;
}

/* OVM TREE methods */

ovm_tree::ovm_tree (AABB<float3> bbox, int ml) {
	rootbox = bbox;
	root_level = ml;
	root = new internal_node(ovm_idx3(0,0,0,root_level));
}

ovm_tree::ovm_tree (const ovm_tree& t) {
	rootbox = t.rootbox;
	root_level = t.root_level;
	root = new internal_node(ovm_idx3(0,0,0,root_level));
}

ovm_tree::ovm_tree (AABB<float3> bbox, int ml, CSGNode *csg) {
	rootbox = bbox;
	root_level = ml;
	root = new internal_node(ovm_idx3(0,0,0,root_level));
	root->model_isosurface (*this, *csg);
}

ovm_idx3 ovm_tree::to_idx (float3 p) const {
	p = (p - rootbox.origin) /rootbox.size * ((double) (1 << (root_level+1)));
	return ovm_idx3 ((ovm_idx) p.x, (ovm_idx) p.y, (ovm_idx) p.z, root_level);
}

float3 ovm_tree::voxel_size (uint8_t level) const {
	return rootbox.size / (double) (1 << (root_level - level + 1));
}

float3 ovm_tree::voxel_corner (ovm_idx3 i) const {
	return rootbox.origin + (((float3) i) * voxel_size(i.level));
}

float3 ovm_tree::voxel_center (ovm_idx3 i) const {
	return rootbox.origin + ((float3(i) + float3(0.5, 0.5, 0.5))* voxel_size(i.level));
}

bool ovm_tree::contains (float3 p) const {
	if (rootbox.contains (p)) {
		return root->contains (to_idx(p));
	}
	return false;
}

void ovm_tree::boolean_op (const ovm_tree& t, BooleanOperation op) {
}

void ovm_tree::coalesce () {
	root->coalesce();
}

int ovm_tree::count_nodes (bool leaves_only=false) const {
	return root->count_nodes(leaves_only);
}

/* Methods on NODES */

ovm_node::~ovm_node () {
}

leaf_node::leaf_node (ovm_idx3 p) {
	memset (&voxels[0], 0, 64);
	pos = p;
}

leaf_node::~leaf_node () {
}

internal_node::internal_node (ovm_idx3 p) {
	// empty, truncated node
	ch_truncate = 0xff;
	ch_inside = 0;
	pos = p;
}

internal_node::~internal_node () {
	for (int i=0; i < 8; i++) {
		if (has_child(i)) {
			delete ch[i];
		}
	}
}

ostream& operator<< (ostream& o, const leaf_node& p) {
	 return o << "LEAF at " << p.pos << endl;
}

ostream& operator<< (ostream& o, const internal_node& p) {
	 return o << "INTERNAL at " << p.pos << "; trunc: " << bits(p.ch_truncate) << "; inside: " << bits(p.ch_inside) << endl;
}


// subdivide child number i, return pointer to newly constructed node
ovm_node* internal_node::subdivide (int i) {
//	cout << "Will subdivide child " << i << "of node at " << pos << "; new pos = " << pos.child(i) << endl;
//	cout << "Current node is " << *this << endl;
	if (has_child (i)) {
		cerr << "Warning! already subdivided." << endl;
		// error: already subdivided! (or silently ignore??)
	} else {
		ch_truncate &= ~(1 << i);
		if (pos.level == 3) {
			ch[i] = new leaf_node(pos.child(i));
		} else {
			ch[i] = new internal_node(pos.child(i));
		}
	}
	return ch[i];
}

// returns 0 for no, 1 for yes (outside), 2 for yes (inside)
int leaf_node::can_coalesce () const {
	if (voxels[0] != 0 && voxels[0] != UINT64_MAX) return 0;
	for (int i=1; i < 8; i++) {
		if (voxels[i] != voxels[0]) {
			return 0;
		}
	}
	return voxels[0] == 0 ? 1 : 2;
}

int internal_node::can_coalesce () const {
	if (ch_truncate != 255) return 0;
	return ch_inside == 0 ? 1 : 2;
}

bool internal_node::has_child (int i) const {
	return (ch_truncate & (1 << i)) == 0;
}

void leaf_node::coalesce () {
}

void internal_node::coalesce () {
	for (int i=0; i < 8; i++) {
		if (has_child(i)) {
			ch[i]->coalesce();
		}
	}
	// the work happens here (we want post-order traversal)
	for (int i=0; i < 8; i++) {
		if (has_child(i)) {
			int cc = ch[i]->can_coalesce();
			if (cc) {
				ch_truncate |= (1 << i);
				delete ch[i];
				if (cc == 1) {
					ch_inside &= ~(1 << i);
				} else {
					ch_inside |= (1 << i);
				}
			}
		}
	}
}

bool TRACE = false;

bool leaf_node::contains (ovm_idx3 p) const {
	if (TRACE) {
		cout << "Leaf check on " << p << "; zidx = " << p.leaf_zidx() << "; xyidx = " << p.leaf_xyidx() << endl;
		cout << "\tvoxels[z] = " << bits(voxels[p.leaf_zidx()]) << endl;
	}
	return (voxels[p.leaf_zidx()] & (1 << p.leaf_xyidx())) > 0;
}

bool internal_node::contains (ovm_idx3 p) const {
	uint8_t i = p.child_index();
	if (TRACE) {
		cout << "Checking " << p << " at " << *this << endl << "\twould be in child " << (int) i << endl;
	}
	if (has_child(i)) {
//		cout << "\trecurse" << endl;
		p.down();
		return ch[i]->contains (p);
	} else {
		return (ch_inside & (1 << i)) > 0;
	}
}

void leaf_node::model_isosurface (const ovm_tree &t, const CSGNode& csg) {
	float3 corner = t.voxel_corner (pos);
	float3 size = t.voxel_size (0);
	corner += size/2;
	double semidiag = size.length() / 1;	// this is for smallest level

	/*
	if (dist (t.voxel_center (pos)) <= 0) {
		memset (&voxels[0], 255, 64);
	} else {
		memset (&voxels[0], 0, 64);
	}
	return;
	*/
//	cout << "LEAF at " << pos << " ==> " << corner << endl;
	for (int z=0; z < 8; z++) {
		voxels[z] = 0;
		float3 vc = t.voxel_center (pos.child(z));
		double d = csg(vc);
		if (fabs(d) >= semidiag * 4) {
			if (d <= 0) {
				voxels[z] = UINT64_MAX;
			}
		} else {
			for (int y=0; y < 8; y++) {
				vc = t.voxel_center (pos.child(z).child(y));
				d = csg(vc);
				if (fabs(d) >= semidiag * 2) {
					if (d <= 0) {
						voxels[z] |= (255ull << (y*8));
					}
				} else {
					for (int x=0; x < 8; x++) {
						float3 p = corner + float3(x,y,z)*size;
						if (csg(p) <= 0) {
							voxels[z] |= (1 << (y*8 + x));
						}
					}
				}
			}
		}
	}
}

void internal_node::model_isosurface (const ovm_tree& t, const CSGNode& csg) {
	for (int i=0; i < 8; i++) {
		float3 cc = t.voxel_center (pos.child(i));
		double d = csg(cc);
		if (fabs(d) < t.voxel_size (pos.level).length() / 1) {
			subdivide (i)->model_isosurface (t, csg);
		} else {
			if (d < 0) {
				ch_inside |= (1 << i);
			} else {
				ch_inside &= ~(1 << i);
			}
		}
	}
}

int leaf_node::count_nodes (bool leaf_only=false) const {
	return 1;
}

int internal_node::count_nodes (bool leaf_only=false) const {
	int ct = leaf_only ? 0 : 1;
	for (int i=0; i < 8; i++) {
		if (has_child (i)) {
			ct += ch[i]->count_nodes(leaf_only);
		}
	}
	return ct;
}

void test (const ovm_tree& t, int npoints, const CSGNode &csg) {
	long time = clock();
	int errct = 0;
	double avg_err = 0;
	double max_err = 0;
	for (int i=0; i < npoints; i++) {
		float3 p = t.rootbox.random_point();
		double d = csg(p);
		double err = fabs(d);
		bool ovm_inside = t.contains (p);
		bool d_inside = d <= 0;
		if (ovm_inside != d_inside) {
			errct ++;
			avg_err += err;
			max_err = fmax(max_err, err);
		}
	}
	cout << "Tested " << npoints << "points (" << ((double) clock() - time)/CLOCKS_PER_SEC << " sec); " << errct << " errors; avg_err = " << (avg_err / errct) << "; max_err = " << max_err << endl;
}

int main () {
	//	ovm_tree t (AABB(float3(-1, -1, -1), float3(2,2,2)), 4);
	CSGNode *csg = new BooleanNode (UNION, new ShapeNode<Sphere> (Sphere(float3(0,0.5,0), 1)), new ShapeNode<Sphere> (Sphere(float3(0,0,-0.2), 0.8)));
	for (int i=10; i < 11; i++) {
		long time = clock();
		cout << "Will make tree with level " << i << " subdivision" << endl;
		ovm_tree t (AABB<float3>(float3(-1, -1, -1), float3(2,2,1.97)), i, csg);
		cout << "\tTook " << ((double) clock() - time)/CLOCKS_PER_SEC << " sec" << endl;
		cout << "\tRaw: " << t.count_nodes() << " nodes, " << t.count_nodes(true) << " leaves." << endl;
		test (t, 5000000, *csg);
		t.coalesce();
		cout << "\tCoalesced: " << t.count_nodes() << " nodes, " << t.count_nodes(true) << " leaves." << endl;
		test (t, 5000000, *csg);
	}
}
