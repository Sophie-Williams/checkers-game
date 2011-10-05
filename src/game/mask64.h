#ifndef MASK64_H
#define MASK64_H

#include <cstdint>
#include <iostream>
#include <utility>

class Mask64
{
  public:
    class BitIterator
    {
      public:
        std::pair<int, int> operator*() const 
        { 
          return std::make_pair(7 - (bit%8), 7 - (bit/8));
        }
        bool operator!=(BitIterator other) { return d != other.d; }
        BitIterator &operator++()
        {
          d &= ~(uint64_t(1) << bit);
          bit = firstbit(d);
          return *this;
        }
      private:
        friend class Mask64;
        BitIterator(uint64_t _d) : d(_d), bit(firstbit(d)) {}
        uint64_t d;
        int bit;
    };

    Mask64(uint64_t _d = 0) : d(_d) {}

    // Iterators
    BitIterator begin() const { return d; }
    BitIterator end() const { return 0; }

    // Bitwise logical ops
    Mask64 operator ~() const { return ~d; }
    Mask64 operator &(Mask64 other) const { return d&other.d; }
    Mask64 operator |(Mask64 other) const { return d|other.d; }

    Mask64 operator &=(Mask64 other) { d &= other.d; return *this; }
    Mask64 operator |=(Mask64 other) { d |= other.d; return *this; }

    Mask64 translated(int dx, int dy) const
    {
      uint64_t masked = ((*this) & ~cols(-dx) & ~rows(-dy)).d;
      if (dx<0) masked <<= -dx;
      else masked >>= dx;
      if (dy<0) masked <<= -8*dy;
      else masked >>= 8*dy;
      return masked;
    }

    bool isNull() const { return d == uint64_t(0); }

    // Bit counting
    int bitcount() const { return bitcount(d); }
    int firstbit() const { return firstbit(d); }

    // Indexing
    bool at(int x, int y) const
    {
      return ((d >> offset(x, y)) & 1) == 1;
    }
    void set(int x, int y, bool v)
    {
      uint64_t mask = ~(uint64_t(1) << offset(x, y));
      d = (d & mask) | (uint64_t(v?1:0) << offset(x, y));
    }


    // Standard mask generators
    // Single point
    static Mask64 point(int x, int y)
    {
      return uint64_t(1) << offset(x, y);
    }
    // Single column
    static Mask64 col(int n)
    {
      return m_rows << (7-n);
    }
    // Single row
    static Mask64 row(int n)
    {
      return uint64_t(0xff) << (8*(7-n));
    }
    
    // Column span: n>0 => 0..n-1 | n<0 => n..7
    static Mask64 cols(int n) 
    { 
      uint8_t row = n==0? 0 :
        n>0? 0xff << (8-n) : 0xff >> (8+n);
      return m_rows * row;
    }

    // Row span: n>0 => 0..n-1 | n<0 => n..7
    static Mask64 rows(int n) 
    { 
      return n==0? 0 : 
        n>0? m_full << (64-8*n) 
           : m_full >> (64+8*n);
    }

    // "\" diagonal
    static Mask64 diag(int off)
    {
      uint64_t masked = m_diag & ~rows(off).d;
      return off>0? masked << 8*off
                  : masked >> -8*off;
    }

    // "/" diagonal
    static Mask64 rdiag(int off)
    {
      uint64_t masked = m_rdiag & ~rows(-off).d;
      return off>0? masked >> 8*off
                  : masked << -8*off;
    }

    // 3x3 block centered at (x, y)
    static Mask64 block9(int x, int y)
    {
      return Mask64(m_3x3).translated(x-1, y-1);
    }

  private:
    friend class BitIterator;

    static const uint64_t m_rows  = 0x0101010101010101;
    static const uint64_t m_full  = 0xffffffffffffffff;
    static const uint64_t m_rdiag = 0x0102040810204080;
    static const uint64_t m_diag  = 0x8040201008040201;
    static const uint64_t m_3x3   = 0xe0e0e00000000000;

    static int offset(int x, int y) { return 8*(7-y) + 7-x; }
    static int firstbit(uint64_t v);
    static int bitcount(uint64_t v);

    uint64_t d;
};

#endif // MASK64_H
