#include <boost/test/unit_test.hpp>
#include <numeric/IntegerPartitioning.hpp>

using namespace numeric;

BOOST_AUTO_TEST_SUITE(integer_partitioning)

BOOST_AUTO_TEST_CASE(integer_partitions)
{

    for(size_t i = 0; i < 16; ++i)
    {
        BOOST_TEST_MESSAGE("IntegerPartitioning: " << i );
        IntegerPartitioning ip;
        BOOST_REQUIRE_NO_THROW( ip.compute(i) );
        IntegerPartitioning::PartitionsMap partitionsMap = ip.getPartitionsMap();
        BOOST_REQUIRE_MESSAGE(partitionsMap.size() == i, "Partitions map: expected size '" << i << "' got '" << partitionsMap.size());
        BOOST_TEST_MESSAGE(ip.toString());
    }
}

BOOST_AUTO_TEST_SUITE_END()
