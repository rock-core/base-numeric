#ifndef __NUMERIC_COMBINATORICS_HPP__
#define __NUMERIC_COMBINATORICS_HPP__

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <stdint.h>
#include <math.h>
#include <base-logging/Logging.hpp>

#include <iostream>
#include <numeric/Twiddle.hpp>
#include <boost/math/special_functions/binomial.hpp>

namespace numeric {

/**
 * \brief Create permutation on a list of given types
 * \verbatim
 #include <numeric/Combinatorics.hpp>
 ...
 std::string content = "abcd";
 std::vector<char> characterList(content.begin(), content.end());

 numeric::Permutation<char> permutation(characterList);
 do {
     std::vector<char> current = permutation.current();
     std::string s(current.begin(), current.end());
     std::cout << "Current permutation: " << s << std::endl;
 } while(permutation.next());
 \endverbatim
 */
template <class T>
class Permutation
{
public:
    typedef typename std::vector<T> ItemList;

private:
    ItemList mItems;

public:

    /**
     * Default constructor for permutation of a given list of items
     * \param items List of items to compute all permutations for
     */
    Permutation(const std::vector<T> items)
        : mItems(items)
    {
        std::sort(mItems.begin(), mItems.end());
    }

    /**
     * Retrieve the next permutation
     * \return True if there is a next permutation, False otherwise
     */
    bool next()
    {
        return std::next_permutation(mItems.begin(), mItems.end());
    }

    /**
     * Retrieve the current permutated list of items
     * \return The permutated items
     */
    const ItemList& current() const
    {
        return mItems;
    }

    /**
     * Get the total number of permutations
     * \return Number of permutations
     */
    uint64_t numberOfPermutations() const { return static_cast<uint64_t>( boost::math::factorial<double>(mItems.size()) ); }
};

enum Mode { EXACT = 0, MAX, MIN };
extern std::map<Mode, std::string> ModeTxt;

/**
 * \brief Combination of a unique item map Binomialcoefficient (n k)
 * \tparam Type of items that should be combined
 * \details A code example
 * \verbatim
   #include <vector>
   #include <string>
   #include <numeric/Combinatorics.hpp>

   using namespace numerics;

   ...

   std::vector<std::string> items;
   items.push_back("A");
   items.push_back("B");
   items.push_back("C");

   Combination<std::string> combinations(items, EXACT);
   do {
       std::vector<std::string> combination = combinations.current();
       ...
   } while(combinations.next());
 \endverbatim
 */
template <class T>
class Combination
{
public:
    typedef typename std::vector<T> ItemList;

private:
    std::vector<T> mItems;
    uint32_t mSizeOfDraw;

    typedef std::vector<bool> Draw;
    typedef std::vector< uint32_t > DrawList;

    Mode mMode;

    std::vector<T> mCurrentDraw;
    // Keep record of items per draw -- and prevent duplicates
    std::set< ItemList > mExistingDraws;

    int mCurrentDrawSize;
    Twiddle mTwiddle;

    // In order to compute the power set we have to maintain a list
    // of the draw for a corresponding combination size
    //
    // [a,a,b]
    // First draw list: a b
    // Second draw list: aa ab
    // Third draw list: aab
    DrawList mDrawList;
    DrawList::const_iterator mCurrentDrawList;

public:
    /**
     * \brief Combination of a unique item map Binomialcoefficient (n k)
     * \param uniqueItems Map of unique items
     * \param sizeOfDraw Size of the draw (k)
     * \param mode One of Mode to get the exact type of combination all up to (MAX) or from to maximum use (MIN)
     */
    Combination(const std::vector<T>& uniqueItems, size_t sizeOfDraw, Mode mode = EXACT)
        : mItems(uniqueItems)
        , mSizeOfDraw(sizeOfDraw)
        , mMode(mode)
    {
        std::sort(mItems.begin(), mItems.end());
        if(sizeOfDraw > uniqueItems.size())
        {
            throw std::invalid_argument("base::combinatorics::Combination: size of draw is greater than number of available items");
        }

        uint32_t numberOfItems = mItems.size();
        switch(mMode)
        {
            case EXACT:
            {
                createStartDraw(numberOfItems, mSizeOfDraw);
                mDrawList.push_back(mSizeOfDraw);
                break;
            }
            case MIN:
            {
                createStartDraw(numberOfItems, mSizeOfDraw);
                mDrawList.push_back(mSizeOfDraw);

                for(uint32_t i = mSizeOfDraw + 1; i <= numberOfItems; ++i)
                {
                    mDrawList.push_back(i);
                }
                break;
            }
            case MAX:
            {
                createStartDraw(numberOfItems, 1);
                mDrawList.push_back(1);

                for(uint32_t i = 2; i <= mSizeOfDraw; ++i)
                {
                    mDrawList.push_back(i);
                }
                break;
            }
            default:
                throw std::runtime_error("Invalid type given to switch");

        }
        mCurrentDrawList = mDrawList.begin();

        LOG_DEBUG_S << "Creating Combination: n = " << numberOfItems << ", k = " << sizeOfDraw << std::endl
            << "    expected number of combinations for (mode: " << ModeTxt[mode] << "): " << numberOfCombinations() << std::endl;
    }

    void createStartDraw(uint32_t n, uint32_t k)
    {
        mCurrentDraw.clear();
        mCurrentDrawSize = k;

        mTwiddle.init(k,n);

        for(size_t i = 0; i < n; ++i)
        {
            if(mTwiddle.isActivePosition(i))
            {
                mCurrentDraw.push_back( mItems[i]);
            }
        }

        mExistingDraws.clear();
        mExistingDraws.insert(mCurrentDraw);
    }

    bool next()
    {
        while(mTwiddle.next())
        {
            mCurrentDraw.clear();
            for(size_t i = 0; i != mItems.size(); ++i)
            {
                if(mTwiddle.isActivePosition(i))
                {
                    mCurrentDraw.push_back(mItems[i]);
                }
            }

            typename std::pair< typename std::set< std::vector<T> >::iterator, bool > result = mExistingDraws.insert(mCurrentDraw);
            if(result.second)
            {
                return true;
            }
            continue;
        }

        // Check if we have to increase the combination size, i.e.
        // try the next draw list
        //
        // [a,a,b]
        // First draw list: a b
        // Second draw list: aa ab
        // Third draw list: aab
        if(mCurrentDrawList + 1 != mDrawList.end())
        {
            ++mCurrentDrawList;
            createStartDraw(mItems.size(), *mCurrentDrawList);
            return true;
        }

        return false;
    }

    ItemList current() const
    {
        return mCurrentDraw;
    }

    uint64_t numberOfCombinations() const
    {
        uint32_t numberOfItems = mItems.size();
        switch(mMode)
        {
            case EXACT:
            {
                return static_cast<uint64_t>( boost::math::binomial_coefficient<double>(numberOfItems, mSizeOfDraw) );
            }
            case MIN:
            {
                uint32_t sum = 0;
                for(uint32_t i = mSizeOfDraw; i <= mItems.size(); ++i)
                {
                    sum += static_cast<uint64_t>( boost::math::binomial_coefficient<double>(numberOfItems, i) );
                }
                return sum;
            }
            case MAX:
            {
                uint32_t sum = 0;
                for(uint32_t i = 1; i <= mSizeOfDraw; ++i)
                {
                    sum += static_cast<uint64_t>( boost::math::binomial_coefficient<double>(numberOfItems, i) );
                }
                return sum;
            }
            default:
                throw std::runtime_error("Invalid type given to switch");
        } // end switch
    }
};

} // end namespace numeric
#endif // __NUMERIC_COMBINATORICS_HPP__
