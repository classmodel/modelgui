#include "modelinput.h"

modelinput::modelinput()
{
  runtime    = -1.;
  dt         = -1.;
  sinperiod  = -1.;

  // mixed-layer variables
  sw_ml      = true;
  sw_ftcws   = false;
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

  // shallow-cumulus
  sw_cu      = true;

  dFz        = false;

  // chemistry
  sw_chem    = false;
  sw_chem_constant = false;
  csize      = 22;
  rsize      = 27;
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

  // shallow-cumulus
  sw_cu      = ref.sw_cu;

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

    // shallow-cumulus
    sw_cu      = ref.sw_cu;

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
