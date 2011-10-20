#include <string>

#include "debug_io.h"
#include "term.h"

using namespace std;

static string contentSymbol(Field::Contents c)
{
#define BOLD(x)  (Term::SetBold  + x + Term::SetRegular)
#define GREEN(x) (Term::SetGreen + x + Term::ResetColor)
#define RED(x)   (Term::SetRed   + x + Term::ResetColor)
#define BLACK(x) (Term::SetBlack + x + Term::ResetColor)
#define BROWN(x) (Term::SetBrown + x + Term::ResetColor)
#define CYAN(x)  (Term::SetCyan  + x + Term::ResetColor)
  switch (c)
  {
    default:
    case Field::Empty:   return BOLD(BLACK("."));
    case Field::First:   return BOLD(GREEN("A"));
    case Field::Second:  return BOLD(RED("B"));
    case Field::One:     return CYAN("1");
    case Field::Two:     return BROWN("2");
    case Field::Blocked: return "*";
  }
#undef BOLD
#undef GREEN
#undef RED
#undef BLACK
#undef BROWN
#undef CYAN
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

ostream &operator <<(ostream &s, Move m)
{
  return s << m.from() << " -> " << m.to();
}


std::ostream &operator<<(std::ostream &s, Mask64 m)
{
  for (int y=0; y<8; y++)
  {
    for (int x=0; x<8; x++)
      s << ' ' << (m.at(x, y)? 'X' : '.');
    s << std::endl;
  }
  return s;
}

std::ostream &operator<<(std::ostream &s, const DeciderBase::Score &sc)
{
  return s << "<" << sc.score << " @ " << sc.depth << ">";
}
