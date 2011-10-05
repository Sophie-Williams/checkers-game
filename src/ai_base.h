#ifndef AI_BASE_H
#define AI_BASE_H

#include <cstdint>
#include <utility>

struct Move
{
  Move(int _x=0, int _y=0, int _nx=0, int _ny=0)
    : x(_x), y(_y), nx(_nx), ny(_ny) {}

  std::pair<int, int> from() const 
  { 
    return std::make_pair(int(x), int(y));
  }
  std::pair<int, int> to() const 
  { 
    return std::make_pair(int(nx), int(ny));
  }

  uint8_t x, y, nx, ny;
} __attribute__((packed));


#endif // AI_BASE_H

