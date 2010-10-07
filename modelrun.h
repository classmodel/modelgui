#include "model.h"
#include <QString>

class modelrun
{
public:
  modelinput previnput;
  QString runname;
  model *run;
  modelrun(modelinput*);
  bool hasrun;
  int  surfacestatus;
  int  soilstatus;
  bool surfaceadvanced;
  bool soiladvanced;
};
