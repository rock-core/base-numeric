#ifndef BASE_STATS_HPP__
#define BASE_STATS_HPP__

namespace base
{

/** 
 * Small helper class, which performs simple statistics
 * on a stream of values. Internally only commulative data
 * is stored, regardless on how many times you call update()
 */
template <class T>
class Stats
{
    T min_;
    T max_;
    T Sxx;
    T Sx;
    size_t n;

public:
    Stats();
    void update( T const& data );

    T min() const;
    T max() const;
    T mean() const;
    T var() const;
    T stdev() const;

protected:
    T square( T const& a ) const;
    T el_min( T const& a, T const& b ) const;
    T el_max( T const& a, T const& b ) const;
};

template <class T>
Stats<T>::Stats() 
    : Sxx(0), Sx(0), n(0)
{
}

template <class T>
inline T Stats<T>::square( T const& a ) const
{
    return a * a;
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
	min_ = el_min(min_, data);
	max_ = el_max(max_, data);
    }

    // accumulate data for variance
    Sx += data;
    Sxx += square( data );

    n++;
}

template <class T>
T Stats<T>::mean() const
{
    return Sx * (1.0/n);
}

template <class T>
T Stats<T>::var() const
{
    return Sxx * (1.0/n) - square( mean() );
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

template <class T>
T Stats<T>::el_min( T const& a, T const& b ) const
{
    return std::min(a,b);
}

template <class T>
T Stats<T>::el_max( T const& a, T const& b ) const
{
    return std::max(a,b);
}



}

#endif
