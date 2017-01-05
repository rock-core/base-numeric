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
        result.emplace_back(px2, py2);
        return result;
    }

    //Get the perpendicular slope by multiplying by the negative reciprocal
    //Then multiply by the h offset in relation to d to get the actual offsets
    double mx = -(dy * h/d);
    double my =  (dx * h/d);

    //Add the offsets to point p2 to obtain the intersection points
    result.emplace_back(px2 + mx, py2 + my);
    result.emplace_back(px2 - mx, py2 - my);
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

