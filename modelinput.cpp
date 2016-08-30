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

#include "modelinput.h"

modelinput::modelinput()
{
  runtime    = -1.;
  dt         = -1.;
  sinperiod  = -1.;

  // mixed-layer variables
  sw_ml      = true;
  sw_ftcws   = false;
  sw_shearwe = false;
  h          = -1.;
  Ps         = -1.;
  omegas     = -1.;
  fc         = -1.;
  
  theta      = -1.;
  dtheta     = -1.;
  gammatheta = -1.;
  advtheta   = -1.;
  beta       = -1.;
  wtheta     = -1.;
  sw_wtheta  = false;
  
  q          = -1.;
  dq         = -1.;
  gammaq     = -1.;
  advq       = -1.;
  wq         = -1.;
  sw_wq      = false;
  
  sw_wind    = false;
  u          = -1.;
  du         = -1.;
  gammau     = -1.;
  advu       = -1.;
  
  v          = -1.;
  dv         = -1.;
  gammav     = -1.;
  advv       = -1.;

  // BvS; a scalar, without the need for the chemistry scheme :)
  sca        = -1.;
  dsca       = -1.;
  gammasca   = -1.;
  advsca     = -1.;
  wsca       = -1.;

  CO2        = -1.;
  gammaCO2   = -1.;
  advCO2     = -1.;
  wCO2       = -1.;

  // scalars for chemistry
  nsc        = 22; // nsc hardcoded...
  sc         = new double[nsc];
  dsc        = new double[nsc];
  gammasc    = new double[nsc];
  advsc      = new double[nsc];
  wsc        = new double[nsc];
  sw_wsc     = new int[nsc];

  for(int i=0; i<nsc; i++)
  {
    sc[i]       = -1.;
    dsc[i]      = -1.;
    gammasc[i]  = -1.;
    advsc[i]    = -1.;
    wsc[i]      = -1.;
    sw_wsc[i]   = -1.;
  }

  // surface layer variables
  sw_sl      = false;
  ustar      = -1.;
  z0m        = -1.;
  z0h        = -1.;

  // radiation parameters
  sw_rad     = false;
  lat        = -1.;
  lon        = -1.;
  doy        = -1.;
  tstart     = -1.;
  cc         = -1.; 
  Q          = -1.;

  // land surface parameters
  sw_ls      = false;
  sw_jarvis  = true;
  C3C4       = -1;
  sw_sea     = true;
  wg         = -1.;
  w2         = -1.;
  Tsoil      = -1.;
  T2         = -1.;
  
  a          = -1.;
  b          = -1.;
  p          = -1.;
  CGsat      = -1.;
  
  wsat       = -1.;
  wfc        = -1.;
  wwilt      = -1.;
  
  C1sat      = -1.;
  C2ref      = -1.;
    
  LAI        = -1.;
  gD         = -1.;
  rsmin      = -1.;
  rssoilmin  = -1.;
  alpha      = -1.;
  
  Ts         = -1.;
  
  cveg       = -1.;
  Wmax       = -1.;
  Wl         = -1.;
  
  Lambda     = -1.;

  c_beta     = -1.;

  // shallow-cumulus
  sw_cu      = true;
  sw_curad   = false;

  dFz        = -1;

  // chemistry
  sw_chem    = false;
  sw_chem_constant = false;
  csize      = 22;
  rsize      = 28;
  sw_reactions = new bool[rsize];
  for(int n=0; n<rsize; n++)
    sw_reactions[n] = true;
  P_ref      = -1.;
  Tcbl_ref   = -1.;
  Tfc_ref    = -1.;
  qcbl_ref   = -1.;
  qfc_ref    = -1.;
  tod_ref    = -1.;
  stocoef    = -1.;
}

modelinput::modelinput(const modelinput &ref)
{
  runtime    = ref.runtime;
  dt         = ref.dt;
  sinperiod  = ref.sinperiod;

  // mixed-layer variables
  sw_ml      = ref.sw_ml;
  sw_ftcws   = ref.sw_ftcws;
  sw_shearwe = ref.sw_shearwe;
  h          = ref.h;
  Ps         = ref.Ps;
  omegas     = ref.omegas;
  fc         = ref.fc;

  theta      = ref.theta;
  dtheta     = ref.dtheta;
  gammatheta = ref.gammatheta;
  advtheta   = ref.advtheta;
  beta       = ref.beta;
  wtheta     = ref.wtheta;
  sw_wtheta  = ref.sw_wtheta;

  q          = ref.q;
  dq         = ref.dq;
  gammaq     = ref.gammaq;
  advq       = ref.advq;
  wq         = ref.wq;
  sw_wq      = ref.sw_wq;
               
  sw_wind    = ref.sw_wind;
  u          = ref.u;
  du         = ref.du;
  gammau     = ref.gammau;
  advu       = ref.advu;
              
  v          = ref.v;
  dv         = ref.dv;
  gammav     = ref.gammav;
  advv       = ref.advv;

  // scalar
  sca        = ref.sca;
  dsca       = ref.dsca;
  gammasca   = ref.gammasca;
  advsca     = ref.advsca;
  wsca       = ref.wsca;

  CO2        = ref.CO2;
  dCO2       = ref.dCO2;
  gammaCO2   = ref.gammaCO2;
  advCO2     = ref.advCO2;
  wCO2       = ref.wCO2;

  // scalars for chemistry
  nsc        = ref.nsc;
  sc         = new double[nsc];
  dsc        = new double[nsc];
  gammasc    = new double[nsc];
  advsc      = new double[nsc];
  wsc        = new double[nsc];
  sw_wsc     = new int[nsc];

  for(int i=0; i<nsc; i++)
  {
    sc[i]       = ref.sc[i];
    dsc[i]      = ref.dsc[i];
    gammasc[i]  = ref.gammasc[i];
    advsc[i]    = ref.advsc[i];
    wsc[i]      = ref.wsc[i];
    sw_wsc[i]   = ref.sw_wsc[i];
  }

  // surface layer variables
  sw_sl      = ref.sw_sl;
  ustar      = ref.ustar;
  z0m        = ref.z0m;
  z0h        = ref.z0h;

  // radiation parameters
  sw_rad     = ref.sw_rad;
  lat        = ref.lat;
  lon        = ref.lon;
  doy        = ref.doy;
  tstart     = ref.tstart;
  cc         = ref.cc;
  Q          = ref.Q;

  // land surface parameters
  sw_ls      = ref.sw_ls;
  sw_jarvis  = ref.sw_jarvis;
  C3C4       = ref.C3C4;
  sw_sea     = ref.sw_sea;  
  wg         = ref.wg;
  w2         = ref.w2;
  Tsoil      = ref.Tsoil;
  T2         = ref.T2;

  a          = ref.a;
  b          = ref.b;
  p          = ref.p;
  CGsat      = ref.CGsat;  

  wsat       = ref.wsat;
  wfc        = ref.wfc;
  wwilt      = ref.wwilt;

  C1sat      = ref.C1sat;
  C2ref      = ref.C2ref;

  LAI        = ref.LAI;
  gD         = ref.gD;
  rsmin      = ref.rsmin;
  rssoilmin  = ref.rssoilmin;
  alpha      = ref.alpha;

  Ts         = ref.Ts;

  cveg       = ref.cveg;
  Wmax       = ref.Wmax;
  Wl         = ref.Wl;

  Lambda     = ref.Lambda;

  c_beta     = ref.c_beta;

  // shallow-cumulus
  sw_cu      = ref.sw_cu;
  sw_curad   = ref.sw_curad;

  dFz        = ref.dFz;

  // chemistry
  sw_chem      = ref.sw_chem;
  sw_chem_constant = ref.sw_chem_constant;
  sw_photo_constant = ref.sw_photo_constant;
  csize        = ref.csize;
  rsize        = ref.rsize;
  reactions    = ref.reactions; // CvH warning...
  sw_reactions = new bool[rsize];
  for(int n=0; n<rsize; n++)
    sw_reactions[n] = ref.sw_reactions[n];

  P_ref      = ref.P_ref;
  Tcbl_ref   = ref.Tcbl_ref;
  Tfc_ref    = ref.Tfc_ref;
  qcbl_ref   = ref.qcbl_ref;
  qfc_ref    = ref.qfc_ref;
  tod_ref    = ref.tod_ref;

  stocoef    = ref.stocoef;
}

bool modelinput::operator!=(const modelinput &ref)
{
  if(runtime         != ref.runtime)
    return true;	  
  else if(dt         != ref.dt)
    return true;	  
  else if(sinperiod  != ref.sinperiod)
    return true;	  

  else if(sw_ml      != ref.sw_ml)
    return true;	  
  else if(sw_ftcws   != ref.sw_ftcws)
    return true;	  
  else if(sw_shearwe != ref.sw_shearwe)
    return true;	  
  else if(h          != ref.h)
    return true;	  
  else if(Ps         != ref.Ps)
    return true;	  
  else if(omegas     != ref.omegas)
    return true;	  
  else if(fc         != ref.fc)
    return true;	  

  else if(theta      != ref.theta)
    return true;	  
  else if(dtheta     != ref.dtheta)
    return true;	  
  else if(gammatheta != ref.gammatheta)
    return true;	  
  else if(advtheta   != ref.advtheta)
    return true;	  
  else if(beta       != ref.beta)
    return true;	  
  else if(wtheta     != ref.wtheta)
    return true;	  
  else if(sw_wtheta  != ref.sw_wtheta)
    return true;	  

  else if(q          != ref.q)
    return true;	  
  else if(dq         != ref.dq)
    return true;	  
  else if(gammaq     != ref.gammaq)
    return true;	  
  else if(advq       != ref.advq)
    return true;	  
  else if(wq         != ref.wq)
    return true;	  
  else if(sw_wq      != ref.sw_wq)
    return true;	  

  else if(sw_wind    != ref.sw_wind)
    return true;	  
  else if(u          != ref.u)
    return true;	  
  else if(du         != ref.du)
    return true;	  
  else if(gammau     != ref.gammau)
    return true;	  
  else if(advu       != ref.advu)
    return true;	  

  else if(v          != ref.v)
    return true;	  
  else if(dv         != ref.dv)
    return true;	  
  else if(gammav     != ref.gammav)
    return true;	  
  else if(advv       != ref.advv)
    return true;	  

  // scalar
  else if(sca        != ref.sca)
    return true;          
  else if(dsca       != ref.dsca)
    return true;          
  else if(gammasca   != ref.gammasca)
    return true;          
  else if(advsca     != ref.advsca)
    return true;          
  else if(wsca       != ref.wsca)
    return true;          

  else if(CO2        != ref.CO2)
    return true;          
  else if(dCO2       != ref.dCO2)
    return true;          
  else if(gammaCO2   != ref.gammaCO2)
    return true;          
  else if(advCO2     != ref.advCO2)
    return true;          
  else if(wCO2       != ref.wCO2)
    return true;	  

  // chemistry species :'(
  else if(nsc        != ref.nsc)
    return false;
    
  for(int i=0; i<nsc; i++)
  {
    if(sc[i]       != ref.sc[i])
      return true;	  
    if(dsc[i]      != ref.dsc[i])
      return true;	  
    if(gammasc[i]  != ref.gammasc[i])
      return true;	  
    if(advsc[i]    != ref.advsc[i])
      return true;	  
    if(wsc[i]      != ref.wsc[i])
      return true;	  
    if(sw_wsc[i]   != ref.sw_wsc[i])
      return true;	  
  }

  // surface layer variables
  if(sw_sl           != ref.sw_sl)
    return true;	  
  else if(ustar      != ref.ustar)
    return true;	  
  else if(z0m        != ref.z0m)
    return true;	  
  else if(z0h        != ref.z0h)
    return true;	  

  // radiatio!n parameters
  else if(sw_rad     != ref.sw_rad)
    return true;	  
  else if(lat        != ref.lat)
    return true;	  
  else if(lon        != ref.lon)
    return true;	  
  else if(doy        != ref.doy)
    return true;	  
  else if(tstart     != ref.tstart)
    return true;	  
  else if(cc         != ref.cc)
    return true;	  
  else if(Q          != ref.Q)
    return true;	  

  // land sur!face parameters
  else if(sw_ls      != ref.sw_ls)
    return true;	  
  else if(sw_jarvis  != ref.sw_jarvis)
    return true;	  
  else if(C3C4       != ref.C3C4)
    return true;	  
  else if(sw_sea     != ref.sw_sea)
    return true;	  
  else if(wg         != ref.wg)
    return true;	  
  else if(w2         != ref.w2)
    return true;	  
  else if(Tsoil      != ref.Tsoil)
    return true;	  
  else if(T2         != ref.T2)
    return true;	  

  else if(a          != ref.a)
    return true;	  
  else if(b          != ref.b)
    return true;	  
  else if(p          != ref.p)
    return true;	  
  else if(CGsat      != ref.CGsat)
    return true;	  

  else if(wsat       != ref.wsat)
    return true;	  
  else if(wfc        != ref.wfc)
    return true;	  
  else if(wwilt      != ref.wwilt)
    return true;	  

  else if(C1sat      != ref.C1sat)
    return true;	  
  else if(C2ref      != ref.C2ref)
    return true;	  

  else if(LAI        != ref.LAI)
    return true;	  
  else if(gD         != ref.gD)
    return true;	  
  else if(rsmin      != ref.rsmin)
    return true;	  
  else if(rssoilmin  != ref.rssoilmin)
    return true;	  
  else if(alpha      != ref.alpha)
    return true;	  

  else if(Ts         != ref.Ts)
    return true;	  

  else if(cveg       != ref.cveg)
    return true;	  
  else if(Wmax       != ref.Wmax)
    return true;	  
  else if(Wl         != ref.Wl)
    return true;	  

  else if(Lambda     != ref.Lambda)
    return true;	  

  else if(c_beta     != ref.c_beta)
    return true;

  // shallow-!cumulus
  else if(sw_cu      != ref.sw_cu)
    return true;	  
  else if(sw_curad   != ref.sw_curad)
    return true;
  else if(dFz        != ref.dFz)
    return true;	  

  // chemistry :'(
  else if(sw_chem            != ref.sw_chem)
    return true;	  
  else if(sw_chem_constant   != ref.sw_chem_constant)
    return true;	  
  else if(sw_photo_constant  != ref.sw_photo_constant)
    return true;	  
  else if(csize              != ref.csize)
    return true;	  
  else if(rsize              != ref.rsize)
    return true;	  
  else if(reactions          != ref.reactions)
    return true;	  

  for(int n=0; n<rsize; n++)
    if(sw_reactions[n] != ref.sw_reactions[n])
      return true;	  

  if(P_ref           != ref.P_ref)
    return true;     
  else if(Tcbl_ref   != ref.Tcbl_ref)
    return true;     
  else if(Tfc_ref    != ref.Tfc_ref)
    return true;     
  else if(qcbl_ref   != ref.qcbl_ref)
    return true;     
  else if(qfc_ref    != ref.qfc_ref)
    return true;     
  else if(tod_ref    != ref.tod_ref)
    return true;     
  else if(stocoef    != ref.stocoef)
    return true;	  
  else
    return false;
}

modelinput &modelinput::operator=(const modelinput &ref)
{
  if(this != &ref)
  {
    runtime    = ref.runtime;
    dt         = ref.dt;
    sinperiod  = ref.sinperiod;

    // mixed-layer variables
    sw_ml      = ref.sw_ml;
    sw_ftcws   = ref.sw_ftcws;
    sw_shearwe = ref.sw_shearwe;
    h          = ref.h;
    Ps         = ref.Ps;
    omegas     = ref.omegas;
    fc         = ref.fc;

    theta      = ref.theta;
    dtheta     = ref.dtheta;
    gammatheta = ref.gammatheta;
    advtheta   = ref.advtheta;
    beta       = ref.beta;
    wtheta     = ref.wtheta;
    sw_wtheta  = ref.sw_wtheta;

    q          = ref.q;
    dq         = ref.dq;
    gammaq     = ref.gammaq;
    advq       = ref.advq;
    wq         = ref.wq;
    sw_wq      = ref.sw_wq;

    sw_wind    = ref.sw_wind;
    u          = ref.u;
    du         = ref.du;
    gammau     = ref.gammau;
    advu       = ref.advu;

    v          = ref.v;
    dv         = ref.dv;
    gammav     = ref.gammav;
    advv       = ref.advv;

    // scalar
    sca        = ref.sca;
    dsca       = ref.dsca;
    gammasca   = ref.gammasca;
    advsca     = ref.advsca;
    wsca       = ref.wsca;

    CO2        = ref.CO2;
    dCO2       = ref.dCO2;
    gammaCO2   = ref.gammaCO2;
    advCO2     = ref.advCO2;
    wCO2       = ref.wCO2;

    // scalars for chemistry
    nsc        = ref.nsc;
    delete[] sc;
    delete[] dsc;
    delete[] gammasc;
    delete[] advsc;
    delete[] wsc;
    delete[] sw_wsc;
    sc         = new double[nsc];
    dsc        = new double[nsc];
    gammasc    = new double[nsc];
    advsc      = new double[nsc];
    wsc        = new double[nsc];
    sw_wsc     = new int[nsc];

    for(int i=0; i<nsc; i++)
    {
      sc[i]       = ref.sc[i];
      dsc[i]      = ref.dsc[i];
      gammasc[i]  = ref.gammasc[i];
      advsc[i]    = ref.advsc[i];
      wsc[i]      = ref.wsc[i];
      sw_wsc[i]   = ref.sw_wsc[i];
    }

    // surface layer variables
    sw_sl      = ref.sw_sl;
    ustar      = ref.ustar;
    z0m        = ref.z0m;
    z0h        = ref.z0h;

    // radiation parameters
    sw_rad     = ref.sw_rad;
    lat        = ref.lat;
    lon        = ref.lon;
    doy        = ref.doy;
    tstart     = ref.tstart;
    cc         = ref.cc;
    Q          = ref.Q;

    // land surface parameters
    sw_ls      = ref.sw_ls;
    sw_jarvis  = ref.sw_jarvis;
    C3C4       = ref.C3C4;
    sw_sea     = ref.sw_sea;
    wg         = ref.wg;
    w2         = ref.w2;
    Tsoil      = ref.Tsoil;
    T2         = ref.T2;

    a          = ref.a;
    b          = ref.b;
    p          = ref.p;
    CGsat      = ref.CGsat;

    wsat       = ref.wsat;
    wfc        = ref.wfc;
    wwilt      = ref.wwilt;

    C1sat      = ref.C1sat;
    C2ref      = ref.C2ref;

    LAI        = ref.LAI;
    gD         = ref.gD;
    rsmin      = ref.rsmin;
    rssoilmin  = ref.rssoilmin;
    alpha      = ref.alpha;

    Ts         = ref.Ts;

    cveg       = ref.cveg;
    Wmax       = ref.Wmax;
    Wl         = ref.Wl;

    Lambda     = ref.Lambda;

    c_beta     = ref.c_beta;

    // shallow-cumulus
    sw_cu      = ref.sw_cu;
    sw_curad   = ref.sw_curad;

    dFz        = ref.dFz;

    // chemistry
    sw_chem    = ref.sw_chem;
    sw_chem_constant = ref.sw_chem_constant;
    sw_photo_constant = ref.sw_photo_constant;
    csize      = ref.csize;
    rsize      = ref.rsize;
    reactions  = ref.reactions; // CvH warning...
    delete[] sw_reactions;
    sw_reactions = new bool[rsize];
    for(int n=0; n<rsize; n++)
      sw_reactions[n] = ref.sw_reactions[n];

    P_ref      = ref.P_ref;
    Tcbl_ref   = ref.Tcbl_ref;
    Tfc_ref    = ref.Tfc_ref;
    qcbl_ref   = ref.qcbl_ref;
    qfc_ref    = ref.qfc_ref;
    tod_ref    = ref.tod_ref;

    stocoef    = ref.stocoef;
  }
  return *this;
}
