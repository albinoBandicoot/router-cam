#ifndef OVM_H
#define OVM_H

#include "common.h"
#include "csg.h"

typedef uint16_t ovm_idx;

struct ovm_tree;
struct ovm_node;
struct leaf_node;
struct internal_node;

struct ovm_idx3 {
	ovm_idx x,y,z,level;

	ovm_idx3 () : x(0), y(0), z(0), level(0) {};
	ovm_idx3 (ovm_idx a, ovm_idx b, ovm_idx c) : x(a), y(b), z(c), level(0) {};
	ovm_idx3 (ovm_idx a, ovm_idx b, ovm_idx c, ovm_idx lev) : x(a), y(b), z(c), level(lev) {};
	ovm_idx3 (const ovm_tree&, float3);

	ovm_idx3 child (int i) const;
	operator float3 () const;
	uint8_t child_index () const;
	uint8_t leaf_zidx () const;
	uint8_t leaf_xyidx () const;

	void down(int i=1) ;
	void up(int i=1) ;

	friend ostream& operator<< (ostream& o, const ovm_idx3& p) ;
};

struct ovm_tree {
	AABB<float3> rootbox;
	int root_level;
	internal_node *root;

	ovm_tree (AABB<float3>, int);
	ovm_tree (const ovm_tree&);	// create a blank tree with same box and subdivision
	ovm_tree (AABB<float3>, int, CSGNode *);

	ovm_idx3 to_idx (float3) const;

	float3 voxel_size (uint8_t level) const;
	float3 voxel_corner (ovm_idx3 i) const;
	float3 voxel_center (ovm_idx3 i) const;
	bool contains (float3) const;	// point test

	int count_nodes(bool) const;

	void boolean_op (const ovm_tree&, BooleanOperation);
	void coalesce ();
};

struct ovm_node {
	ovm_idx3 pos;

	virtual ~ovm_node ();

	virtual void coalesce () =0;
	virtual int can_coalesce () const =0;
	virtual bool contains (ovm_idx3) const =0;
	virtual void model_isosurface (const ovm_tree&, const CSGNode&) =0;
	virtual int count_nodes (bool) const =0;
};

struct leaf_node : public ovm_node {
	uint64_t voxels[8];

	leaf_node (ovm_idx3 p);
	~leaf_node ();

	bool contains (ovm_idx3) const;
	void coalesce();
	int can_coalesce () const;
	void model_isosurface (const ovm_tree&, const CSGNode&);
	int count_nodes (bool) const ;
	friend ostream& operator<< (ostream& o, const leaf_node& p) ;
};

struct internal_node : public ovm_node {
	ovm_node *ch[8];
	uint8_t ch_truncate;
	uint8_t ch_inside;

	internal_node (ovm_idx3 p);
	~internal_node ();

	bool has_child (int) const;
	ovm_node* subdivide(int ch);
	bool contains (ovm_idx3) const;
	void coalesce();
	int can_coalesce () const;
	void model_isosurface (const ovm_tree&, const CSGNode&);
	int count_nodes(bool) const;
	friend ostream& operator<< (ostream& o, const internal_node& p) ;
};

#endif
