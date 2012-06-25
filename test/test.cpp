#define BOOST_TEST_MODULE BaseTypes
#include <boost/test/included/unit_test.hpp>
#include <numeric/stats.hpp>
#include <numeric/histogram.hpp>
#include <numeric/MatchTemplate.hpp>

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

    // specialisations for eigen
    base::Stats< Eigen::Vector2d > mv;
    mv.update( Eigen::Vector2d(0,1) );
    mv.update( Eigen::Vector2d(1,1) );

    BOOST_CHECK( mv.mean().isApprox( Eigen::Vector2d( 0.5, 1.0 ) ) );
    Eigen::Matrix2d t2;
    t2 << 0.25, 0, 0, 0;
    BOOST_CHECK( mv.var().isApprox( t2 ) );

    // test weighted algorithm
    // TODO
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

BOOST_AUTO_TEST_CASE(test_match_template)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);
  values.push_back(9);
  values.push_back(10);
  values.push_back(9);
  values.push_back(11);
  values.push_back(5);
  values.push_back(8);
  values.push_back(6);
  values.push_back(7);
  values.push_back(0);

  std::vector<float> vtemplate;
  vtemplate.push_back(5);
  vtemplate.push_back(4);
  vtemplate.push_back(10);
  vtemplate.push_back(9);
  vtemplate.push_back(12);
  vtemplate.push_back(9);
  vtemplate.push_back(11);
  vtemplate.push_back(5);

  int pos;
  double match;
  numeric::matchTemplate1D(values.begin(),values.end(),vtemplate.begin(),vtemplate.end(),pos,match,false);
  BOOST_CHECK_EQUAL(5,pos);
  BOOST_CHECK_EQUAL(4,match);

  numeric::matchTemplate1D(vtemplate.begin(),vtemplate.end(),values.begin(),values.end(),pos,match,false);
  BOOST_CHECK_EQUAL(-1,pos);
}

BOOST_AUTO_TEST_CASE(test_match_template2)
{
  std::vector<float> values;
  values.push_back(0);
  values.push_back(3);
  values.push_back(2);
  values.push_back(3);
  values.push_back(2);
  values.push_back(5);
  values.push_back(4);
  values.push_back(10);

  std::vector<float> vtemplate;
  vtemplate.push_back(0);
  vtemplate.push_back(3);
  vtemplate.push_back(2);
  vtemplate.push_back(3);
  vtemplate.push_back(2);
  vtemplate.push_back(5);
  vtemplate.push_back(4);
  vtemplate.push_back(10);

  int pos;
  double match;
  numeric::matchTemplate1D(values.begin(),values.end(),vtemplate.begin(),vtemplate.end(),pos,match,false);
  BOOST_CHECK_EQUAL(0,pos);
  BOOST_CHECK_EQUAL(0,match);

  vtemplate.erase(vtemplate.begin());
  numeric::matchTemplate1D(values.begin(),values.end(),vtemplate.begin(),vtemplate.end(),pos,match,false);
  BOOST_CHECK_EQUAL(1,pos);
  BOOST_CHECK_EQUAL(0,match);
}
