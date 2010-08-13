#include "modelrun.h"

modelrun::modelrun(modelinput giveninput)
{
  // modelinput ref_input;
  run = new model(giveninput);
  surfacestatus = 0;
  soilstatus = 0;
  surfaceadvanced = false;
  soiladvanced    = false;
}
