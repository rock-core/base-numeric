#pragma once
#include <base/Eigen.hpp>
#include <Eigen/Geometry>
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
        
        /**Calculate intersection points of this and @p other.
         * @return a vector containing the intersections or an empty vector
         * if the circles do not intersect or are identical*/
        std::vector<base::Vector2d> intersect(const Circle& other) const;
        
        /** Calculate intersection points of this and @p line.
         *  @return a vector containing the intersections or an empty vector
         *          if the line does not intersect the circle.
         *  @throw std::runtime_error If the line is invalid*/
        std::vector<base::Vector2d> intersect(const Eigen::ParametrizedLine<double, 2>& line) const;
        
        
        /** Sample points on the circle 
         * @param start Angle of the first sample (in rad)
         * @param end Angle of the last sample (in rad)
         * @param num Total number of samples (including start and end)
         * @note If num == 1, @p start is returned
         * @exception std::runtime_error if num == 0
         * @exception std::runtime_error if start == end
         */
        std::vector<base::Vector2d> samplePoints(double start, double end, size_t num) const;
                        
        double getRadius() const;
        void setRadius(double radius);
        
        base::Vector2d getCenter() const;
        void setCenter(const base::Vector2d& c);

        
        base::Vector2d center;
        double r;
    };
};