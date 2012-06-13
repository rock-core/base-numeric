#define BOOST_TEST_MODULE TEST_POLY_FIT 
#include <boost/test/included/unit_test.hpp>
#include <numeric/FitPolynom.hpp>
#include <iostream>

//test if polynomial fit is working
BOOST_AUTO_TEST_CASE(test_poly_fit)
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

  std::vector<float> result;
  double chisq=0;
  numeric::fitPolynom(3,values.begin(),values.end(),result,chisq);
  BOOST_CHECK_EQUAL(3,result.size());
  BOOST_CHECK(result[0] > -1.28 && result[0] < -1.279);
  BOOST_CHECK(result[1] > 2.09 && result[1] < 2.093);
  BOOST_CHECK(result[2] > -0.1129 && result[2] < -0.1128);

  std::cout << "polynomial fit: " << result[0] << " " << result[1] << " " << result[2] << " " << sqrt(chisq) << std::endl ;
}

//test if polynomial derivation is working
BOOST_AUTO_TEST_CASE(test_derive_poly)
{
  std::vector<double> values;
  values.push_back(1);
  values.push_back(2);
  values.push_back(3);

  std::vector<double> result;
  numeric::derivePolynom(values,result);
  BOOST_CHECK_EQUAL(2,result.size());
  BOOST_CHECK(result[0] == 2.0);
  BOOST_CHECK(result[1] == 6.0);

  values.resize(1);
  numeric::derivePolynom(values,result);
  BOOST_CHECK_EQUAL(1,result.size());
  BOOST_CHECK(result[0] == 0.0);
}

//test if calculation of the roots is working
BOOST_AUTO_TEST_CASE(test_calc_poly_roots)
{
  std::vector<double> values;
  //-6+x+x^2 => 2 and -3
  values.push_back(-6);
  values.push_back(1);
  values.push_back(1);

  std::vector<double> result;
  numeric::calcPolyRoots(values,result);

  BOOST_CHECK_EQUAL(4,result.size());
  BOOST_CHECK(result[0] == 2.0);
  BOOST_CHECK(result[1] == 0.0);
  BOOST_CHECK(result[2] == -3.0);
  BOOST_CHECK(result[3] == 0.0);

  values.clear();
  result.clear();
  values.push_back(-5.84);
  values.push_back(90.36);
  values.push_back(-80.665);
  values.push_back(27.3567);
  values.push_back(-4.43239);
  values.push_back(0.343546);
  values.push_back(-0.0102124);
  BOOST_CHECK_EQUAL(0,result.size());
}

//test if calculation of the roots is working
BOOST_AUTO_TEST_CASE(test_calc_poly_val)
{
  std::vector<double> values;
  //-6+2x+x^2 
  values.push_back(-6);
  values.push_back(2);
  values.push_back(1);

  BOOST_CHECK_EQUAL(-6,numeric::calcPolyVal(values,0));
  BOOST_CHECK_EQUAL(-3,numeric::calcPolyVal(values,1));
  BOOST_CHECK_EQUAL(2,numeric::calcPolyVal(values,2));
}
