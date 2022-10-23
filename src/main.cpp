#include "Tetrits.h"
#include "debug_config.h"

int main()
{
  Tetrits t;
  if(t.init(0))
  t.loop();
}
