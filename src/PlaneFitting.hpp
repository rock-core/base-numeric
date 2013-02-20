#ifndef BASE_NUMERIC_PLANEFITTING_HPP__
#define BASE_NUMERIC_PLANEFITTING_HPP__

#include <Eigen/Core>

namespace base
{

template<class Scalar>
class PlaneFitting
{
    Scalar x, y, z, xx, yy, xy, xz, yz, n;

    PlaneFitting() :
	x(0), y(0), z(0), xx(0), yy(0), xy(0), xz(0), yz(0), n(0) {}


};

/** 
 * calculates the slope and roughness parameters from 
 * a set of points. The function is optimized for low 
 * point counts. The minimum number of points is 3.
 */ 
void fromPoints( const std::vector<base::Vector3d>& points )
{
    double x=0, y=0, z=0, xx=0, yy=0, xy=0, xz=0, yz=0;
    // make a linear equation system based on the points
    // and solve for coefficients
    // (from: http://stackoverflow.com/questions/1400213/3d-least-squares-plane)
    for( size_t i=0; i<points.size(); i++ )
    {
	const base::Vector3d &p( points[i] );
	x += p.x();
	y += p.y();
	z += p.z();
	xx += p.x()*p.x();
	yy += p.y()*p.y();
	xy += p.x()*p.y();
	xz += p.x()*p.z();
	yz += p.y()*p.z();
    }

    Eigen::Matrix3d A;
    A << xx, xy, x,
      xy, yy, y,
      x, y, points.size();

    Eigen::Vector3d b( xz, yz, z );

    Eigen::Vector3d res = A.ldlt().solve( b );
    // the coefficients are now so that 
    // the plane equation is
    // z = (-a/c)x + (-b/c)y + (-d/c)

    slope_x = res.x();
    slope_y = res.y();
}

#endif
