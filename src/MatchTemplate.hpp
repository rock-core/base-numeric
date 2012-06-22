
#ifndef __MATCH_TEMPLATE
#define __MATCH_TEMPLATE 

#include <vector>
#include <limits>
#include <iostream>

namespace numeric
{
    //matches the 1D template against the given sequence by sliding the template over the sequence and 
    //calculating the total square difference for each position 
    //
    //saves the best position in pos or -1 if the sequence is too small to accommodate the template
    //saves the square difference of the best position in best_match
  template<typename TIter>
    void matchTemplate1D(TIter start,TIter end,TIter template_start, TIter template_end,int &pos,double &best_match)
    {
        best_match = std::numeric_limits<double>::max();
        pos = -1;
        for(int i=0;;++i,++start)
        {
            double match = 0;
            TIter temp_start = start;
            TIter temp_tstart = template_start;
            for(;temp_tstart != template_end && temp_start != end;++temp_tstart,++temp_start)
                match += pow((*temp_start) - (*temp_tstart),2);

            //could not compare hole template
            if(temp_start == end && temp_tstart != template_end)
                break;

            if(match < best_match)
            {
                pos = i;
                best_match = match;
            }
        }
    }
};
#endif
