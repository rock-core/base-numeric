#include "Circle.hpp"

using namespace numeric;
    
Circle::Circle(const base::Vector2d& center, double radius) : center(center), r(radius)
{}

Circle::Circle(double centerX, double centerY, double radius) : center(centerX, centerY), r(radius)
{}

Circle Circle::Unit()
{
    return Circle(0, 0, 1);
}

std::vector<base::Vector2d> Circle::intersect(const Eigen::ParametrizedLine<double, 2>& line) const
{
    //Variable names correspond to names in:
    //http://mathworld.wolfram.com/Circle-LineIntersection.html
    
    std::vector<base::Vector2d> result;
    
    Eigen::Vector2d p1 = line.pointAt(0);
    Eigen::Vector2d p2 = line.pointAt(1);
    
    //move points to coordinate system of circle
    p1 -= center;
    p2 -= center;
    
    const double dx = p2.x() - p1.x();
    const double dy = p2.y() - p1.y();
    const double dr2 = dx * dx + dy * dy;
    const double D = p1.x() * p2.y() - p2.x() * p1.y();
    const double delta = r * r * dr2 - D * D;
    
    if(dr2 == 0)
        throw std::runtime_error("Cannot intersect line and circle. Line is invalid.");
    
    if(delta < 0) // no intersection
        return result;
    
    if(delta == 0) //one ontersection
    {
        base::Vector2d tangentPoint(D * dy / dr2, -D * dx / dr2);
        tangentPoint += center; //move back to original coordinate system
        result.push_back(tangentPoint);
        return result;
    }
    
    //two intersections
    const double sgn = dy < 0? -1 : 1;
    const double sqrtDelta = std::sqrt(delta);
    
    base::Vector2d intersection1;
    intersection1.x() = (D * dy + sgn * dx * sqrtDelta) / dr2;
    intersection1.y() = (-D * dx + std::abs(dy) * sqrtDelta) / dr2;
    
    base::Vector2d intersection2;
    intersection2.x() = (D * dy - sgn * dx * sqrtDelta) / dr2;
    intersection2.y() = (-D * dx - std::abs(dy) * sqrtDelta) / dr2;
    
    //move back to original coordinate system
    intersection1 += center;
    intersection2 += center;
    
    result.push_back(intersection1);
    result.push_back(intersection2);
    return result;
}
 
std::vector<base::Vector2d> Circle::intersect(const numeric::Circle& other) const
{
    //see: http://paulbourke.net/geometry/circlesphere/
    //and  http://paulbourke.net/geometry/circlesphere/tangentpointtocircle.zip
    std::vector<base::Vector2d> result;
    const double r1 = other.r;
    const double px0 = center.x();
    const double py0 = center.y();
    const double px1 = other.center.x();
    const double py1 = other.center.y();
       
    const double dx = px1-px0;
    const double dy = py1-py0;
    const double d = sqrt(dx*dx + dy*dy); //Distance between centers

    /*Circles share centers. This results in division by zero,
      infinite solutions or one circle being contained within the other. */
    if(d == 0.0)
        return result;
    //Circles do not touch each other
    else if(d > (r + r1))
        return result;
    //One circle is contained within the other
    else if(d < fabs(r - r1))
        return result; 

    /*
    //Considering the two right triangles p0p2p3 and p1p2p3 we can write:
    //a^2 + h^2 = r^2 and b^2 + h^2 = r1^2

    //BEGIN PROOF

    //Remove h^2 from the equation by setting them equal to themselves:
    r^2 - a^2 = r1^2 - b^2
    //Substitute b with (d - a) since it is proven that d = a + b:
    r^2 - a^2 = r1^2 - (d - a)^2
    //Complete the square:
    r^2 - a^2 = r1^2 - (d^2 -2da + a^2)
    //Subtract r1^2 from both sides:
    r^2 - r1^2 - a^2 = -(d^2 -2da + a^2)
    //Invert the signs:
    r^2 - r1^2 - a^2 = -d^2 + 2da - a^2
    //Adding a^2 to each side cancels them out:
    r^2 - r1^2 = -d^2 + 2da
    //Add d^2 to both sides to shift it to the other side:
    r^2 - r1^2 + d^2 = 2da
    //Divide by 2d to finally solve for a:
    a = (r^2 - r1^2 + d^2)/ (2d)
    //END PROOF
    */

    double a = ((r*r) - (r1*r1) + (d*d)) / (2.0 * d);
    //Solve for h by substituting a into a^2 + h^2 = r^2
    double h = sqrt((r*r) - (a*a));

    //Find point p2 by adding the a offset in relation to line d to point p0
    double px2 = px0 + (dx * a/d);
    double py2 = py0 + (dy * a/d);

    //Tangent circles have only one intersection
    if(d == (r + r1))
    {
        result.push_back(base::Vector2d(px2, py2));
        return result;
    }

    //Get the perpendicular slope by multiplying by the negative reciprocal
    //Then multiply by the h offset in relation to d to get the actual offsets
    double mx = -(dy * h/d);
    double my =  (dx * h/d);

    //Add the offsets to point p2 to obtain the intersection points
    result.push_back(base::Vector2d(px2 + mx, py2 + my));
    result.push_back(base::Vector2d(px2 - mx, py2 - my));
    return result;
}

base::Vector2d Circle::getCenter() const
{
    return center;
}

double Circle::getRadius() const
{
    return r;
}

void Circle::setCenter(const base::Vector2d& c)
{
    center = c;
}

void Circle::setRadius(double radius)
{
    r = radius;
}

