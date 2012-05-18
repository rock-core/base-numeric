#ifndef BASE_NUMERIC_HISTOGRAM__
#define BASE_NUMERIC_HISTOGRAM__

#include <algorithm>

namespace base
{

template <class T>
struct Buckets
{
    int count;
    double min_val, max_val;
    std::vector<T> buckets;

    Buckets( int count, double min_val, double max_val, const T& initial = T() )
	: count( count ), min_val( min_val ), max_val( max_val ), buckets( count, initial )
    {
    }

    T& operator[]( size_t idx )
    {
	return buckets[idx];
    }
    
    size_t getIndex( double value ) const
    {
	int idx = (value - min_val) / (max_val - min_val) * count;
	return std::min( count-1, std::max( 0, idx ) );
    }

    T& get( double value )
    {
	return buckets[ getIndex( value ) ];
    }

    size_t size() const
    {
	size_t sum = 0;
	for( typename std::vector<T>::const_iterator it = buckets.begin(); it != buckets.end(); it++ )
	{
	    sum += it->size();
	}
	return sum;
    }
    
};

struct Histogram : public Buckets<size_t>
{
    Histogram( int count, double min_val, double max_val )
	: Buckets( count, min_val, max_val )
    {
    }

    void update( double value )
    {
	++get( value );
    }
};

}

#endif
