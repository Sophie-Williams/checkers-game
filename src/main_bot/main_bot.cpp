#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai_ab.h"
#include "ai_chaotic.h"
#include "debug_io.h"

using namespace std;

const int maxDepth = 5;

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
 
  ABDecider ai_ab(maxDepth);
  ChaoticDecider ai_chaotic;
  vector<DeciderBase *> ais = { &ai_ab, &ai_chaotic };
  st.makeTurn(ais);
  st.write("matrix.txt");

  return 0;
}

