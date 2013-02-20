#ifndef BASE_NUMERIC_PLANEFITTING_HPP__
#define BASE_NUMERIC_PLANEFITTING_HPP__

#include <Eigen/Core>
#include <Eigen/Cholesky>

namespace base
{

/** 
 * Performs a linear least squares regression of a plane to a set of points in
 * 3D space. The points are added incrementally using the update method, and can
 * be weighted according to their contribution to the fit. 
 * A call to getCoeffs() will solve the regression and return the coefficients 
 * of the fitted plane. Note that the linear least squares fit is over the
 * distance on the z-axis, and not actually the shortest distance to the plane.
 */
template<class Scalar>
class PlaneFitting
{
public:
    Scalar x, y, z, xx, yy, xy, xz, yz, n;

    PlaneFitting() :
	x(0), y(0), z(0), xx(0), yy(0), xy(0), xz(0), yz(0), n(0) {}

    /**
     * @brief clears all previous input to the update method 
     */
    void clear()
    {
	x = y = z = xx = yy = xy = xz = yz = n = 0;
    }

    void update( const typename Eigen::Matrix<Scalar, 3, 1>& p, Scalar weight = 1.0 )
    {
	x += p.x() * weight;
	y += p.y() * weight;
	z += p.z() * weight;
	xx += p.x()*p.x() * weight;
	yy += p.y()*p.y() * weight;
	xy += p.x()*p.y() * weight;
	xz += p.x()*p.z() * weight;
	yz += p.y()*p.z() * weight;
	n += weight;
    }

    /**
     * @brief Get the coefficients of the fitted plane. 
     *
     * will return all zeros if no input was given to the update() method.
     *
     * @result is a three vector <a,b,c> such that the plane is given
     * by z = a * x + b * y + c
     */
    typename Eigen::Matrix<Scalar, 3, 1> getCoeffs() const
    {
	// make a linear equation system based on the points
	// and solve for coefficients
	// (from: http://stackoverflow.com/questions/1400213/3d-least-squares-plane)
	Eigen::Matrix<Scalar,3,3> A;
	A << xx, xy, x,
	  xy, yy, y,
	  x, y, n;

	Eigen::Matrix<Scalar,3,1> b( xz, yz, z );

	Eigen::Matrix<Scalar,3,1> res = A.ldlt().solve( b );
	// the coefficients are now so that 
	// the plane equation is
	// z = (-a/c)x + (-b/c)y + (-d/c)
	
	return res;
    }
};

}

#endif
