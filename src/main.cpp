#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai.h"
#include "debug_io.h"

using namespace std;

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
 
#ifdef DEBUG_INTERFACE 

  cout << st.field << endl;

  const int nmoves = 60;
  for (int i=0; i<nmoves && st.field.checkState() == Field::ContinueGame; i++)
  {
    cout << "Turn " << (i/2+1) << "/" << ("AB"[i%2]) << ":" << endl;
    Ai::makeTurn(st);
    cout << st.field << endl;
  }

  st.write(stdout);

#else // DEBUG_INTERFACE

  Ai::makeTurn(st);
  st.write("matrix.txt");

#endif // DEBUG_INTERFACE

  return 0;
}

