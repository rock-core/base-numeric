#include "Combinatorics.hpp"
#if __cplusplus <= 199711L
#else
#include <boost/assign/list_of.hpp>
#endif

namespace numeric {

#if __cplusplus <= 199711L
std::map<Mode, std::string> ModeTxt = {
    {EXACT, "EXACT"},
    {MIN, "MIN"},
    {MAX, "MAX"}
    };
#else
std::map<Mode, std::string> ModeTxt = ::boost::assign::map_list_of
    (EXACT, "EXACT")
    (MIN, "MIN")
    (MAX, "MAX")
    ;
#endif

} // end namespace numeric
