#define BOOST_TEST_MODULE BaseTypes
#include <boost/test/included/unit_test.hpp>
#include <numeric/Stats.hpp>
#include <numeric/Histogram.hpp>
#include <numeric/MatchTemplate.hpp>
#include <numeric/PlaneFitting.hpp>

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
    
    // test base::VectorXd
    base::Stats <base::VectorXd> xv;
    base::MatrixXd x_data(2,3);
    x_data << 0.0, -1.0, 1.0, 1.0, 0.0, 1.0;
    xv.update(x_data.col(0));
    xv.update(x_data.col(1));
    xv.update(x_data.col(2));
    
    base::VectorXd xmean(2);
    xmean << 0.0, 2.0/3.0;
    base::MatrixXd xvar(2,2);
    xvar << 2.0/3.0, 1.0/3.0, 1.0/3.0, 2.0/9.0;
    base::VectorXd xstd(2);
    xstd << 0.81649658, 0.47140452;

    BOOST_CHECK( xv.n() == 3);
    BOOST_CHECK( xv.mean().isApprox(xmean) );
    BOOST_CHECK( xv.var().isApprox(xvar) );
    BOOST_CHECK( xv.stdev().isApprox(xstd,6) );
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

BOOST_AUTO_TEST_CASE( planefitting_test )
{
    // fully specified on xy plane
    base::PlaneFitting<float> pf;
    pf.update( Eigen::Vector3f( 0.0, 0, -1.0 ) );
    pf.update( Eigen::Vector3f( 1.0, 0, -1.0 ) );
    pf.update( Eigen::Vector3f( 0.0, 1.0, -1.0 ) );
    Eigen::Vector3f r1 = pf.getCoeffs();
    BOOST_CHECK_SMALL( r1.x(), 1e-6f );
    BOOST_CHECK_SMALL( r1.y(), 1e-6f );
    BOOST_CHECK_CLOSE( r1.z(), -1.0, 1e-6 );

    // underspecified (returns xy plane)
    pf.clear();
    pf.update( Eigen::Vector3f( 0.0, 0, -1.0 ) );
    pf.update( Eigen::Vector3f( 1.0, 0, -1.0 ) );
    Eigen::Vector3f r2 = pf.getCoeffs();
    BOOST_CHECK_SMALL( r2.x(), 1e-4f );
    BOOST_CHECK_SMALL( r2.y(), 1e-4f );
    BOOST_CHECK_CLOSE( r2.z(), -1.0, 1e-4 );

    // check slope on y axis and give unequal weights
    // the weights shouldnt affect the slope in this case
    pf.clear();
    pf.update( Eigen::Vector3f( 0.0, 0, -1.0 ), 1.0 );
    pf.update( Eigen::Vector3f( 1.0, 0, -1.0 ), 1.0 );
    pf.update( Eigen::Vector3f( 0.0, 1.0, 0.0 ), 0.1 );
    Eigen::Vector3f r3 = pf.getCoeffs();
    // need to go down here with the accuracy
    // seems to be the solver which is happy
    // this way
    BOOST_CHECK_SMALL( r3.x(), 1e-4f );
    BOOST_CHECK_CLOSE( r3.y(), 1.0, 1e-4 );
    BOOST_CHECK_CLOSE( r3.z(), -1.0, 1e-4 );

    // this will actually yield a vector of all zeros
    pf.clear();
    Eigen::Vector3f r4 = pf.getCoeffs();
    BOOST_CHECK_SMALL( r4.x(), 1e-4f );
    BOOST_CHECK_SMALL( r4.y(), 1e-4f );
    BOOST_CHECK_SMALL( r4.z(), 1e-4f );

    // check the covariance matrix for
    {
    pf.clear();
    pf.update( Eigen::Vector3f( 0.0, 0, -1.0 ), 0.5 );
    pf.update( Eigen::Vector3f( 0.0, 0, 1.0 ), 0.5 );
    base::PlaneFitting<float>::Result res = pf.solve();

    BOOST_CHECK_CLOSE( res.getCovariance()(2,2), 1.0, 1e-4 );
    }
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

BOOST_AUTO_TEST_CASE(test_join_vectors)
{
  std::vector<float> v1;
  v1.push_back(0);
  v1.push_back(3);
  v1.push_back(2);
  v1.push_back(2);
  v1.push_back(5);
  v1.push_back(4);
  v1.push_back(10);

  std::vector<float> v2;
  v2.push_back(11);
  v2.push_back(12);
  v2.push_back(13);
  v2.push_back(14);
  v2.push_back(15);
  v2.push_back(16);
  v2.push_back(17);
  v2.push_back(18);
  v2.push_back(19);
  v2.push_back(20);

  //copy it to the end of v1
  std::vector<float> result = numeric::joinVectors(v1,v2,(int) v1.size(),0.0F);
  std::vector<float> result2 = v1;
  result2.resize(v1.size()+v2.size());
  std::copy(v2.begin(),v2.end(),result2.begin()+v1.size());
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));

  //copy it to the end of v2
  result = numeric::joinVectors(v1,v2,-((int)v2.size()),0.0F);
  result2 = v2;
  result2.resize(v1.size()+v2.size());
  std::copy(v1.begin(),v1.end(),result2.begin()+v2.size());
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));

  //hole between v1 and v2
  result = numeric::joinVectors(v1,v2,(int) v1.size()+10,0.0F);
  result2 = v1;
  result2.resize(v1.size()+v2.size()+10);
  std::fill(result2.begin()+v1.size(),result2.begin()+v1.size()+10,0.0F);
  std::copy(v2.begin(),v2.end(),result2.begin()+v1.size()+10);
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));

  //hole between v2 and v1
  result = numeric::joinVectors(v1,v2,-((int)v2.size()+10),0.0F);
  result2 = v2;
  result2.resize(v1.size()+v2.size()+10);
  std::fill(result2.begin()+v2.size(),result2.begin()+v2.size()+10,0.0F);
  std::copy(v1.begin(),v1.end(),result2.begin()+v2.size()+10);
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));
  
  //half overlapping (left side) 
  result = numeric::joinVectors(v1,v2,-5,0.0F);
  result2 = v2;
  result2.resize(-5+v1.size()+v2.size());
  std::copy(v1.begin()+(v2.size()-5),v1.end(),result2.begin()+v2.size());
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));
  
  //half overlapping (right side) 
  result = numeric::joinVectors(v1,v2,5,0.0F);
  result2 = v1;
  result2.resize(v2.size()+5);
  std::copy(v2.begin(),v2.end(),result2.begin()+5);
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));
  
  //fully overlapping (v1 < v2) 
  result = numeric::joinVectors(v1,v2,-1,0.0F);
  result2 = v2;
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));

  result = numeric::joinVectors(v2,v1,1,0.0F);
  result2 = v2;
  std::copy(v1.begin(),v1.end(),result2.begin()+1);
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));
  
  //fully overlapping (v1 = v2) 
  v2.resize(v1.size());
  result = numeric::joinVectors(v1,v2,0,0.0F);
  result2 = v2;
  BOOST_CHECK_EQUAL(result.size(),result2.size());
  BOOST_CHECK_EQUAL(true,std::equal(result.begin(),result.end(),result2.begin()));
}
