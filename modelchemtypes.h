/*
 * CLASS
 * Copyright (c) 2010-2013 Meteorology and Air Quality section, Wageningen University and Research centre
 * Copyright (c) 2011-2013 Jordi Vila-Guerau de Arellano
 * Copyright (c) 2011-2013 Chiel van Heerwaarden
 * Copyright (c) 2011-2013 Bart van Stratum
 * Copyright (c) 2011-2013 Kees van den Dries
 *
 * This file is part of CLASS
 *
 * CLASS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CLASS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CLASS.  If not, see <http://www.gnu.org/licenses/>.
 */

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
