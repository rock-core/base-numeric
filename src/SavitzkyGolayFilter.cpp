#include "SavitzkyGolayFilter.hpp"

namespace numeric
{
SavitzkyGolayFilter::SavitzkyGolayFilter(std::vector<double>& result, int ls_point, int half_width, int poly_order, int derivative, double step)
{
    checkParameters(ls_point, half_width, poly_order, derivative, step);
    result.clear();
    for (int i = -half_width; i <= half_width; i++)
        result.push_back( weight(i, ls_point, half_width, poly_order, derivative));
    if(derivative && step != 1)
        std::transform(result.begin(), result.end(), result.begin(), std::bind2nd(std::divides<double>(), step));
}
SavitzkyGolayFilter::~SavitzkyGolayFilter(){}

double SavitzkyGolayFilter::weight(int i, int t, int m, int n, int s)
{
    double sum = 0;
    for(int k=0; k<=n; k++)
        sum += ( (double)(2*k+1)*generalizedFactorial(2*m, k) / generalizedFactorial((2*m+k+1),(k+1)) *
                gramPolynomial(i, m, k, 0) * gramPolynomial(t, m, k, s) );
    return sum;
}

double SavitzkyGolayFilter::gramPolynomial(int i, int m, int k, int s)
{
    double gram_poly = 0;
    if (k > 0)
    {
        double gp1 = i*gramPolynomial(i, m, k-1, s) + s*gramPolynomial(i, m, k-1, s-1);
        double gp2 = gramPolynomial(i, m, (k-2), s);
        double k1 = (double)(4*k-2) / (k*(2*m-k+1));
        double k2 = (double)((k-1)*(2*m+k)) / (k*(2*m-k+1));

        gram_poly = k1*gp1 - k2*gp2;
    }
    else if (k==0 && s==0)
        gram_poly = 1;
    return gram_poly;
}

double SavitzkyGolayFilter::generalizedFactorial(int a, int b)
{
    double generalized_factorial = 1;
    if (b > 0 && a > b)
    {
        for( int i = (a-b+1) ; i <= a; i++)
            generalized_factorial *= i;
    }
    return generalized_factorial;
}

void SavitzkyGolayFilter::checkParameters(int t, int m, int n, int s, double step)
{
    if(step <= 0)
        throw std::invalid_argument("numeric::SavitzkyGolayFilter: negative or zero step.");
    if( n < 0 || m <= 0 || s < 0 )
        throw std::invalid_argument("numeric::SavitzkyGolayFilter: negative parameters.");
    if( t < -m || t > m )
        throw std::invalid_argument("numeric::SavitzkyGolayFilter: point outside width.");
    if( n > 2*m )
        throw std::invalid_argument("numeric::SavitzkyGolayFilter: polynomial order bigger than number of samples.");
}

}
