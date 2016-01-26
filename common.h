#ifndef COMMON_H
#define COMMON_H

#include <ostream>
#include <cmath>
#include <vector>
using namespace std;

enum BooleanOperation {INTERSECTION, UNION, DIFFERENCE};

double randf ();

/* 2D and 3D POINTS */

class float2 {
	public:
		double x,y;
		float2();
		float2(const float2&) ;
		float2(double, double);

		static float2 rand ();

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
		double arg () const;
		float2 perp_cw () const;
		float2 perp_ccw () const;

		friend ostream& operator<< (ostream&, const float2&);
};

float2 operator+ (const float2& a, const float2 &b) ;
float2 operator- (const float2& a, const float2 &b) ;

template<typename T>
float2 operator* (const float2& a, const T& b) {
	float2 res(a);
	res *= b;
	return res;
}
template<typename T>
float2 operator/ (const float2& a, const T& b) {
	float2 res(a);
	res /= b;
	return res;
}

class float3 {
	public:
		double x,y,z;

		float3();
		float3(const float3&);
		float3(double, double, double);

		static float3 rand ();

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

/* COMMON SHAPES */

class Ray {
	public:
		float3 start, dir;

		Ray(float3 s, float3 d) : start(s), dir(d) {};

		float3 operator() (double);
};

template<class P>
class CommonShape {
	public:
		bool contains (P p) const {
			return distance (p) < 0;
		}
		bool on_surface (P p) const {
			return fabs(distance(p)) < 1e-8;
		}
		double operator() (P p) const {
			return distance(p);
		}

		virtual double distance (P) const =0;
};

template<class P>
class Segment : public CommonShape<P> {
	public:
		float3 start;
		float3 end;

		Segment (P, P);
		Segment (Ray, double);
		Segment (Ray, double, double);

		double distance (P) const;

};

template<class P>
class Line : public CommonShape<P>{
	public:
		P pt;
		P dir;

		Line<P> (P, P);	// point and direction. use line_through_points for two points.
//		Line (Ray);
		Line<P> (Segment<P>);

		double distance (P) const;
};

class Plane : public CommonShape<float3> {
	public:
		float3 normal;
		double offset;

		Plane (float3, double);
		Plane (float3, float3, float3);
		Plane (Ray, double);
		Plane (Plane, double);

		bool in_positive_halfspace (float3) const;
		double distance (float3) const;

};

class Cylinder : public CommonShape <float3>{
	public:
		float3 center;
		float3 axis;
		double length;
		double radius;

		Cylinder (float3, float3, double, double);

		double distance (float3) const;

};

template<class P>
class Hypersphere : public CommonShape<P> {
	public:
		P center;
		double radius;

		Hypersphere (P c, double r) : center(c), radius(r) {};

		double distance (P) const;
};

template<class P>
double Hypersphere<P>::distance (P p) const {
	return (p - center).length() - radius;
}

typedef Hypersphere<float2> Circle;
typedef Hypersphere<float3> Sphere;

template<class P>
class AABB : public CommonShape<P> {
	public:
		P origin;
		P size;

		AABB () : origin(P()), size(P()) {};	// to make the compiler shut up
		AABB (P o, P s) : origin(o), size(s) {};
		double distance (P) const;
		bool contains (P) const;
		P random_point () const ;
};

template<class P>
double AABB<P>::distance (P p) const {
	return 0;
}

template<class P>
bool AABB<P>::contains (P p) const {
	return p >= origin && p <= origin+size;
}

template<class P>
P AABB<P>::random_point () const {
	return origin + size*P::rand();
}

#endif
