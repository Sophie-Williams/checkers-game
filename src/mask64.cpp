#include <mask64.h>

/**
 * Lookup table-based bit counter
 * table size: 64k
 *
 * Why?
 * > 200% optimization
 * (19s fast vs. 50s GCC builtin vs. 73s naive)
**/
class BitCounter
{
  public:
    BitCounter()
    {
      for (int i=0; i<65536; i++)
        lookup[i] = naive(i);    
    }
    inline int operator()(uint64_t u)
    {
      return lookup[(u)       & 0xffff]
           + lookup[(u >> 16) & 0xffff]
           + lookup[(u >> 32) & 0xffff]
           + lookup[(u >> 48) & 0xffff];
    }
  
  private:
    inline unsigned char naive(unsigned int u)
    {
#ifdef __GNUC__
      return __builtin_popcount(u);
#else
      unsigned int n = 0;
      while (u>0)
      {
        n =+ u & 1;
        u >>= 1;
      }
      return n;
#endif
    }
    unsigned char lookup[65536]; 
};

static BitCounter fastBitCounter;


int Mask64::firstbit(uint64_t v)
{
  static const uint64_t masks[] =
  {
    0xaaaaaaaaaaaaaaaa,
    0xcccccccccccccccc,
    0xf0f0f0f0f0f0f0f0,
    0xff00ff00ff00ff00,
    0xffff0000ffff0000,
    0xffffffff00000000
  };
  int res = 0;
  for (int i=5; i>=0; i--)
    if (v & masks[i])
    {
      res |= 1 << i; 
      v &= masks[i];
    }
  return res;
}

int Mask64::bitcount(uint64_t v)
{
  return fastBitCounter(v);
}
