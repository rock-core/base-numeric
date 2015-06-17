#include "Twiddle.hpp"

namespace numeric {

Twiddle::Twiddle()
    : p(NULL)
    , b(NULL)
{}

Twiddle::~Twiddle()
{
    delete[] p;
    delete[] b;
}

bool Twiddle::isActivePosition(size_t position) const
{
    return b[position];
}

bool Twiddle::next()
{
  register int i, j, k;
  j = 1;
  while(p[j] <= 0)
    j++;
  if(p[j-1] == 0)
  {
    for(i = j-1; i != 1; i--)
      p[i] = -1;
    p[j] = 0;
    x = z = 0;
    p[1] = 1;
    y = j-1;
  }
  else
  {
    if(j > 1)
      p[j-1] = 0;
    do
      j++;
    while(p[j] > 0);
    k = j-1;
    i = j;
    while(p[i] == 0)
      p[i++] = -1;
    if(p[i] == -1)
    { 
      p[i] = p[k];
      z = p[k]-1;
      x = i-1;
      y = k-1;
      p[k] = -1;
    } 
    else
    {
      if(i == p[0])
        return false;
      else
      {
        p[j] = p[i];
        z = p[i]-1;
        p[i] = 0;
        x = j-1;
        y = i-1;
      }
    }
  }

  b[x] = 1;
  b[y] = 0;

  return true;
}

void Twiddle::init(int m, int n)
{
  delete[] p;
  delete[] b;

  p = new int[n+2];
  b = new int[n];

  int i;
  p[0] = n+1;
  for(i = 1; i != n-m+1; i++)
    p[i] = 0;
  while(i != n+1)
  {
    p[i] = i+m-n;
    i++;
  }
  p[n+1] = -2;
  if(m == 0)
    p[1] = 1;

  // Set irrelevant items to 0
  int pos = 0;
  for(; pos != n-m; ++pos)
  {
      b[pos] = 0;
  }
 
  // Set selection count to 1
  while(pos != n)
  {
      b[pos++] = 1;
  }
}

} // end namespace numeric
