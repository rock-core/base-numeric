#include <boost/test/unit_test.hpp>
#include <numeric/Circle.hpp>
#include <iostream>

using namespace numeric;

BOOST_AUTO_TEST_SUITE(circle)

BOOST_AUTO_TEST_CASE(same_center)
{
    Circle unit = Circle::Unit(); 
    Circle unit2 = Circle::Unit();
    //same circles have infinit number of intersections
    BOOST_CHECK(unit.intersect(unit2).size() == 0);
}

BOOST_AUTO_TEST_CASE(no_intersection)
{
    Circle unit = Circle::Unit(); 
    Circle circle2(2, 0, 0.3);
    BOOST_CHECK(unit.intersect(circle2).size() == 0);
}


BOOST_AUTO_TEST_CASE(simple)
{
    Circle c1 = Circle::Unit(); 
    Circle c2(0.5, 0, 1);
    
    std::vector<base::Vector2d> result = c1.intersect(c2);
    BOOST_CHECK(result.size() == 2);
    BOOST_CHECK_CLOSE(result[0].x(), 0.25, 0.001);
    BOOST_CHECK_CLOSE(result[1].x(), 0.25, 0.001);
    BOOST_CHECK_CLOSE(result[1].y(), -0.96824, 0.001);
    BOOST_CHECK_CLOSE(result[0].y(), 0.96824, 0.001);
}

BOOST_AUTO_TEST_CASE(line1)
{
    Circle c1 = Circle::Unit(); 

    Eigen::Vector2d origin(0, -2);
    Eigen::Vector2d dir(0, 1);
    Eigen::ParametrizedLine<double, 2> line(origin, dir);
    
    std::vector<base::Vector2d> result = c1.intersect(line);
    BOOST_CHECK(result.size() == 2);   
    BOOST_CHECK_CLOSE(result[0].x(), 0, 0.0001);
    BOOST_CHECK_CLOSE(result[1].x(), 0, 0.0001);
    BOOST_CHECK_CLOSE(result[1].y(), -1, 0.0001);
    BOOST_CHECK_CLOSE(result[0].y(), 1, 0.0001);
}

BOOST_AUTO_TEST_CASE(line2)
{
    Circle c1(0, 1, 2);

    Eigen::Vector2d origin(0, -2);
    Eigen::Vector2d dir(0, 1);
    Eigen::ParametrizedLine<double, 2> line(origin, dir);
    
    std::vector<base::Vector2d> result = c1.intersect(line);
    BOOST_CHECK(result.size() == 2);   
    
    BOOST_CHECK_CLOSE(result[0].x(), 0, 0.0001);
    BOOST_CHECK_CLOSE(result[1].x(), 0, 0.0001);
    BOOST_CHECK_CLOSE(result[1].y(), -1, 0.0001);
    BOOST_CHECK_CLOSE(result[0].y(), 3, 0.0001);
}

BOOST_AUTO_TEST_CASE(line3)
{
    Circle c1(4, 4, 1.23);

    Eigen::Vector2d p1(0.46, 1.4);
    Eigen::Vector2d p2(7.26, 5.29);
    
    Eigen::ParametrizedLine<double, 2> line(p1, p1 - p2);
    
    std::vector<base::Vector2d> result = c1.intersect(line);
    BOOST_CHECK(result.size() == 2);   
    
    BOOST_CHECK_CLOSE(result[0].x(), 3.27, 1);
    BOOST_CHECK_CLOSE(result[0].y(), 3.01, 1);
    BOOST_CHECK_CLOSE(result[1].x(), 5.22, 1);
    BOOST_CHECK_CLOSE(result[1].y(), 4.13, 1);
}

BOOST_AUTO_TEST_CASE(lineTangent)
{
    Circle c1(1, 0, 1);

    Eigen::Vector2d p1(0, 0);
    Eigen::Vector2d dir(0, 1);
    
    Eigen::ParametrizedLine<double, 2> line(p1, dir);
    
    std::vector<base::Vector2d>  result = c1.intersect(line);
    BOOST_CHECK(result.size() == 1);   
    
    BOOST_CHECK_CLOSE(result[0].x(), 0, 0.001);
    BOOST_CHECK_CLOSE(result[0].y(), 0, 0.001);
}

BOOST_AUTO_TEST_CASE(lineNoIntersection)
{
    Circle c1(2, 0, 1.42);

    Eigen::Vector2d p1(-2, -1);
    Eigen::Vector2d dir(0, 1.042);
    
    Eigen::ParametrizedLine<double, 2> line(p1, dir);
    
    std::vector<base::Vector2d> result = c1.intersect(line);
    BOOST_CHECK(result.size() == 0);   
}




BOOST_AUTO_TEST_SUITE_END() 
