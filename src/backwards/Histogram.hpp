#ifndef __NUMERIC_BACKWARDS_HISTOGRAM_HPP__
#define __NUMERIC_BACKWARDS_HISTOGRAM_HPP__

#warning "Usage of base::Buckets is deprecated.\
It will be removed in future releases. \
Adapt/Rename to numeric::Buckets instead. \
After adoptation you can disable the warning \
by building this package with -DNUMERIC_DEPRECATE=1"

#warning "Usage of base::Histogram is deprecated. \
It will be removed in future releases. \
Adapt/Rename to numeric::Histogram instead. \
After adaptation you can disable the warning \
by building this package with -DNUMERIC_DEPRECATE=1"

namespace base
{
    using numeric::Buckets;
    using numeric::Histogram;
}
#endif
