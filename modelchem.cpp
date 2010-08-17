#include <string>
#include <fstream>
#include <cmath>
#include <cstring>
#include "modelchem.h"

modelchem::modelchem(Reaction *RC_ptrin[], Name_Number *PL_ptrin[], int rsizein, int csizein)
{
  int i;

  csize  = csizein;
  rsize  = rsizein;
  RC_ptr = new Reaction*[rsize];
  PL_ptr = new Name_Number*[csize];

  printf("rsize: %i, csize: %i\n", rsize, csize);
  for(i=0; i < rsize; i++)
    RC_ptr[i] = RC_ptrin[i];

  for(i=0; i<csize; i++)
    PL_ptr[i] = PL_ptrin[i];
}

//int modelchem::inputchem(Reaction *RC_ptr[],Name_Number *PL_ptr[],int tnor)
void modelchem::inputchem(int tnor)
{
  int i,j,k,L;
  int found;
  std::string name;
  //char dname1[6],dname2[6];
  int react_nr;
  int icoef[3];
  int nchasp;
  FILE *rcout;
  Name_Number *PL_temp;
    
  
  // CvH, is this var only local?
  int nr_chemicals;

  PL_temp = new Name_Number[csize];

  k = 0;
  for(i=0;i<rsize;i++)
  {
    printf("CvH check: %i, %p\n", i, RC_ptr[i]);
    for( j=0;j<RC_ptr[i]->nr_chem_inp;j++)
    {               // look only on input side of reaction
      name = RC_ptr[i]->inp[j].cname;
      found = 0;
      for(L=0;L<=k;L++){
        if(name == PL_ptr[L]->name) {
          found = 1;
          PL_ptr[L]->nr_PL = PL_ptr[L]->nr_PL +1;      //count number of reactions
          if( PL_ptr[L]->nr_PL > MRPCC ) {
            printf("MRPCC to low, increase MRPCC in modchem");
             //STOP
           }
           PL_ptr[L]->PL[PL_ptr[L]->nr_PL-1].r_nr = i;       //store reaction number index to RC
           PL_ptr[L]->PL[PL_ptr[L]->nr_PL-1].PorL = LOSS;    //this is a loss reaction for this component
           break;
        } //end if
      } //end for L
      if (found == 0) {
        PL_ptr[k]->name = name;
        PL_ptr[k]->chem_number = RC_ptr[i]->inp[j].chem_nr;
        PL_ptr[k]->nr_PL = PL_ptr[k]->nr_PL +1;
        if ( PL_ptr[k]->nr_PL > MRPCC ) {
          printf("MRPCC to low, increase MRPCC in modchem");
          //STOP
        }
        PL_ptr[k]->PL[PL_ptr[k]->nr_PL-1].r_nr = i;        //store reaction number
        PL_ptr[k]->PL[PL_ptr[k]->nr_PL-1].PorL = LOSS;      //this is a loss reaction for this component
        k=k+1;
    }
   } // end for j
 } //end for i


  for(i=0;i<rsize;i++){
    for(j=0;j<RC_ptr[i]->nr_chem_outp;j++){             // look only on output side of reaction
      name = RC_ptr[i]->outp[j].cname;
      found = 0;
      for(L=0;L<k;L++){
        if(name == PL_ptr[L]->name) {
          found = 1;
          PL_ptr[L]->nr_PL = PL_ptr[L]->nr_PL +1;        //count number of reactions
          if( PL_ptr[L]->nr_PL > MRPCC ) {
            printf("MRPCC to low, increase MRPCC in modchem");
            //STOP
          }
          PL_ptr[L]->PL[PL_ptr[L]->nr_PL-1].r_nr = i;       //store reaction number index to RC
          PL_ptr[L]->PL[PL_ptr[L]->nr_PL-1].PorL = PRODUCTION;    //this is a produktion reaction for this component
          break;
        } //end if
      } //end for L
      if(found == 0) {
        PL_ptr[k]->name = name;
        PL_ptr[k]->chem_number = RC_ptr[i]->outp[j].chem_nr;
        PL_ptr[k]->nr_PL = PL_ptr[k]->nr_PL +1;
        if ( PL_ptr[L]->nr_PL > MRPCC ) {
          printf("MRPCC to low, increase MRPCC in modchem");
          //STOP
        }
        PL_ptr[k]->PL[PL_ptr[k]->nr_PL-1].r_nr = i;          //store reaction number
        PL_ptr[k]->PL[PL_ptr[k]->nr_PL-1].PorL = PRODUCTION;    //this is a produktion reaction for this component
        k=k+1;
      }
    } // end for j
  } //end for i


  nr_chemicals = k+1; // CvH k has 0 basis, thus nr of chemicals == k + 1
  printf("number of species: %i, number declared %i\n",  nr_chemicals, csize);

  for(i=0;i<nr_chemicals;i++){                        //this is for PRODUCTION
    for(j=0;j<PL_ptr[i]->nr_PL;j++){
      react_nr = PL_ptr[i]->PL[j].r_nr;
      if( PL_ptr[i]->PL[j].PorL == PRODUCTION){
        for(k=0;k<RC_ptr[react_nr]->nr_chem_outp;k++){
//debug only
  //strcpy(dname1,RC_ptr[react_nr]->outp[k].cname);
    //strcpy(dname2,PL_ptr[i]->name);

          if( RC_ptr[react_nr]->outp[k].cname == PL_ptr[i]->name)
          {
            switch(RC_ptr[react_nr]->nr_chem_inp){            //left of arrow 1 reactant
              case 1:
                icoef[0] = (int)(RC_ptr[react_nr]->inp[0].coef +0.05);
                switch (icoef[0]){
                  case 1:
                     PL_ptr[i]->PL[j].formula = 1;
                     PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                     break;
                  case 2:
                     PL_ptr[i]->PL[j].formula = 2;
                     PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                     PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[0].index;
                     break;
                  default:
                     PL_ptr[i]->PL[j].formula = 3;
                     PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                     PL_ptr[i]->PL[j].exp1  = icoef[0];
                     break;
                }
                PL_ptr[i]->PL[j].coef =  RC_ptr[react_nr]->outp[k].coef;
                break;
              case 2:  //there are 2 reacting species
                icoef[0] = (int)(RC_ptr[react_nr]->inp[0].coef +0.05);
                icoef[1] = (int)(RC_ptr[react_nr]->inp[1].coef +0.05);
                if ((icoef[0] == 1) && (icoef[1] == 1) ){
                  PL_ptr[i]->PL[j].formula = 2;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                }else{
                  PL_ptr[i]->PL[j].formula = 4;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].exp1  = icoef[0];
                  PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                  PL_ptr[i]->PL[j].exp2  = icoef[1];
                }
                PL_ptr[i]->PL[j].coef =  RC_ptr[react_nr]->outp[k].coef;
                break;
              case 3: // there are 3 reacting species
                icoef[0] = (int)(RC_ptr[react_nr]->inp[0].coef +0.05);
                icoef[1] = (int)(RC_ptr[react_nr]->inp[1].coef +0.05);
                icoef[2] = (int)(RC_ptr[react_nr]->inp[2].coef +0.05);
                if( (icoef[0]==1) && (icoef[1] == 1) && (icoef[2] == 1)){
                  // we don't need exponents keep it simple
                  PL_ptr[i]->PL[j].formula = 5;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[0].index;
                }else{
                  PL_ptr[i]->PL[j].formula = 6;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].exp1  = icoef[0];
                  PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                  PL_ptr[i]->PL[j].exp2  = icoef[1];
                  PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[2].index;
                  PL_ptr[i]->PL[j].exp3  = icoef[2];
                }
                PL_ptr[i]->PL[j].coef =  RC_ptr[react_nr]->outp[k].coef;
                break;
              case 4:
                icoef[0] = (int)(RC_ptr[react_nr]->inp[0].coef +0.05);
                icoef[1] = (int)(RC_ptr[react_nr]->inp[1].coef +0.05);
                icoef[2] = (int)(RC_ptr[react_nr]->inp[2].coef +0.05);
                icoef[3] = (int)(RC_ptr[react_nr]->inp[3].coef +0.05);
                PL_ptr[i]->PL[j].formula = 7;
                PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                PL_ptr[i]->PL[j].exp1  = icoef[0];
                PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                PL_ptr[i]->PL[j].exp2  = icoef[1];
                PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[2].index;
                PL_ptr[i]->PL[j].exp3  = icoef[2];
                PL_ptr[i]->PL[j].comp4 = RC_ptr[react_nr]->inp[3].index;
                PL_ptr[i]->PL[j].exp4  = icoef[3];
                PL_ptr[i]->PL[j].coef =  RC_ptr[react_nr]->outp[k].coef;
                break;
              default:
                printf("Error 1");
                break;
            }
          }
        } // k=1,RC_ptr[react_nr]->nr_chem_outp
      } //( PL_ptr[i]->PL[j].PorL == 1)
    } //j=1,PL_ptr[i]->nr_PL
  } //1,nr_chemicals



  //do all reactions on the loss side
  for(i=0;i<nr_chemicals;i++){
    for(j=0;j<PL_ptr[i]->nr_PL;j++){
      react_nr = PL_ptr[i]->PL[j].r_nr;
      if( PL_ptr[i]->PL[j].PorL == LOSS){    // This is LOSS
        for(k=0;k<RC_ptr[react_nr]->nr_chem_inp;k++){
          icoef[0] = (int)(RC_ptr[react_nr]->inp[0].coef +0.05);
          icoef[1] = (int)(RC_ptr[react_nr]->inp[1].coef +0.05);
          icoef[2] = (int)(RC_ptr[react_nr]->inp[2].coef +0.05);
          icoef[3] = (int)(RC_ptr[react_nr]->inp[3].coef +0.05);
          switch(RC_ptr[react_nr]->nr_chem_inp){
            case 1: //the loss comp is the only reactant
              switch (icoef[0]){
                case 1:
                  PL_ptr[i]->PL[j].formula = 0;
                  break;
                case 2:
                  PL_ptr[i]->PL[j].formula = 1;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  break;
                case 3:
                  PL_ptr[i]->PL[j].formula = 3;
                  PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                  PL_ptr[i]->PL[j].exp1  = icoef[0] - 1;
                  break;
              }
              PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[0].coef;
              break;
            case 2: // we have 2 components in which one is current species
         //debug only
        //strcpy(dname1,RC_ptr[react_nr]->inp[k].cname);
          //strcpy(dname2,PL_ptr[i]->name);
               if( RC_ptr[react_nr]->inp[k].cname == PL_ptr[i]->name){
                switch (k){
                  case 0:
                    if( (icoef[0] ==1) && (icoef[1] == 1) ){
                      PL_ptr[i]->PL[j].formula = 1;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[1].index;
                    }else if((icoef[0] ==1) && (icoef[1] > 1) ){
                      PL_ptr[i]->PL[j].formula = 3;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[1].index;
                      PL_ptr[i]->PL[j].exp1  = icoef[1];
                    }else if((icoef[0] ==2) && (icoef[1] == 1) ){
                      PL_ptr[i]->PL[j].formula = 2;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                    }else{
                      PL_ptr[i]->PL[j].formula = 4;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].exp1  = icoef[0] - 1;
                      PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                      PL_ptr[i]->PL[j].exp2  = icoef[1];
                    }
                    PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[0].coef;
                    break;
                  case 1:
                    if( (icoef[0] ==1) && (icoef[1] == 1) ){
                      PL_ptr[i]->PL[j].formula = 1;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                    }else if((icoef[1] ==1) && (icoef[0] > 1)){
                      PL_ptr[i]->PL[j].formula = 3;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].exp1  = icoef[0];
                    }else if((icoef[1] ==2) && (icoef[0] == 1) ) {
                      PL_ptr[i]->PL[j].formula = 2;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                    }else{
                      PL_ptr[i]->PL[j].formula = 4;
                      PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].exp1  = icoef[0];
                      PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                      PL_ptr[i]->PL[j].exp2  = icoef[1] - 1;
                    }
                    PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[1].coef;
                    break;
                } //end switch k
              } //if( RC_ptr[react_nr]->inp[k].name == PL_ptr[i]->name)
              break;
            case 3: //we have 3 components on input
              if( RC_ptr[react_nr]->inp[k].cname == PL_ptr[i]->name){
                switch (k){
                  case 1:
                    PL_ptr[i]->PL[j].formula = 6;
                    PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[1].index;
                    PL_ptr[i]->PL[j].exp1  = icoef[1];
                    PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[2].index;
                    PL_ptr[i]->PL[j].exp2  = icoef[2];
                    PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[0].coef;
                    if ( icoef[0] == 1 ) {
                      PL_ptr[i]->PL[j].formula = 4;
                    }else{
                      PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[0].index;
                      PL_ptr[i]->PL[j].exp3  = icoef[0] - 1;
                    }
                    break;
                  case 2:
                    PL_ptr[i]->PL[j].formula = 6;
                    PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                    PL_ptr[i]->PL[j].exp1  = icoef[0];
                    PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[2].index;
                    PL_ptr[i]->PL[j].exp2  = icoef[2];
                    PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[1].coef;
                    if ( icoef[1] == 1 ){
                      PL_ptr[i]->PL[j].formula = 4;
                    }else{
                      PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[1].index;
                      PL_ptr[i]->PL[j].exp3  = icoef[1] -1;
                    }
                    break;
                  case 3:
                    PL_ptr[i]->PL[j].formula = 6;
                    PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                    PL_ptr[i]->PL[j].exp1  = icoef[0];
                    PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                    PL_ptr[i]->PL[j].exp2  = icoef[1];
                    PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[2].coef;
                    if ( icoef[2] == 1 ){
                      PL_ptr[i]->PL[j].formula = 4;
                    }else{
                      PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[2].index;
                      PL_ptr[i]->PL[j].exp3  = icoef[2] -1;
                    }
                    break;
                  }// end switch k
                } //end if
              break;
            case 4:  //we have 4 components on input
              if( RC_ptr[react_nr]->inp[k].cname == PL_ptr[i]->name)
              {
                 PL_ptr[i]->PL[j].formula = 7;
                 PL_ptr[i]->PL[j].comp1 = RC_ptr[react_nr]->inp[0].index;
                 PL_ptr[i]->PL[j].exp1  = icoef[0];
                 PL_ptr[i]->PL[j].comp2 = RC_ptr[react_nr]->inp[1].index;
                 PL_ptr[i]->PL[j].exp2  = icoef[1];
                 PL_ptr[i]->PL[j].comp3 = RC_ptr[react_nr]->inp[2].index;
                 PL_ptr[i]->PL[j].exp3  = icoef[2];
                 PL_ptr[i]->PL[j].comp4 = RC_ptr[react_nr]->inp[3].index;
                 PL_ptr[i]->PL[j].exp4  = icoef[3];
                 switch(k){
                   case 1:
                     PL_ptr[i]->PL[j].exp1  = icoef[0] - 1;
                     PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[0].coef;
                     break;
                   case 2:
                     PL_ptr[i]->PL[j].exp2  = icoef[1] - 1;
                     PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[1].coef;
                     break;
                   case 3:
                     PL_ptr[i]->PL[j].exp3  = icoef[2] - 1;
                     PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[2].coef;
                     break;
                   case 4:
                     PL_ptr[i]->PL[j].exp4  = icoef[3] - 1;
                     PL_ptr[i]->PL[j].coef = RC_ptr[react_nr]->inp[3].coef;
                     break;
                 }//end switch k
               }//end if
              break;
          }//end switch
        } //for k
      }// end if
    }
  }


  for (i=0;i<csize;i++){
    PL_temp[i].nr_PL = 0;
    PL_temp[i].name = "\0";
  }

// sort PL_scheme on chem_number;
  for (i=0;i<csize;i++){
    if(PL_ptr[i]->chem_number != -99){
      PL_temp[PL_ptr[i]->chem_number] = *PL_ptr[i];
    }
  }


  for(i=0;i<csize;i++){
      *PL_ptr[i] = PL_temp[i];
  }


  nchasp =0;
  for(i=0;i<csize;i++){
  if(PL_ptr[i]->nr_PL > 0){
    PL_ptr[i]->active = 1;
    nchasp++;
  }else{
    PL_ptr[i]->active = 0;}
  }

// we don't calculate production and loss off N2, O2, and H2O
  for(i=0;i<csize;i++){
    if(PL_ptr[i]->name == "N2" || PL_ptr[i]->name == "O2" || PL_ptr[i]->name == "H2O")
    {
      printf("Deactivate: %i\n", i);
      PL_ptr[i]->active = 0;
        nchasp--;
    }
  }
  j=0;

//
//
////  PL_scheme = PL_temp;
//
  printf("number of active species: %i number declared %i\n",  nchasp, csize);
//
//  rcout=fopen("reaction_scheme","w");
//  for(i=0;i<csize;i++){
//    fprintf(rcout,"---------------------------------------------------\n");
//    fprintf(rcout," ");
//    if(PL_ptr[i]->active == 0 && strlen(PL_ptr[i]->name)>0){
//      fprintf(rcout,"%s  (%i) NOT CALCULATED \n",PL_ptr[i]->name,i);
//      fprintf(rcout,"\n");
//    }else if(strlen(PL_ptr[i]->name)==0){
//      fprintf(rcout,"%-6s  (%i) NO SPECIES \n",PL_ptr[i]->name,i);
//    }else{
//      fprintf(rcout,"%-6s  (%2i) \n",PL_ptr[i]->name,i);
//      fprintf(rcout,"YP = \n");
//      for(j=0;j<PL_ptr[i]->nr_PL;j++){
//        if(PL_ptr[i]->PL[j].PorL == PRODUCTION){
//          switch (PL_ptr[i]->PL[j].formula){
//            case 0:
//              fprintf(rcout,"  + %4.2f * K(%2i) (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[i]->PL[j].formula);
//              break;
//            case 1:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,\
//                PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,\
//                PL_ptr[i]->PL[j].formula);
//              break;
//            case 2:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] * Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].formula);
//              break;
//            case 3:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,PL_ptr[i]->PL[j].formula);
//              break;
//            case 4:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,PL_ptr[i]->PL[j].formula);
//              break;
//            case 5:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] * Y[%-5s]* Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[PL_ptr[i]->PL[j].comp2]->name,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].formula);
//              break;
//            case 6:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i  * Y[%-5s] ** %i  * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].exp3,PL_ptr[i]->PL[j].formula);
//              break;
//            case 7:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i  * Y[%-5s] ** %i  * Y[%-5s] ** %i  * Y[%-5s] ** %i(F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].exp3,\
//                PL_ptr[PL_ptr[i]->PL[j].comp4]->name,PL_ptr[i]->PL[j].exp4,PL_ptr[i]->PL[j].formula);
//              break;
//          }//end switch
//        }//end if production
//      }//end for j
//
//      fprintf(rcout,"YL = \n");
//      for(j=0;j<PL_ptr[i]->nr_PL;j++){
//        if(PL_ptr[i]->PL[j].PorL == LOSS){
//          switch (PL_ptr[i]->PL[j].formula){
//            case 0:
//              fprintf(rcout,"  + %4.2f * K(%2i) (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[i]->PL[j].formula);
//              break;
//            case 1:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].formula);
//              break;
//            case 2:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] * Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].formula);
//              break;
//            case 3:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,PL_ptr[i]->PL[j].formula);
//              break;
//            case 4:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,PL_ptr[i]->PL[j].formula);
//              break;
//            case 5:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] * Y[%-5s]* Y[%-5s] (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[PL_ptr[i]->PL[j].comp2]->name,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].formula);
//              break;
//            case 6:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i  * Y[%-5s] ** %i * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].exp3,PL_ptr[i]->PL[j].formula);
//              break;
//            case 7:
//              fprintf(rcout,"  + %4.2f * K(%2i) * Y[%-5s] ** %i  * Y[%-5s] ** %i * Y[%-5s] ** %i * Y[%-5s] ** %i (F=%i)\n",PL_ptr[i]->PL[j].coef,PL_ptr[i]->PL[j].r_nr,\
//                PL_ptr[PL_ptr[i]->PL[j].comp1]->name,PL_ptr[i]->PL[j].exp1,\
//                PL_ptr[PL_ptr[i]->PL[j].comp2]->name,PL_ptr[i]->PL[j].exp2,\
//                PL_ptr[PL_ptr[i]->PL[j].comp3]->name,PL_ptr[i]->PL[j].exp3,\
//                PL_ptr[PL_ptr[i]->PL[j].comp4]->name,PL_ptr[i]->PL[j].exp4,PL_ptr[i]->PL[j].formula);
//              break;
//          }//end switch
//        }//end if production
//      }//end for j
//    }//end else
//
//  }//end for i
//  fprintf(rcout,"---------------------------------------------------\n");
//
//  fclose(rcout);

  return;
}

void modelchem::calc_k( \
          double pressure,double temp_cbl,double temp_ft, \
          double zenith,int ldiuvar,int lchconst, double h_ref, \
          double t_ref_cbl,double p_ref_cbl,double q_ref_cbl, \
          double t_ref_ft,double p_ref_ft,double q_ref_ft, double* sc, double* dsc)
{
  int i;
  double coszen;
  double K,k1,k2,k3;
  double conv_cbl,conv_ft;
  double Rfact;
  int lday;

  double *c_cbl;
  double *c_ft;

  c_cbl = new double[csize];
  c_ft  = new double[csize];

  for(i=0; i<csize; i++)
  {
    c_cbl[i] = sc[i];
    c_ft[i]  = sc[i] + dsc[i];
  }



  Rfact=8.314e-2; //mbar*m3/K*mol
  if (lchconst ==1){
    conv_cbl= 6.023e8 * p_ref_cbl/(Rfact * temp_cbl);
    conv_ft = 6.023e8 * p_ref_ft /(Rfact * temp_ft);
  }else{
    conv_cbl= 6.023e8 * pressure /(Rfact * temp_cbl);
    conv_ft = 6.023e8 * pressure /(Rfact * temp_ft);
  }

  if(cos(zenith) > 0.){
    lday = 1;
    coszen=cos(zenith);
    if (ldiuvar == 0){
      // we need solar zenith angle for h_ref or 12 hour  !!!!!!!!!!!!!!!!!
      // for testing set a value of .8
      zenith = 1.012093;
      coszen = cos(zenith);
    }
  }else{
    lday = 0;
  }


  // adjust the Kreact depending on the func code
  for(i=0;i<rsize;i++)
  {
    if(RC_ptr[i]->RadDep == 1)
    {
      if(lday == 0){
        RC_ptr[i]->Keff_cbl= 0.0;
        RC_ptr[i]->Keff_cbl= 0.0;
      }else{
        switch(RC_ptr[i]->func1){
          case 0: // constant independent of sza
            RC_ptr[i]->Keff_cbl = RC_ptr[i]->KReact;
            RC_ptr[i]->Keff_ft  = RC_ptr[i]->KReact;
            break;
          case 1: // constant independent of sza
            RC_ptr[i]->Keff_cbl = RC_ptr[i]->A;
            RC_ptr[i]->Keff_ft  = RC_ptr[i]->A;
            break;
          case 2:// exponential function
            RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * exp(RC_ptr[i]->B /coszen );
            RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * exp(RC_ptr[i]->B /coszen );
            break;
          case 3: // powferfunction
            RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * powf(coszen ,RC_ptr[i]->B);
            RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * powf(coszen ,RC_ptr[i]->B);
            break;
          case 4: // powferfunction but special for JO3
          //need [H20] in kg/kg so c_cbl[H2O]*1e-9
             K = RC_ptr[i]->A * pow(coszen, RC_ptr[i]->B);
             RC_ptr[i]->Keff_cbl = K * RC_ptr[i]->D *  c_cbl[QLOC]*1.e-9 / \
                  (RC_ptr[i]->D * c_cbl[QLOC]*1.e-9  + RC_ptr[i]->E * (1.- c_cbl[QLOC]*1.e-9));
             RC_ptr[i]->Keff_ft = K * RC_ptr[i]->D *  c_cbl[QLOC]* 1.e-9 / \
                  (RC_ptr[i]->D * c_cbl[QLOC]* 1.e-9 + RC_ptr[i]->E * (1.- c_cbl[QLOC]*1.e-9));
            break;
          default: //if someone put by mistake a number
             RC_ptr[i]->Keff_cbl = 1;
             RC_ptr[i]->Keff_ft  = 1;
             break;
        }
      }
    }
    else
    {   // normal (no photolysis) reactions func can has a different meaning
      switch ( RC_ptr[i]->func1 ){
        case 0:
          //do nothing K is in PPB and constant
        case 1: // K in cm3/molecules*sec
          RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * conv_cbl;
          RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * conv_ft;
          break;
        case 2: //temperature depence of K for both cbl and ft
          RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * exp(RC_ptr[i]->B / temp_cbl)* conv_cbl;
          RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * exp(RC_ptr[i]->B / temp_ft )* conv_ft;
          break;
        case 3: //more complex temperature dependence
          RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * (powf(temp_cbl/RC_ptr[i]->B,RC_ptr[i]->C)) * exp(RC_ptr[i]->D / temp_cbl)* conv_cbl;
          RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * (powf(temp_ft /RC_ptr[i]->B,RC_ptr[i]->C)) * exp(RC_ptr[i]->D / temp_ft )* conv_ft;
          break;
        case 4: //use Lindemann-Hinshelwood equation  4 = second order 5 = first order so no conv_XXX factor
        case 5:  //first CBL
          k1=RC_ptr[i]->A * (powf(temp_cbl/300,RC_ptr[i]->B)) * exp(RC_ptr[i]->C/temp_cbl) * conv_cbl * 1e9;
          k2=RC_ptr[i]->D * (powf(temp_cbl/300,RC_ptr[i]->E)) * exp(RC_ptr[i]->F/temp_cbl);
          K = k1*k2/(k1+k2) * RC_ptr[i]->G;
          if (RC_ptr[i]->func1 == 4) {
              RC_ptr[i]->Keff_cbl = K * conv_cbl;
          }else{
              RC_ptr[i]->Keff_cbl = K;
          }
          //for FT
          k1=RC_ptr[i]->A * (powf(temp_ft/300,RC_ptr[i]->B)) * exp(RC_ptr[i]->C/temp_ft) * conv_ft * 1e9;
          k2=RC_ptr[i]->D * (powf(temp_ft/300,RC_ptr[i]->E)) * exp(RC_ptr[i]->F/temp_ft);
          K = k1*k2/(k1+k2) * RC_ptr[i]->G;
          if (RC_ptr[i]->func1 == 4) {
              RC_ptr[i]->Keff_ft = K * conv_ft;
          }else{
              RC_ptr[i]->Keff_ft = K;
          }
          break;
        case 6://special function of reaction 2H02->H202
           //first CBL
           k1 = RC_ptr[i]->A * exp(RC_ptr[i]->B / temp_cbl)* conv_cbl;
           k2 = RC_ptr[i]->C * exp(RC_ptr[i]->D / temp_cbl)* conv_cbl*conv_cbl *1e9;
           k3 = RC_ptr[i]->E * exp(RC_ptr[i]->F / temp_cbl)* conv_cbl * c_cbl[QLOC];
           RC_ptr[i]->Keff_cbl = (k1+k2)*(1+k3);
           //for FT
           k1 = RC_ptr[i]->A * exp(RC_ptr[i]->B / temp_ft)* conv_ft;
           k2 = RC_ptr[i]->C * exp(RC_ptr[i]->D / temp_ft)* conv_ft*conv_ft *1e9;
           k3 = RC_ptr[i]->E * exp(RC_ptr[i]->F / temp_ft)* conv_ft * c_ft[QLOC];
           RC_ptr[i]->Keff_ft = (k1+k2)*(1+k3);
           break;
         case 7: // same as 3 but third order so conv_XXX**2
           RC_ptr[i]->Keff_cbl = RC_ptr[i]->A * (powf(temp_cbl/RC_ptr[i]->B,RC_ptr[i]->C)) * exp(RC_ptr[i]->D / temp_cbl)* (powf(conv_cbl,2));
           RC_ptr[i]->Keff_ft  = RC_ptr[i]->A * (powf(temp_ft /RC_ptr[i]->B,RC_ptr[i]->C)) * exp(RC_ptr[i]->D / temp_ft )* (powf(conv_ft,2));
         default: // if someone put by mistake a different number
          // write (*,*) 'FUNCTIONS GREATER THEN 7 NOT IMPLEMENTED'
          // STOP
          break;
      }//end switch
    }//end if
  } //tnor
}

// CvH iter: cf_switch: BL or FT
// CvH ynew = array scalars out, ycurrent = array scalars in
void modelchem::iter(int cf_switch, double dt, double ynew[], double ycurrent[])
{
 //t is the number of sec since the beginning of the run (for dtime=1)
  int n,j;
  int iiter,niter;
  double YP,YL,YPL,*YPL_ptr;
  double kreact;


// start Gauss Seidel iterations;
// Gauss-Seidel iterations;

  niter=4;
  
  //for( n=0;n<nr_chemicals;n++)
  //  printf("CvH PL_ptr: %p\n", PL_ptr[n]); 
  //return;

  for( iiter=0;iiter<niter;iiter++)
  {
    for( n=0;n<nr_chemicals;n++)
    {
       if (PL_ptr[n]->active == 1){
       if (PL_ptr[n]->name == "CO") break;  // don't do calculations for CO;
       if (PL_ptr[n]->name == "H2O") break; // don't do calculations for H2O;
       if (PL_ptr[n]->name == "PRODUC") break;

       YL = 0.;
       YP = 0.;
       for(j = 0; j<PL_ptr[n]->nr_PL;j++){

         if (PL_ptr[n]->PL[j].PorL == PRODUCTION ) {  // this the production
           YPL_ptr = &YP;
         }else{
           YPL_ptr = &YL;
         }

         if ( cf_switch == CBL ) {
            kreact = PL_ptr[n]->PL[j].coef * RC_ptr[PL_ptr[n]->PL[j].r_nr]->Keff_cbl;
         }else{;
            kreact = PL_ptr[n]->PL[j].coef * RC_ptr[PL_ptr[n]->PL[j].r_nr]->Keff_ft;
         }

         switch (PL_ptr[n]->PL[j].formula){
           case 0:
             YPL = kreact;
             break;
           case 1:
             YPL = kreact * ynew[PL_ptr[n]->PL[j].comp1];
             break;
           case 2:
             YPL = kreact * ynew[PL_ptr[n]->PL[j].comp1] * ynew[PL_ptr[n]->PL[j].comp2];
             break;
           case 3:
             YPL = kreact * powf(ynew[PL_ptr[n]->PL[j].comp1], PL_ptr[n]->PL[j].exp1);
             break;
           case 4:
             YPL = kreact * (powf(ynew[PL_ptr[n]->PL[j].comp1], PL_ptr[n]->PL[j].exp1)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp2], PL_ptr[n]->PL[j].exp2));
             break;
           case 5:
             YPL = kreact * ynew[PL_ptr[n]->PL[j].comp1] * ynew[PL_ptr[n]->PL[j].comp2] * ynew[PL_ptr[n]->PL[j].comp3];
             break;
           case 6:
             YPL = kreact * (powf(ynew[PL_ptr[n]->PL[j].comp1], PL_ptr[n]->PL[j].exp1)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp2], PL_ptr[n]->PL[j].exp2)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp3], PL_ptr[n]->PL[j].exp3)) ;
             break;
           case 7:
             YPL = kreact * (powf(ynew[PL_ptr[n]->PL[j].comp1], PL_ptr[n]->PL[j].exp1)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp2], PL_ptr[n]->PL[j].exp2)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp3], PL_ptr[n]->PL[j].exp3)) \
                          * (powf(ynew[PL_ptr[n]->PL[j].comp4], PL_ptr[n]->PL[j].exp4)) ;
             break;
         }

         *YPL_ptr = *YPL_ptr + YPL;
       } //j=1, PL_ptr[n]->nr_PL;

       ynew[n] = std::max(0.0, (ycurrent[n]+dt*YP)/(1.0+dt*YL));

      }  //active == true
    } //n=1,nchsp
  } //iiter
}

