#ifndef MODELCHEMTYPES
#define MODELCHEMTYPES
#define MRPCC 20
#include <string>

typedef struct{
  std::string cname;
  double coef;
  int chem_nr;
  int index;  /*  waarschijnlijk niet nodig */
}Chem;

typedef struct{
  std::string rname;
  bool activ;
  int  RadDep;
  int func1;
  double KReact;
  double Keff_cbl;
  double Keff_ft;
  int  nr_chem_inp;
  int  nr_chem_outp;
  Chem inp[4];
  Chem outp[8];
  double A;
  double B;
  double C;
  double D;
  double E;
  double F;
  double G;
}Reaction;

typedef struct{
  int formula;    //number of the formula to use
  int r_nr;      //reaction number, index to RC
  int PorL;      //0=> not active   1=>Production  2=>Loss
  double coef  ;      //coefficient in formula
  int comp1;    //index to c_cbl
  int exp1;
  int comp2;    //index to c_cbl
  int exp2;
  int comp3;    //index to c_cbl
  int exp3;
  int comp4;    //index to c_cbl
  int exp4;
}Form;

typedef struct{
  std::string name;    //name of chemical
  int active;        //active=1 else 0
  int chem_number;    //number [not doublely used)
  int nr_PL;        //total number of reactions in which this chemical is used
  Form PL[MRPCC];    //stucture holding the reaction components, reaction number etc
} Name_Number;
#endif 
