#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai.h"

using namespace std;

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

  for (int i=0; i<10; i++)
  {
    Ai::makeTurn(st);
    cout << "Semiturn " << i << ":" << endl;
    cout << st.field << endl;
  }

  st.write(stdout);

  return 0;
}

