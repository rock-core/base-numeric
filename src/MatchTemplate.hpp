
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
    void matchTemplate1D(TIter start,TIter end,TIter template_start, TIter template_end,int &pos,double &best_match,bool remove_offset=true)
    {
        best_match = std::numeric_limits<double>::max();
        pos = -1;
        for(int i=0;;++i,++start)
        {
            double match = 0;
            double offset = 0;
            if(remove_offset)
                offset = *template_start - *start;
            TIter temp_start = start;
            TIter temp_tstart = template_start;
            for(;temp_tstart != template_end && temp_start != end;++temp_tstart,++temp_start)
                match += pow((*temp_start) - (*temp_tstart) + offset,2);

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


  // joins two vectors of the same type by copying v2 to the given position of 
  // v1 and returns it as new vector
  //
  // pos is adjusting the start position of v2 insight v1 
  // a negative values means that v1 is shifted pos positions to the left
  // a positive values means that v2 starts at position pos of v1
  // if abs(pos) is too big the hole between the two vectors is filled with
  // the given default_value
  template<typename T>
      std::vector<T> joinVectors(const std::vector<T> &v1,const std::vector<T> &v2, int pos,const T &default_value)
      {
          std::vector<T> temp;
          if(pos > 0)
          {
              temp.resize(pos+v2.size());
              int n = std::min(pos,(int)v1.size());
              std::copy(v1.begin(),v1.begin()+n,temp.begin());

              //fill hole with default values
              if(n < pos)
                  std::fill(temp.begin()+n,temp.begin()+pos,default_value);
              std::copy(v2.begin(),v2.end(),temp.begin()+pos);
          }
          else
              temp = v2;

          if((int)v1.size()-pos > (int)v2.size())
          {
              int n = (int)v1.size()-pos-(int)v2.size();
              int hole = n-(int)v1.size();
              temp.resize(temp.size()+n);
              if(hole > 0)
              {
                  //fill hole with default values
                  std::fill(temp.begin()+((int)temp.size()-n),temp.begin()+((int)temp.size()-n+hole),0);
                  n -= hole;
              }
              std::copy(v1.begin()+((int)v1.size()-n),v1.end(),temp.begin()+((int)temp.size()-n));
          }
          return temp;
      }
};
#endif
