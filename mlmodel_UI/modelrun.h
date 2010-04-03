#include "model.h"
#include <QString>

class modelrun
{
public:
  modelinput previnput;
  QString runname;
  model *run;
  modelrun();
  bool hasrun;
};
