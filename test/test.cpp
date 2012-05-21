#define BOOST_TEST_MODULE BaseTypes
#include <boost/test/included/unit_test.hpp>
#include <numeric/stats.hpp>
#include <numeric/histogram.hpp>

BOOST_AUTO_TEST_CASE( stats_test )
{
    // singular value statistics
    base::Stats<double> sv;
    sv.update( 1.0 );
    sv.update( 2.0 );
    sv.update( 3.0 );
    BOOST_CHECK_EQUAL( sv.mean(), 2.0 );
    BOOST_CHECK_CLOSE( sv.var(), 2.0/3.0, 1e-6 );
    BOOST_CHECK_CLOSE( sv.stdev(), sqrt(2.0/3.0), 1e-6 );
    BOOST_CHECK_EQUAL( sv.min(), 1.0 );
    BOOST_CHECK_EQUAL( sv.max(), 3.0 );

    /*
    // specialisations for eigen
    base::Stats< Eigen::Vector2d > mv;
    sv.update( Eigen::Vector2d(0,1) );
    sv.update( Eigen::Vector2d(1,1) );

    BOOST_CHECK( sv.mean().isApprox( Eigen::Vector2d( 0.5, 1.0 ) ) );
    */
}

BOOST_AUTO_TEST_CASE( histogram_test )
{
    base::Histogram h( 10, 0.0, 10.0 );

    BOOST_CHECK_CLOSE( h.getLowerBound( 0 ), 0, 1e-6 );
    BOOST_CHECK_CLOSE( h.getUpperBound( 0 ), 1.0, 1e-6 );
    BOOST_CHECK_CLOSE( h.getUpperBound( 9 ), 10.0, 1e-6 );

    h.update( 0.5 );
    BOOST_CHECK_EQUAL( h[0], 1 );
    h.update( -0.5 );
    BOOST_CHECK_EQUAL( h[0], 2 );
    h.update( 8.5 );
    BOOST_CHECK_EQUAL( h[8], 1 );
    h.update( 9.5 );
    BOOST_CHECK_EQUAL( h[9], 1 );
    h.update( 10.5 );
    BOOST_CHECK_EQUAL( h[9], 2 );
}
