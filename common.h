#ifndef COMMON_H
#define COMMON_H

#include <ostream>
#include <cmath>
using namespace std;

enum BooleanOperation {INTERSECT, UNION, DIFFERENCE};

double randf ();

class float2 {
	public:
		double x,y;
		float2();
		float2(double, double);

		void operator+= (const float2&); 
		void operator-= (const float2&);
		void operator*= (const float2&);
		void operator*= (double);
		void operator/= (const float2&);
		void operator/= (double);

		double operator[] (int);

		double dot (const float2&) const;
		double length () const;
		float2 normalize () const;

		friend ostream& operator<< (ostream&, const float2&);
};

class float3 {
	public:
		double x,y,z;

		float3();
		float3(const float3&);
		float3(double, double, double);

		bool operator>= (const float3& a) const;
		bool operator<= (const float3& a) const;
		bool operator== (const float3& a) const;

		void operator+= (const float3&); 
		void operator-= (const float3&);
		void operator*= (const float3&);
		void operator*= (double);
		void operator/= (const float3&);
		void operator/= (double);

		double operator[] (int);

		double dot (const float3&) const;
		float3 cross (const float3&) const;
		double length () const;
		float3 normalize () const;

		friend ostream& operator<< (ostream&, const float3&);
};

float3 rand_float3 ();

float3 operator+ (const float3& a, const float3 &b) ;

float3 operator- (const float3& a, const float3 &b) ;

template<typename T>
float3 operator* (const float3& a, const T& b) {
	float3 res(a);
	res *= b;
	return res;
}
template<typename T>
float3 operator/ (const float3& a, const T& b) {
	float3 res(a);
	res /= b;
	return res;
}

class Ray {
	public:
		float3 start, dir;

		Ray(float3 s, float3 d) : start(s), dir(d) {};

		float3 operator() (double);
};

template <class Obj>
class Interpoint {
	public:
		float3 ipt;
		Obj *obj;
		double t;	// ray parameter, if desired

		Interpoint ();
		Interpoint (float3, Obj *);
		Interpoint (const Ray&, double, Obj*);
		bool is_hit () const;
		bool update (const Ray&, double, Obj*);
};


class CommonShape {
	public:
		bool contains (float3 p) const {
			return distance (p) < 0;
		}
		bool on_surface (float3 p) const {
			return fabs(distance(p)) < 1e-8;
		}
		virtual double distance (float3) const =0;
		virtual Interpoint<CommonShape> intersection (Ray) const =0;
};

class Segment : public CommonShape {
	public:
		float3 start;
		float3 end;

		Segment (float3, float3);
		Segment (Ray, double);
		Segment (Ray, double, double);

		double distance (float3) const;
		Interpoint<CommonShape> intersection (Ray) const;

};

class Line : public CommonShape {
	public:
		float3 pt;
		float3 dir;

		Line (float3, float3);	// point and direction. use line_through_points for two points.
		Line (Ray);
		Line (Segment);

		double distance (float3) const;
		Interpoint<CommonShape> intersection (Ray) const;
};

class Plane : public CommonShape{
	public:
		float3 normal;
		double offset;

		Plane (float3, double);
		Plane (float3, float3, float3);
		Plane (Ray, double);
		Plane (Plane, double);

		bool in_positive_halfspace (float3) const;
		double distance (float3) const;
		Interpoint<CommonShape> intersection (Ray) const;

};

class Cylinder : public CommonShape{
	public:
		float3 center;
		float3 axis;
		double length;
		double radius;

		Cylinder (float3, float3, double, double);

		double distance (float3) const;
		Interpoint<CommonShape> intersection (Ray) const;

};

class Sphere : public CommonShape{
	public:
		float3 center;
		double radius;

		Sphere (float3 c, double r) : center(c), radius(r) {};

		double distance (float3) const;
		Interpoint<CommonShape> intersection (Ray) const;
};

class AABB : public CommonShape {
	public:
		float3 origin;
		float3 size;

		AABB () : origin(float3()), size(float3()) {};	// to make the compiler shut up
		AABB (float3 o, float3 s) : origin(o), size(s) {};
		double distance (float3) const;
		bool contains (float3) const;
		float3 random_point () const ;
		Interpoint<CommonShape> intersection (Ray) const;
};

#endif
