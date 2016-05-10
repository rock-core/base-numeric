#ifndef __NUMERIC_SAVITZKY_GOLAY_FILTER_HPP__
#define __NUMERIC_SAVITZKY_GOLAY_FILTER_HPP__

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace numeric
{
/**
 * Compute coefficients of the Savitzky-Golay filter.
 *
 * Based on:
 * General Least-Squares smoothing and differentiation by the Convolution (Savitzky-Golay) method.
 * By Peter A. Gory (1989)
 */
class SavitzkyGolayFilter
{
public:

    /** Compute the coefficients of Savitzky-Golay filter
     *
     *  @param result, computed coefficients
     *  @param ls_point, Least-Square point, between [-half_width, half_width]
     *  @param half_width. Number of points used 2*half_width+1
     *  @param poly_order, polynomial order
     *  @param derivative. 0 = smooth
     *  @param step. Necessary in case of derivatives, when the coefficients need to be divided by the step.
     */
    SavitzkyGolayFilter(std::vector<double>& result, int ls_point, int half_width, int poly_order, int derivative = 0, double step = 1);
    ~SavitzkyGolayFilter();

private:

    /** Calculates the weight of the i'th data point for the t'th least-Square point of the s'th derivative, over 2m+1 points, order n
     *
     *  @param i data point, between [-m, m]
     *  @param t Least-Square point, between [-m, m]
     *  @param m half width
     *  @param n polynomial order
     *  @param s derivative
     *  @return weight
     */
    double weight(int i, int t, int m, int n, int s);

    /** Calculates the Gram Polynomial (s=0), or its s'th derivative evaluated at i, order k, over 2m+1 points
     *
     *  @param i data point
     *  @param m half width
     *  @param k order
     *  @param s derivative
     *  @return gram polynomial, or it's derivative
     */
    double gramPolynomial(int i, int m, int k, int s);

    /** Calculates the generalized factorial (a)(a-1)...(a-b+1)
     *
     *  @param a higher number
     *  @param b lower limit
     *  @return (a)(a-1)...(a-b+1)
     */
    double generalizedFactorial(int a, int b);

    /** Chack parameters of Savitzky-Golay filter
     *
     *  @param t Least-Square point
     *  @param m half width
     *  @param n polynomial order
     *  @param s derivative
     */
    void checkParameters(int t, int m, int n, int s, double step);
};
}

#endif
