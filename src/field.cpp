#include <iostream>
#include "field.h"

using namespace std;

static char contentSymbol(Field::Contents c)
{
  switch (c)
  {
    default:
    case Field::Empty:   return '.';
    case Field::First:   return 'A';
    case Field::Second:  return 'B';
    case Field::One:     return '1';
    case Field::Two:     return '2';
    case Field::Blocked: return '*';
  }
}

std::ostream &operator<<(std::ostream &s, const Field &f)
{
  for (int y=0; y<8; y++)
  {
    for (int x=0; x<8; x++)
      s << ' ' << contentSymbol(f.at(x, y));
    s << std::endl;
  }
  return s;
}

Field::Contents Field::at(int x, int y) const
{
  if (m_first.at(x, y))   return First;
  if (m_second.at(x, y))  return Second;
  if (m_ones.at(x, y))     return One;
  if (m_twos.at(x, y))     return Two;
  if (m_blocked.at(x, y)) return Blocked;
  return Empty;
}

void Field::set(int x, int y, Contents c)
{
  m_first.set(x, y, false);
  m_second.set(x, y, false);
  m_ones.set(x, y, false);
  m_twos.set(x, y, false);
  m_blocked.set(x, y, false);
  switch (c)
  {
    case First:   m_first.set(x, y, true); break;
    case Second:  m_second.set(x, y, true); break;
    case One:     m_ones.set(x, y, true); break;
    case Two:     m_twos.set(x, y, true); break;
    case Blocked: m_blocked.set(x, y, true); break;
    default: break;
  }
}

Mask64 Field::movesFrom(int x, int y) const
{
  Mask64 ps = m_first | m_second;
  int nhor =   (ps & Mask64::row(y)).bitcount();
  int nver =   (ps & Mask64::col(x)).bitcount();
  int ndiag =  (ps & Mask64::diag(x-y)).bitcount();
  int nrdiag = (ps & Mask64::rdiag(x+y-7)).bitcount();

  /*
  cout << "nhor: " << nhor << endl;
  cout << "nver: " << nver << endl;
  cout << "ndiag: " << ndiag << endl;
  cout << "nrdiag: " << nrdiag << endl;
  cout << (Mask64::row(y) | Mask64::col(x)
         | Mask64::diag(x-y) | Mask64::rdiag(x+y-7)) << endl;
         */

  Mask64 p = Mask64::point(x, y);
  return p.translated(nhor, 0)         | p.translated(-nhor, 0)
       | p.translated(0, nver)         | p.translated(0, -nver)
       | p.translated(ndiag, ndiag)    | p.translated(-ndiag, -ndiag)
       | p.translated(-nrdiag, nrdiag) | p.translated(nrdiag, -nrdiag);
}

void Field::makeMove(int x, int y, int nx, int ny)
{
  Contents player = at(x, y);
  Contents fill = (player == First)? One : Two;

  // Take old
  set(x, y, Empty);

  // Mark adjacent cells
  Mask64 mark = Mask64::block9(nx, ny) & ~m_first & ~m_second;
  Mask64 clear = ~mark;

  m_first &= clear;
  m_second &= clear;
  m_blocked &= clear;
  m_ones &= clear;
  m_twos &= clear;
  if (fill == One)
    m_ones |= mark;
  else
    m_twos |= mark;

  // Put new
  set(nx, ny, player);
}

