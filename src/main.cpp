#include "mask64.h"
#include "field.h"
#include "state.h"

using namespace std;

struct Move
{
  uint8_t x, y, nx, ny;
};

template<class T1, class T2>
ostream& operator<<(ostream &s, pair<T1, T2> p)
{
  return s << "(" << p.first << ", " << p.second << ")";
}

int main()
{
  State st;
  try
  {
    st = State::read("matrix.txt");
  }
  catch (const char *err)
  {
    cout << "Error: " << err << endl;
    return 1;
  }
  
  cout << st.field << endl;

  for (auto pos: st.field.first())
  {
    cout << "/==-------------------------------->" << endl;
    cout << "Player at " << pos << endl;
    Mask64 moves = st.field.movesFrom(pos.first, pos.second);
    cout << moves << endl;
    for (auto newPos: moves)
    {
      Field f1 = st.field;
      f1.makeMove(pos.first, pos.second, newPos.first, newPos.second);
      cout << f1 << endl;
    }
  }

  st.field.makeMove(4, 3, 4, 1);
  st.write(stdout);

  return 0;
}

