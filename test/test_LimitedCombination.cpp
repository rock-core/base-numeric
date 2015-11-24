#include <boost/test/unit_test.hpp>
#include <numeric/LimitedCombination.hpp>

using namespace numeric;

void test(const std::string& testName, const std::map<std::string, size_t>& resources, std::map<size_t, size_t> expectedSizes)
{
    for(size_t i = 1; i <= expectedSizes.size(); ++i)
    {
        LimitedCombination<std::string> combinations(resources,i, EXACT);
        std::vector< std::vector<std::string> > combinationList;
        do {
            std::vector<std::string> currentCombination = combinations.current();
            combinationList.push_back(currentCombination);
        } while(combinations.next());

        size_t actualSize = combinationList.size();
        size_t expectedSize = expectedSizes[i];

        BOOST_REQUIRE_MESSAGE(actualSize == expectedSize, testName << ": number of combinations of size " << i << " equal number of available resources: expected: '" << expectedSize << "' vs. '" << actualSize << "'");
    }

    for(size_t i = 1; i <= expectedSizes.size(); ++i)
    {
        LimitedCombination<std::string> combinations(resources,i, MAX);
        std::vector< std::vector<std::string> > combinationList;
        do {
            std::vector<std::string> currentCombination = combinations.current();
            combinationList.push_back(currentCombination);
        } while(combinations.next());

        size_t actualSize = combinationList.size();
        size_t expectedSize = 0;
        for(size_t s = 1; s <= i; ++s)
        {
            expectedSize += expectedSizes[s];
        }

        BOOST_REQUIRE_MESSAGE(actualSize == expectedSize, testName << ": number of combinations of size " << i << " equal number of available resources: expected: '" << expectedSize << "' vs. '" << actualSize << "'");
    }
}

BOOST_AUTO_TEST_SUITE(limited_combination)

BOOST_AUTO_TEST_CASE(compute_limited_combinations)
{
    {
        std::map<std::string, size_t> resources;
        resources["A"] = 2;
        resources["B"] = 1;
        resources["C"] = 1;

        // max size 1: A,B,C
        // max size 2: AA, AB, AC, BC
        // max size 3: AAB, AAC, ABC
        // max size 4: AABC
        std::map<size_t, size_t> expectedSizes;
        expectedSizes[1] = 3;
        expectedSizes[2] = 4;
        expectedSizes[3] = 3;
        expectedSizes[4] = 1;

        test("test A:2,B:1,C:1", resources, expectedSizes);
    }

    {
        std::map<std::string, size_t> resources;
        resources["A"] = 2;
        resources["B"] = 2;
        resources["C"] = 2;

        // max size 1: A,B,C
        // max size 2: AA, BB, CC, AB, AC, BC
        // max size 3: AAB, AAC, BBA, BBC, CCA, CCB, ABC
        // max size 4: AABB, AABC, AACC, ABBC, ABCC, BBCC
        // max size 5: AABBC, AABCC, ABBCC
        // max size 6: AABBCC
        std::map<size_t, size_t> expectedSizes;
        expectedSizes[1] = 3;
        expectedSizes[2] = 6;
        expectedSizes[3] = 7;
        expectedSizes[4] = 6;
        expectedSizes[5] = 3;
        expectedSizes[6] = 1;

        test("test A:2,B:2,C:2", resources, expectedSizes);
    }
}

BOOST_AUTO_TEST_SUITE_END()
