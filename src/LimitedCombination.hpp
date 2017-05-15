#ifndef __NUMERIC_LIMITED_COMBINATION_HPP__
#define __NUMERIC_LIMITED_COMBINATION_HPP__

#include <map>
#include <vector>
#include <numeric/Combinatorics.hpp>

namespace numeric {

/**
 * \brief Compute combinatorics on a given set of limited but typed resources,
 * e.g. for available resource A:2, B:1, C:1 the following combinations are
 * possible:
 * max size 1: A,B,C
 * max size 2: AA, AB, AC, BC
 * max size 3: AAB, AAC, ABC
 *
 *
 * Use integers to compute the core combinatorics, i.e. avoiding the
 * computational effort for comparison of custom objects
 *
 \verbatim
 using namespace numeric;
 std::map<std::string, size_t> items;
 items["A"] = 2;
 items["B"] = 1;
 items["C"] = 1;

 LimitedCombination<std::string> combinations(items, LimitedCombination<std::string>::totalNumberOfAtoms(items), MAX);
 do {
    std::vector<std::string> currentCombination = combinations.current();
 } while(combinations.next());
 \endverbatim
 */
template<typename AtomType>
class LimitedCombination
{
    typedef uint32_t CoreType;
    typedef std::map<AtomType, size_t> AtomType2CountMap;
    typedef std::vector<AtomType> AtomTypeList;
    typedef std::vector<CoreType> CombinationDescriptor;

    // Model with the number of available models of this type
    AtomType2CountMap mAtomTypeAvailablilityMap;
    AtomTypeList mAtomTypeList;

    std::vector<CoreType> mLimits;
    std::vector<CoreType> mCurrentCombination;

    /// Size limit of the combination
    size_t mSize;
    numeric::Mode mMode;

    /**
     * Prepare limited combinations, i.e. setup the internal
     * item vector which will be used to determine the combination
     */
    void prepare()
    {
        CoreType currentType = 0;
        typename AtomType2CountMap::const_iterator cit = mAtomTypeAvailablilityMap.begin();
        for(; cit != mAtomTypeAvailablilityMap.end(); ++cit, ++currentType)
        {
            mAtomTypeList.push_back(cit->first);
            size_t numberPerType = cit->second;
            mLimits.at(currentType) = numberPerType;
        }

        // Making sure this behaves like the other combinatorics loops
        if(!next())
        {
            throw std::runtime_error("numeric::LimitedCombination::prepare: preparation failed "
                    "check the given parameters");
        }
    }

    /**
     * Map the internal combination to external types
     * \ŗeturn Combination of custom types
     */
    std::vector<AtomType> mapToAtomTypes(const std::vector<CoreType>& combination) const
    {
        std::vector<AtomType> atomTypeList;
        for(size_t coreType = 0; coreType < combination.size(); ++coreType)
        {
            size_t count = combination[coreType];
            for(size_t i = 0; i < count; ++i)
            {
                atomTypeList.push_back( mAtomTypeList[coreType] );
            }
        }
        return atomTypeList;
    }

    bool increment(std::vector<uint32_t>& combination, const std::vector<uint32_t>& limits) const
    {
        for(size_t pos = 0; pos < combination.size(); ++pos)
        {
            if( combination[pos] < limits[pos] )
            {
                ++combination[pos];
                if(pos == 0)
                {
                    return true;
                }

                // reset lower values after overflow
                for(int lpos = pos-1; lpos >= 0; --lpos)
                {
                    combination[lpos] = 0;
                }
                return true;
            } else if(pos == combination.size() - 1)
            {
                return false;
            }
        }
        throw std::runtime_error("numeric::LimitedCombination::increment internal error -- "
                " please check consistency of your input");
    }

public:
    /**
     * Construct limited combination generator
     * \param countMap Mapping a item (type) to the maximum possible number of occurence
     * \param size In combination with mode, define the combination size
     * \param mode Interpretation of the combination size, i.e. can be exact, min or max
     */
    LimitedCombination(const AtomType2CountMap& countMap, size_t size, numeric::Mode mode)
        : mAtomTypeAvailablilityMap(countMap)
        , mLimits(countMap.size(),0)
        , mCurrentCombination(countMap.size(),0)
        , mSize(size)
        , mMode(mode)
    {
        size_t totalCount = totalNumberOfAtoms(mAtomTypeAvailablilityMap);
        if(countMap.empty() || totalCount == 0)
        {
            throw std::invalid_argument("numeric::LimitedCombination no atoms to generate combination from -- check for empty map");
        }
        if(mSize > totalCount)
        {
            mSize = totalCount;
        }

        prepare();
    }

    /**
     * Get the totalNumberOfAtoms, i.e. the sum of occurrence
     * \return total number of atoms defined by the given map
     */
    static size_t totalNumberOfAtoms(const AtomType2CountMap& countMap)
    {
        typename AtomType2CountMap::const_iterator cit = countMap.begin();
        size_t count = 0;
        for(; cit != countMap.end(); ++cit)
        {
            count += cit->second;
        }
        return count;
    }

    /**
     * Get the current custom combination
     * \return current custom combination
     */
    std::vector<AtomType> current() const
    {
        std::vector<AtomType> atomTypeList = mapToAtomTypes(mCurrentCombination);
        std::sort(atomTypeList.begin(), atomTypeList.end());
        return atomTypeList;
    }

    size_t getCombinationSize(const std::vector<CoreType>& combination) const
    {
        size_t combinationSize = 0;
        std::vector<CoreType>::const_iterator cit = combination.begin();
        for(; cit != combination.end(); ++cit)
        {
            combinationSize += *cit;
        }
        return combinationSize;
    }

    /**
     * Check if there is a next combination and
     * forward internal iterator, so that current() allows
     * to retrieve this combination
     * \return true if there is another valid combination
     */
    bool next()
    {
        while(true)
        {
            bool hasNext = increment(mCurrentCombination, mLimits);
            if(hasNext)
            {
                // check for size constraint
                size_t combinationSize = getCombinationSize(mCurrentCombination);

                switch(mMode)
                {
                    case EXACT:
                        if(combinationSize != mSize)
                        {
                            continue;
                        }
                        break;
                    case MAX:
                        if(combinationSize > mSize)
                        {
                            continue;
                        }
                        break;
                    case MIN:
                        if(combinationSize < mSize)
                        {
                            continue;
                        }
                        break;
                }
                return true;
            }

            return false;
        }

        return false;
    }

};

} // numeric
#endif // __NUMERIC_LIMITED_COMBINATION_HPP__
