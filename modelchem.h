#ifndef MODELCHEM
#define MODELCHEM
#include "modelchemtypes.h"

// #define TRUE 1
// #define FALSE 0

#define FT 0
#define CBL 1

#define PRODUCTION 1
#define LOSS 2

//#define NCCBA = 4   //Number Chemical Components Before Arrow //////////////  4 is MAXIMUM //////////
//#define NCCAA = 8   //Number Chemical Components After Arrow
//#define NNSPEC = 2*(NCCBA + NCCAA) - 1

//#define MW_Air 28.97
//#define MW_H2O 18

//#define RSIZE 13
//#define CSIZE 21

#define QLOC 15

   
class modelchem
{
  public:
    modelchem(Reaction*[], Name_Number*[], const int, const int);
    //int inputchem(Reaction *RC[],Name_Number *PL_ptr[],int tnor);
    void inputchem(bool*, bool*, double);

    void calc_k( double pressure_cbl, double pressure_ft, \
                 double temp_cbl ,double temp_ft, \
                 double Q_bl, double Q_ft, \
                 double zenith);

    void iter(int cf_switch, double dt, double q, double ynew[],double ycurrent[], double*);
  
    int nr_chemicals;
    char chem_name[25][10];

    int rsize;
    int csize;
    //double c_cbl[CSIZE];
    //double c_ft[CSIZE];

  private:
    Reaction **RC_ptr;
    Name_Number **PL_ptr;
};
#endif
