#ifndef __NUMERIC_BACKWARDS_STATS_HPP__
#define __NUMERIC_BACKWARDS_STATS_HPP__

#warning "Usage of base::Square, base::Zero, base::min_el, base::max_el, base::Stats and base::SeriesStats is deprecated. \
It will be removed in future releases. \
Adapt/Rename namespace base:: to numeric:: instead. \
After adaptation you can disable the warning \
by building this package with -DNUMERIC_DEPRECATE=1"

namespace base
{
    using numeric::Square;
    using numeric::Zero;
    using numeric::min_el;
    using numeric::max_el;
    using numeric::Stats;
    using numeric::SeriesStats;
} // namespace base
#endif
