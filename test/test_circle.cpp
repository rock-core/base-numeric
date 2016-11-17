#include <boost/test/unit_test.hpp>
#include <numeric/Circle.hpp>

using namespace numeric;

BOOST_AUTO_TEST_SUITE(circle)

BOOST_AUTO_TEST_CASE(same_center)
{
    Circle unit; 
    Circle unit2;
    //same circles have infinit number of intersections
    BOOST_CHECK(unit.intersect(unit2).size() == 0);
}

BOOST_AUTO_TEST_CASE(no_intersection)
{
    Circle unit; 
    Circle circle2(2, 0, 0.3);
    //same circles have infinit number of intersections
    BOOST_CHECK(unit.intersect(circle2).size() == 0);
}


BOOST_AUTO_TEST_CASE(simple)
{
    Circle c1; 
    Circle c2(0.5, 0, 1);
    
    //same circles have infinit number of intersections
    const auto result = c1.intersect(c2);
    BOOST_CHECK(result.size() == 2);
    BOOST_CHECK_CLOSE(result[0].x(), 0.25, 0.001);
    BOOST_CHECK_CLOSE(result[1].x(), 0.25, 0.001);
    BOOST_CHECK_CLOSE(result[1].y(), -0.96824, 0.001);
    BOOST_CHECK_CLOSE(result[0].y(), 0.96824, 0.001);
}


BOOST_AUTO_TEST_SUITE_END() 