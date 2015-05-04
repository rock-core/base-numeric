#ifndef __NUMERIC_STATS_HPP__
#define __NUMERIC_STATS_HPP__

#include <Eigen/Core>
#include <base/eigen.h>

namespace numeric
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

    double ddof_; //!< Delta degrees of freedem to correct variance estimates (default:0)

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

    /** Sets the delta for degrees of freedom, that is used to correct variance estimates.
     *
     *      var = 1 / (N-ddof) * sum(x_i - mean)^2
     *
     * Default value is 0.
     */
    void setDDof(double new_ddof) { ddof_ = new_ddof; }
};

template <class T>
Stats<T>::Stats()
{
    clear();
    ddof_ = 0.0;
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
    if( !n_ )
    {
        init(data);
    }
    else
    {
	min_ = numeric::min_el(min_, data);
	max_ = numeric::max_el(max_, data);
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
    return (sum_weight_ > 0.0 ) ? SquareType(M2_ / (sum_weight_ - ddof_)) : Zero<SquareType>::value();
}

template <>
inline Stats<base::VectorXd>::SquareType Stats<base::VectorXd>::var() const
{
    return ( sum_weight_ > 0.0 ) ? SquareType(M2_ / (sum_weight_ - ddof_)) :
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

/** Compute statistics for multiple time series given as a matrix (eigen matrix).
 *
 * One column is an observation and each row is a data item.
 *
 * e.g. like (time step row is for illustrative purpose only)
 *
 * time step |  0   |   1   |  2  | ...
 * ----------+------+-------+-----+--
 *  speed    | 0.1  |  0.2  | 0.2 | ..
 *  xpos     | 0.0  |  0.1  | 0.3 | ...
 */
class SeriesStats {

    Eigen::VectorXd min_;
    Eigen::VectorXd max_;
    Eigen::VectorXd mean_;
    Eigen::VectorXd stdev_;

    Eigen::MatrixXd var_;

    size_t n_;

protected:

    template<typename Derived>
    Eigen::VectorXd weightOnes(const Eigen::MatrixBase<Derived>& data) {
        return Eigen::VectorXd::Ones(data.cols());
    }

    template <typename Derived>
    void compute (const Eigen::MatrixBase<Derived>& data) {
        compute(data, weightOnes(data), 0.0);
    }

    template <typename Derived>
    void compute (const Eigen::MatrixBase<Derived>& data, double ddof) {
        compute(data, weightOnes(data), ddof);
    }

    template <typename Derived1, typename Derived2>
    void compute (const Eigen::MatrixBase<Derived1>& data,
                  const Eigen::MatrixBase<Derived2>& weights) {

        compute(data, weights, 0.0);
    }

    template <typename Derived1, typename Derived2>
    void compute (const Eigen::MatrixBase<Derived1>& data,
                  const Eigen::MatrixBase<Derived2>& weights, double ddof) {

        min_ = data.array().rowwise().minCoeff();
        max_ = data.array().rowwise().maxCoeff();
        n_ = data.cols();
        double w_sum = weights.sum();
        Eigen::MatrixXd weighted_data = data * (weights / w_sum * n_).asDiagonal();
        mean_ = weighted_data.rowwise().mean();
        Eigen::MatrixXd centered_data = weighted_data.colwise() - mean_;
        var_ = centered_data * centered_data.adjoint() / ( n_ - ddof );
        stdev_ = var_.diagonal().array().sqrt();
    }

public:

    template <typename Derived>
    SeriesStats(const Eigen::MatrixBase<Derived>& data) { compute(data); }

    template <typename Derived>
    SeriesStats(const Eigen::MatrixBase<Derived>& data, double ddof) {
        compute(data, ddof);
    }

    template <typename Derived1, typename Derived2>
    SeriesStats(const Eigen::MatrixBase<Derived1>& data,
                const Eigen::MatrixBase<Derived2>& weights) {
        compute(data, weights);
    }

    /**Constructor for the class that gives statistics on time series data.
     *
     * \param data The time series data. Each row is a data item.
     * \param weights A vector with amount of observations length weighing each column.
     *          Weights are normalized to one and are one by default.
     * \param ddof Delta degree of freedom for adjusting the variance estimate.
     *          Default is 0. ( var = 1 / (N-ddof) * sum (x_i - x_mean)^2 )
     */
    template <typename Derived1, typename Derived2>
    SeriesStats(const Eigen::MatrixBase<Derived1>& data,
                const Eigen::MatrixBase<Derived2>& weights, double ddof) {
        compute(data, weights, ddof);
    }

    const Eigen::VectorXd& min() const { return min_; }
    const Eigen::VectorXd& max() const { return max_; }
    const Eigen::VectorXd& mean() const { return mean_; }
    const Eigen::MatrixXd& var() const { return var_; }
    const Eigen::VectorXd& stdev() const { return stdev_; }
    size_t n() const { return n_; }

};

} // namespace numeric

#ifndef NUMERIC_DEPRECATE
#include <numeric/backwards/Stats.hpp>
#endif

#endif
