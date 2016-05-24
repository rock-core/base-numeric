#ifndef __NUMERIC_HISTOGRAM_HPP__
#define __NUMERIC_HISTOGRAM_HPP__

#include <algorithm>

namespace numeric
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
	assert( count > 0 );
	assert( min_val < max_val );
    }

    const T& operator[]( size_t idx ) const
    {
	return buckets[idx];
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

    double getBucketWidth() const
    {
	return (max_val - min_val)/count; 
    }

    double getUpperBound( size_t idx ) const
    {
	return min_val + ( (idx+1) * getBucketWidth() );
    }

    double getLowerBound( size_t idx ) const
    {
	return min_val + ( (idx) * getBucketWidth() );
    }

    double getCenter( size_t idx ) const
    {
	return min_val + ( (idx+0.5) * getBucketWidth() );
    }

    size_t size() const
    {
	return buckets.size();
    }
    
};

/**
 * @brief Histogram class, that splits an interval into n regular bins, and counts how
 * often a value is added to a bin.
 *
 * The counts can be recovered using the bracket operator and a bin index.
 *
 * @code
 * Histogram h( 10, 0, 10 );
 * h.update( 1.5 );
 * h.update( 11 );
 * for( size_t i=0; i<h.size(); i++ )
 *	cout << h[i] << endl;
 * @endcode
 *
 */
struct Histogram : public Buckets<size_t>
{
    size_t n;

    /**
     * @brief Constructs a histogram where the number of update() calls with a value
     * that all into a bin are counted.
     *
     * @param count - number of divisions in the interval, needs to be greater 0
     * @param min_val - lower bound for the interval 
     * @param max_val - upper bound for the interval, needs to be greater than
     *			min_val
     */
    Histogram( int count, double min_val, double max_val )
	: Buckets<size_t>( count, min_val, max_val ), n(0)
    {
    }

    /**
     * Increase the count for the bin, which value fits in by one.  This
     * function will assign a value which is less than min_val the first bucket,
     * and greater than max_val to the last bucket.
     *
     * @param value - value for which the count in the bin should be increased
     */
    void update( double value )
    {
	++n;
	++get( value );
    }

    /** 
     * return relative count in bin so that the integral over all bins would
     * result in 1.0. Note, that this is the integral and not the sum, so it
     * takes the width of the bins into account.
     */
    double getRelative( size_t idx ) const
    {
	return operator[]( idx ) / getBucketWidth() / (double)total();
    }

    /**
     * total number of values in the bins
     */
    size_t total() const
    {
	return n;
    }
};

}

#endif
