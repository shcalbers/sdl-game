#ifndef PHYSICS_HEADER
#define PHYSICS_HEADER

#include <complex>
using namespace std;

//template<typename T>
struct Point2D
{
	float x, y;//T x, y;
};

//template<typename T>
using Vector2D = complex<float>;//T>;



//template<typename T>
//operator Point2D/*<T>*/(const Vector2D/*<T>*/ & vector)
//{
//	return Point2D/*<T>*/{ vector.real(), vector.imag() };
//}

//template<typename T>
//operator Vector2D/*<T>*/(const Point2D/*<T>*/ & point)
//{
//	return Vector2D/*<T>*/{ point.x, point.y };
//}

#endif
