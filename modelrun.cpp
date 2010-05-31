#include "modelrun.h"

modelrun::modelrun()
{
  modelinput ref_input;
  run = new model(ref_input);
}
