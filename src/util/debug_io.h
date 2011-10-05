#ifndef DEBUG_IO_H
#define DEBUG_IO_H

#include <iostream>
#include <utility>

#include "mask64.h"
#include "field.h"
#include "ai_base.h"

// Pair as (first, second)
template<class T1, class T2>
std::ostream& operator <<(std::ostream &s, std::pair<T1, T2> p)
{
  return s << "(" << p.first << ", " << p.second << ")";
}

// Move: (from) -> (to)
std::ostream &operator <<(std::ostream &s, Move m);

// Print a bit mask as an 8x8 matrix
std::ostream &operator<<(std::ostream &s, Mask64 m);

// Print a game field
std::ostream &operator<<(std::ostream &s, const Field &f);

#endif // DEBUG_IO_H
