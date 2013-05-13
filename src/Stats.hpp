#ifndef BASE_STATS_HPP__
#define BASE_STATS_HPP__

#include <Eigen/Core>
#include <base/eigen.h>

namespace base
{

template <class T>
struct Square
{
    typedef T Type;
    static inline Type value( Type a ) { return a * a; }
    static inline Type value( Type a, Type b ) { return a * b; }
};

template <class T>
struct Zero
{
    static inline T value() { return 0; }
};


template <class _Scalar, int _Rows, int _Options>
struct Square< Eigen::Matrix<_Scalar, _Rows, 1, _Options> >
{
    typedef typename Eigen::Matrix<_Scalar, _Rows, 1, _Options> T;
    typedef typename Eigen::Matrix<_Scalar, _Rows, _Rows, _Options> Type;
    static inline Type value( const T& a ) { return a * a.transpose(); }
    static inline Type value( const T& a, const T& b ) { return a * b.transpose(); }
};

template <class _Scalar, int _Rows, int _Cols, int _Options>
struct Zero< Eigen::Matrix<_Scalar, _Rows, _Cols, _Options> >
{
    typedef Eigen::Matrix<_Scalar, _Rows, _Cols, _Options> T;
    static inline T value() { return T::Zero(); }
};

template <class T> T min_el( T a, T b ) { return std::min( a, b ); }
template <class T> T max_el( T a, T b ) { return std::max( a, b ); }

template <class _Scalar, int _Rows, int _Options>
Eigen::Matrix<_Scalar, _Rows, 1, _Options> min_el( 
	Eigen::Matrix<_Scalar, _Rows, 1, _Options> const& a, 
	Eigen::Matrix<_Scalar, _Rows, 1, _Options> const& b )
{
    return a.array().min( b.array() ).matrix();
}

template <class _Scalar, int _Rows, int _Options>
Eigen::Matrix<_Scalar, _Rows, 1, _Options> max_el( 
	Eigen::Matrix<_Scalar, _Rows, 1, _Options> const& a, 
	Eigen::Matrix<_Scalar, _Rows, 1, _Options> const& b )
{
    return a.array().max( b.array() ).matrix();
}

/** 
 * Small helper class, which performs simple statistics
 * on a stream of values. Internally only commulative data
 * is stored, regardless on how many times you call update()
 */
template <class T>
class Stats
{
    typedef typename Square<T>::Type SquareType;

    T min_;
    T max_;

    SquareType M2_;
    T mean_;
    double sum_weight_;
    size_t n_;

    void init( T const& data);

public:
    Stats();
    void update( T const& data, double weight = 1.0 );
    void clear();

    T min() const;
    T max() const;
    T mean() const;
    SquareType var() const;
    T stdev() const;
    double sumWeights() const;
    size_t n() const;
};

template <class T>
Stats<T>::Stats() 
{
    clear();
}

template <class T>
void Stats<T>::clear() {
    n_ = 0;
}

template <class T>
void Stats<T>::init( T const& data ) {

        sum_weight_ = 0.0;
        min_ = data;
        max_ = data;
        M2_ = Zero<SquareType>::value();
        mean_ = Zero<T>::value();
}

template <>
inline void Stats<base::VectorXd>::init( base::VectorXd const& data ) {

    int rows = data.rows();

    sum_weight_ = 0.0;
    min_ = data;
    max_ = data;
    M2_ = base::MatrixXd::Zero(rows,rows);
    mean_ = base::VectorXd::Zero(rows);
}

template <class T>
void Stats<T>::update( T const& data, double weight ) 
{
    // min max handling
    if( !n_ )
    {
        init(data);
    }
    else
    {
	min_ = base::min_el(min_, data);
	max_ = base::max_el(max_, data);
    }

    //
    // algorithm is based on 
    // D. H. D. West (1979). Communications of the ACM, 22, 9, 532-535:
    // Updating Mean and Variance Estimates: An Improved Method
    //
    // http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
    //
    double temp = weight + sum_weight_;
    T delta = data - mean_;
    T R = delta * weight / temp;
    mean_ = mean_ + R;
    M2_ = M2_ + sum_weight_ * Square<T>::value( delta, R );
    sum_weight_ = temp;

    n_++;
}

template <class T>
double Stats<T>::sumWeights() const
{
    return sum_weight_;
}

template <class T>
T Stats<T>::mean() const
{
    return mean_;
}

template <class T>
inline typename Stats<T>::SquareType  Stats<T>::var() const
{
    return (sum_weight_ > 0.0 ) ? SquareType(M2_ / sum_weight_) : Zero<SquareType>::value();
}

template <>
inline Stats<VectorXd>::SquareType Stats<VectorXd>::var() const
{
    return ( sum_weight_ > 0.0 ) ? SquareType(M2_ / sum_weight_) :
        base::MatrixXd::Zero(M2_.rows(),M2_.cols());
}

template <class T>
T Stats<T>::stdev() const
{
    return sqrt( var() );
}

template <>
inline base::VectorXd Stats<base::VectorXd>::stdev() const
{
    return var().diagonal().array().sqrt();
}

template <class T>
T Stats<T>::min() const
{
    return min_;
}

template <class T>
T Stats<T>::max() const
{
    return max_;
}

template <class T>
size_t Stats<T>::n() const
{
    return n_;
}

}

#endif
