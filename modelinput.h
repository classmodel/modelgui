#include "modelchemtypes.h"

// Model input class
class modelinput
{
public:
  // general model variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]
  double sinperiod; // period for sinusoidal flux [s]

  // mixed-layer variables  
  bool   sw_ml;     // mixed-layer model switch;
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double ws;        // large scale vertical velocity [m s-1]
  double fc;        // Coriolis parameter [s-1]
  
  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  bool   sw_wtheta; // switch for sinusoidal wtheta
  
  double q;         // initial mixed-layer specific humidity [kg kg-1]
  double dq;        // initial specific humidity jump at h [kg kg-1]
  double gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  double advq;      // advection of moisture [kg kg-1 s-1]
  double wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  bool   sw_wq;     // switch for sinusoidal wq

  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  
  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]

  int nsc;
  double *sc;       // initial mixed-layer scalar [kg kg-1]
  double *dsc;      // initial scalar jump at h [kg kg-1]
  double *gammasc;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double *advsc;    // advection of moisture [kg kg-1 s-1]
  double *wsc;      // surface kinematic moisture flux [kg kg-1 m s-1]
  bool   *sw_wsc;   // switch for sinusoidal wsc

  // surface layer variables
  bool   sw_sl;     // surface layer switch
  double ustar;     // surface friction velocity [m s-1]
  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]

  // radiation parameters
  bool   sw_rad;    // radiation switch
  double lat;       // latitude [deg]
  double lon;       // longitude [deg]
  double doy;       // day of the year [-]
  double tstart;    // time of the day [h UTC]
  double cc;        // cloud cover fraction [-]
  double Q;         // net radiation [W m-2]

  // land surface parameters
  bool   sw_ls;     // land surface switch
  bool   sw_sea;    // land / sea switch
  double wg;        // volumetric water content top soil layer [m3 m-3]
  double w2;        // volumetric water content deeper soil layer [m3 m-3]
  double Tsoil;     // temperature top soil layer [K]
  double T2;        // temperature deeper soil layer [K]
  
  double a;         // Clapp and Hornberger retention curve parameter a
  double b;         // Clapp and Hornberger retention curve parameter b
  double p;         // Clapp and Hornberger retention curve parameter p 
  double CGsat;     // saturated soil conductivity for heat
  
  double wsat;      // saturated volumetric water content ECMWF config [-]
  double wfc;       // volumetric water content field capacity [-]
  double wwilt;     // volumetric water content wilting point [-]
  
  double C1sat;     // Coefficient force term moisture [-]
  double C2ref;     // Coefficient restore term moisture [-]
    
  double LAI;       // leaf area index [-]
  double gD;        // correction factor transpiration for VPD [-]
  double rsmin;     // minimum resistance transpiration [s m-1]
  double rssoilmin; // minimum resistance soil evaporation [s m-1]
  double alpha;     // surface albedo [-]
    
  double Ts;        // initial surface temperature [K]
    
  double cveg;      // vegetation fraction [-]
  double Wmax;      // thickness of water layer on wet vegetation [m]
  double Wl;        // equivalent water layer depth for wet vegetation [m]
  
  double Lambda;    // thermal diffusivity skin layer [-]

  // chemistry
  bool   sw_chem;
  Reaction *reactions;
  int    rsize;
  int    csize;

  modelinput()
  {
    runtime    = -1.;
    dt         = -1.;

    // mixed-layer variables
    sw_ml      = true;
    h          = -1.;
    Ps         = -1.;
    ws         = -1.;
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

    // scalars for chemistry
    nsc        = 0;
    //sc         = new double[nsc];
    //dsc        = new double[nsc];
    //gammasc    = new double[nsc];
    //advsc      = new double[nsc];
    //wsc        = new double[nsc];
    //sw_wsc     = new bool[nsc];

    //for(int i=0; i<nsc; i++)
    //{
    //  sc[i]       = -1.;
    //  dsc[i]      = -1.;
    //  gammasc[i]  = -1.;
    //  advsc[i]    = -1.;
    //  wsc[i]      = -1.;
    //  sw_wsc[i]   = -1.;
    //}

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

    // chemistry
    sw_chem    = false;
    csize      = -1;
    rsize      = -1;
  }
};

