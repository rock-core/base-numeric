#include <boost/test/unit_test.hpp>
#include <numeric/SavitzkyGolayFilter.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(savitzky_golay)

//test if Savitzky-Golay filter coefficients generator is working
BOOST_AUTO_TEST_CASE(savgol_5pt_smooth_quadratic)
{
    /**
     * From: General Least-Squares smoothing and differentiation by the Convolution (Savitzky-Golay) method.
     *  5 pt, quadratic, smooth
        -------------------------
        i   -2  -1  0   1   2

        -2  31  9   -3  -5  3
        -1  9   13  12   6  -5
        0   -3  12  17  12  -3
        1   -5  6   12  13   9
        2   3   -5  -3  9   31
    norm    35  35  35  35  35
     */
    int half_width = 2;
    int polynom_order = 2;
    std::vector<double> values;
    std::vector<double> coeff;

    //// ls-point = -2
    values.push_back(31);
    values.push_back(9);
    values.push_back(-3);
    values.push_back(-5);
    values.push_back(3);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 35));

    int ls_point = -2;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order);

    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = -1
    values.clear();
    values.push_back(9);
    values.push_back(13);
    values.push_back(12);
    values.push_back(6);
    values.push_back(-5);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 35));

    ls_point = -1;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 0
    values.clear();
    values.push_back(-3);
    values.push_back(12);
    values.push_back(17);
    values.push_back(12);
    values.push_back(-3);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 35));

    ls_point = 0;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 1
    values.clear();
    values.push_back(-5);
    values.push_back(6);
    values.push_back(12);
    values.push_back(13);
    values.push_back(9);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 35));

    ls_point = 1;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 2
    values.clear();
    values.push_back(3);
    values.push_back(-5);
    values.push_back(-3);
    values.push_back(9);
    values.push_back(31);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 35));

    ls_point = 2;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);
}

BOOST_AUTO_TEST_CASE(savgol_7pt_1stderivative_quadratic)
{
    /**
     * From: General Least-Squares smoothing and differentiation by the Convolution (Savitzky-Golay) method.
     *  7 pt, quadratic, 1st derivative
        -------------------------
        i   -3  -2  -1  0   1   2   3

        -3  -13 -29 -19 -3  1   11  7
        -2  -2  -6  -6  -2  -6  -6  -2
        -1  5   9   3   -1  -9  -15 -7
        0   8   16  8   0   -8  -16 -8
        1   7   15  9   1   -3  -9  -5
        2   2   6   6   2   6   6   2
        3   -7  -11 -1  3   19  29  13
    norm    28  84  84  28  84  84  28
     */
    std::vector<double> values;
    std::vector<double> coeff;
    int half_width = 3;
    int polynom_order = 2;
    int derivative = 1;

    //// ls-point = -3
    values.push_back(-13);
    values.push_back(-2);
    values.push_back(5);
    values.push_back(8);
    values.push_back(7);
    values.push_back(2);
    values.push_back(-7);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 28));

    int ls_point = -3;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);

    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = -2
    values.clear();
    values.push_back(-29);
    values.push_back(-6);
    values.push_back(9);
    values.push_back(16);
    values.push_back(15);
    values.push_back(6);
    values.push_back(-11);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 84));

    ls_point = -2;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = -1
    values.clear();
    values.push_back(-19);
    values.push_back(-6);
    values.push_back(3);
    values.push_back(8);
    values.push_back(9);
    values.push_back(6);
    values.push_back(-1);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 84));

    ls_point = -1;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 0
    values.clear();
    values.push_back(-3);
    values.push_back(-2);
    values.push_back(-1);
    values.push_back(0);
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 28));

    ls_point = 0;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 1
    values.clear();
    values.push_back(1);
    values.push_back(-6);
    values.push_back(-9);
    values.push_back(-8);
    values.push_back(-3);
    values.push_back(6);
    values.push_back(19);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 84));

    ls_point = 1;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 2
    values.clear();
    values.push_back(11);
    values.push_back(-6);
    values.push_back(-15);
    values.push_back(-16);
    values.push_back(-9);
    values.push_back(6);
    values.push_back(29);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 84));

    ls_point = 2;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);

    //// ls-point = 3
    values.clear();
    values.push_back(7);
    values.push_back(-2);
    values.push_back(-7);
    values.push_back(-8);
    values.push_back(-5);
    values.push_back(2);
    values.push_back(13);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 28));

    ls_point = 3;
    double step = 0.01;
    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative, step);
    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width]*step, values[i+half_width], 0.0001);
}

BOOST_AUTO_TEST_CASE(savgol_9pt_3stderivative_sextic)
{
    /**
     * From wikipedia: https://en.wikipedia.org/wiki/Savitzky%E2%80%93Golay_filter
     *  9 pt, sextic, 3st derivative
        -------------------------
        i       0

        -4      100
        -3      -457
        -2      256
        -1      459
        0       0
        1       -459
        2       -256
        3       457
        4       -100
    norm        1144
     */

    std::vector<double> values;
    std::vector<double> coeff;
    int half_width = 4;
    int polynom_order = 6;
    int derivative = 3;
    int ls_point = 0;

    //// ls-point = 0
    values.push_back(100);
    values.push_back(-457);
    values.push_back(256);
    values.push_back(459);
    values.push_back(0);
    values.push_back(-459);
    values.push_back(-256);
    values.push_back(457);
    values.push_back(-100);
    std::transform(values.begin(), values.end(), values.begin(), std::bind2nd(std::divides<double>(), 1144));

    numeric::SavitzkyGolayFilter(coeff, ls_point, half_width, polynom_order, derivative);

    for(int i = -half_width; i <= half_width; i++)
        BOOST_CHECK_CLOSE(coeff[i+half_width], values[i+half_width], 0.0001);
}

BOOST_AUTO_TEST_SUITE_END()
