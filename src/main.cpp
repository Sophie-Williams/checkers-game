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
  
  cout << st.field << endl;

  const int nmoves = 60;
  for (int i=0; i<nmoves && st.field.checkState() == Field::ContinueGame; i++)
  {
    cout << "Semiturn " << i << ":" << endl;
    Ai::makeTurn(st);
    cout << st.field << endl;
  }

  st.write(stdout);

  return 0;
}

