#include "mask64.h"
#include "field.h"
#include "state.h"
#include "ai_ab.h"
#include "debug_io.h"

using namespace std;

const int maxDepth = 6;

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
 
  ABDecider ai(maxDepth);
  st.makeTurn(ai);
  st.write("matrix.txt");

  return 0;
}

