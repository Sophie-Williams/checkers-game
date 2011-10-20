#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai_ab.h"
#include "ai_chaotic.h"
#include "debug_io.h"

using namespace std;

int main()
{
  State st;
  try
  {
    st = State::read("matrix.txt");
  }
  catch (const string &err)
  {
    cout << "Error: " << err << endl;
    return 1;
  }
  catch (...)
  {
    cout << "Unknown reading error" << endl;
  }

  cout << st.field << endl;

  const int nmoves = 60;
  ABDecider ai_ab(5);
  ChaoticDecider ai_chaotic;
  vector<DeciderBase *> ais = { &ai_ab, &ai_chaotic };
  for (int i=0; i<nmoves && st.field.checkState() == Field::ContinueGame; i++)
  {
    cout << "Turn " << (i/2+1) << "/" << ("AB"[i%2]) << ":" << endl;
    st.makeTurn(ais); 
    cout << st.field << endl;
  }

  st.write(cout);

  return 0;
}

