#pragma once
#include <base/Eigen.hpp>
#include <vector>

namespace numeric
{
    class Circle
    {
    public:
        Circle(const base::Vector2d& center, double radius);
        
        Circle(double centerX, double centerY, double radius);
        
        /**Create circle with center at 0/0 and radius 1 */
        static Circle Unit();
        
        /**Calculate intersections points of this and @p other.
         * @return a vector containing the intersections or an empty vector
         * if the circles do not intersect or are identical*/
        std::vector<base::Vector2d> intersect(const Circle& other) const;
        
        double getRadius() const;
        void setRadius(double radius);
        
        base::Vector2d getCenter() const;
        void setCenter(const base::Vector2d& c);

        
        base::Vector2d center;
        double r;
    };
};