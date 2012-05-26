#ifndef BASE_STATS_HPP__
#define BASE_STATS_HPP__

#include <Eigen/Core>

namespace base
{

template <class T>
struct Square
{
    typedef T Type;
    static inline Type value( Type a ) { return a * a; }
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
    SquareType Sxx;
    T Sx;
    size_t n;

public:
    Stats();
    void update( T const& data );

    T min() const;
    T max() const;
    T mean() const;
    SquareType var() const;
    T stdev() const;
};

template <class T>
Stats<T>::Stats() 
    : Sxx( Zero< typename Square<T>::Type >::value() ), Sx( Zero<T>::value() ), n( 0 )
{
}

template <class T>
void Stats<T>::update( T const& data ) 
{
    // min max handling
    if( !n )
    {
	min_ = data;
	max_ = data;
    }
    else
    {
	min_ = base::min_el(min_, data);
	max_ = base::max_el(max_, data);
    }

    // accumulate data for variance
    Sx += data;
    Sxx += Square<T>::value( data );

    n++;
}

template <class T>
T Stats<T>::mean() const
{
    return Sx * (1.0/n);
}

template <class T>
inline typename Stats<T>::SquareType  Stats<T>::var() const
{
    return Sxx * (1.0/n) - Square<T>::value( mean() );
}

template <class T>
T Stats<T>::stdev() const
{
    return sqrt( var() );
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

}

#endif
