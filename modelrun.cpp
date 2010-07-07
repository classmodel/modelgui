#include "modelrun.h"

modelrun::modelrun()
{
  modelinput ref_input;
  run = new model(ref_input);
  surfacestatus = 0;
  soilstatus = 0;
  surfaceadvanced = false;
  soiladvanced    = false;
}
