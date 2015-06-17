#include "Combinatorics.hpp"
#include <boost/assign/list_of.hpp>

namespace numeric {

std::map<Mode, std::string> ModeTxt = boost::assign::map_list_of
    (EXACT, "EXACT")
    (MIN, "MIN")
    (MAX, "MAX");

} // end namespace numeric
