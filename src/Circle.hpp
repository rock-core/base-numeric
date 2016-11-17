#pragma once
#include <base/Eigen.hpp>

namespace numeric
{
    class Circle
    {
    public:
        /**Create circle with center at 0/0 and radius 1 */
        Circle();
        /**Create circle*/
        Circle(const base::Vector2d& center, double radius);
        Circle(double centerX, double centerY, double radius);
        
        /**Calculate intersections points of this and @p other.
         * @return a vector containing the intersections or an empty vector
         * if the circles do not intersect*/
        std::vector<base::Vector2d> intersect(const Circle& other);
        
        double getRadius() const;
        void setRadius(double radius);
        
        base::Vector2d getCenter() const;
        void setCenter(const base::Vector2d& c);
    private:
        base::Vector2d center;
        double r;
    };
};