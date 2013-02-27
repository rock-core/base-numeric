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
    typedef typename Eigen::Matrix<Scalar,3,1> Vector3;
    typedef typename Eigen::Matrix<Scalar,3,3> Matrix3;

    Scalar x, y, z, xx, yy, xy, xz, yz, zz, n;

    PlaneFitting() :
	x(0), y(0), z(0), xx(0), yy(0), xy(0), xz(0), yz(0), zz(0), n(0) {}

    explicit PlaneFitting( const Vector3& p, Scalar weight = 1.0 ) :
	x(p.x() * weight),
	y(p.y() * weight),
	z(p.z() * weight),
	xx(p.x()*p.x() * weight),
	yy(p.y()*p.y() * weight),
	xy(p.x()*p.y() * weight),
	xz(p.x()*p.z() * weight),
	yz(p.y()*p.z() * weight),
	zz(p.z()*p.z() * weight),
	n(weight)
    {
    }

    /**
     * @brief clears all previous input to the update method 
     */
    void clear()
    {
	x = y = z = xx = yy = xy = xz = yz = zz = n = 0;
    }

    void update( const PlaneFitting& other )
    {
	x += other.x;
	y += other.y;
	z += other.z;
	xx += other.xx;
	yy += other.yy;
	xy += other.xy;
	xz += other.xz;
	yz += other.yz;
	zz += other.zz;
	n += other.n;
    }

    void update( const Vector3& p, Scalar weight = 1.0 )
    {
	update( PlaneFitting( p, weight ) );
    }

    /**
     * @brief Get the coefficients of the fitted plane. 
     *
     * will return all zeros if no input was given to the update() method.
     *
     * Note this function will call the solve function internally.
     * If you need both coefficients and covariance matrix it is
     * more efficient to call solve directly.
     *
     * @result is a three vector <a,b,c> such that the plane is given
     * by z = a * x + b * y + c
     */
    void solve( Vector3& coeffs, Matrix3* covariance = NULL ) const
    {
	// make a linear equation system based on the points
	// and solve for coefficients
	// (from: http://stackoverflow.com/questions/1400213/3d-least-squares-plane)
	Matrix3 A;
	A << xx, xy, x,
	  xy, yy, y,
	  x, y, n;

	Vector3 b( xz, yz, z );

	Eigen::LDLT<Matrix3> ldlt( A );
	coeffs = ldlt.solve( b );
	// the coefficients are now so that 
	// the plane equation is
	// z = (-a/c)x + (-b/c)y + (-d/c)
	
	// if requested, calculate the covariance matrix
	if( covariance )
	{
	    Matrix3 &cov(*covariance);
	    Scalar residuals = 
		zz - 2 * b.transpose() * coeffs +
		coeffs.transpose() * A * coeffs;

	    cov = residuals * ldlt.solve( Matrix3::Identity() );
	}
    }

    /**
     * @brief Get the coefficients of the fitted plane. 
     *
     * will return all zeros if no input was given to the update() method.
     *
     * Note this function will call the solve function internally.
     * If you need both coefficients and covariance matrix it is
     * more efficient to call solve directly.
     *
     * @result is a three vector <a,b,c> such that the plane is given
     * by z = a * x + b * y + c
     */
    Vector3 getCoeffs() const
    {
	Vector3 res; 
	solve( res );
	return res;
    }

    /**
     * @brief calculate the covariance matrix
     *
     * Note this function will call the solve function internally.
     * If you need both coefficients and covariance matrix it is
     * more efficient to call solve directly.
     *
     * @result coefficient covariance matrix
     */
    Matrix3 getCovariance() const
    {
	Vector3 coeffs;
	Matrix3 cov;
	solve( coeffs, cov );
	return cov;
    }
};

}

#endif
