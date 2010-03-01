class modeloutput
{
public:
  double *t;         // time [s]

  // mixed-layer variables
  double *h;         // initial ABL height [m]
  double *Ps;        // surface pressure [Pa]
  double *ws;        // large scale vertical velocity [m s-1]
  
  double *theta;     // initial mixed-layer potential temperature [K]
  double *thetav;    // initial mixed-layer virtual potential temperature [K]
  double *dtheta;    // initial potential temperature jump at h [K]
  double *dthetav;   // initial virtual potential temperature jump at h [K]
  double *gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double *advtheta;  // advection of heat [K s-1]
  double *beta;      // entrainment ratio for virtual heat [-]
  double *wtheta;    // surface kinematic heat flux [K m s-1]
  double *wthetav;   // surface kinematic virtual heat flux [K m s-1]
  
  double *q;         // mixed-layer specific humidity [kg kg-1]
  //double *qsat;      // mixed-layer saturated specific humidity [kg kg-1]
  //double *e;         // mixed-layer vapor pressure [Pa]
  //double *esat;      // mixed-layer saturated vapor pressure [Pa]
  double *dq;        // initial specific humidity jump at h [kg kg-1]
  double *gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  double *advq;      // advection of moisture [kg kg-1 s-1]
  double *wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  
  double *u;         // initial mixed-layer u-wind speed [m s-1]
  double *du;        // initial u-wind jump at h [m s-1]
  double *gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double *advu;      // advection of u-wind [m s-2]
  
  double *v;         // initial mixed-layer u-wind speed [m s-1]
  double *dv;        // initial u-wind jump at h [m s-1]
  double *gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double *advv;      // advection of v-wind [m s-2]
  
  double *ustar;     // friction velocity [m s-1]
  double *uw;        // u-momentum flux [m2 s-2]
  double *vw;        // v-momentum flux [m2 s-2]

  modeloutput(int tsteps)
  {
    t          = new double[tsteps];   // time [s]

    // mixed-layer variables
    h          = new double[tsteps];   // initial ABL height [m]
    Ps         = new double[tsteps];   // surface pressure [Pa]
    ws         = new double[tsteps];   // large scale vertical velocity [m s-1]
    
    theta      = new double[tsteps];   // initial mixed-layer potential temperature [K]
    thetav     = new double[tsteps];   // initial mixed-layer virtual potential temperature [K]
    dtheta     = new double[tsteps];   // initial potential temperature jump at h [K]
    dthetav    = new double[tsteps];   // initial virtual potential temperature jump at h [K]
    gammatheta = new double[tsteps];   // free atmosphere potential temperature lapse rate [K m-1]
    advtheta   = new double[tsteps];   // advection of heat [K s-1]
    beta       = new double[tsteps];   // entrainment ratio for virtual heat [-]
    wtheta     = new double[tsteps];   // surface kinematic heat flux [K m s-1]
    wthetav    = new double[tsteps];   // surface kinematic virtual heat flux [K m s-1]
    
    q          = new double[tsteps];   // mixed-layer specific humidity [kg kg-1]
    //qsat       = new double[tsteps];   // mixed-layer saturated specific humidity [kg kg-1]
    //e          = new double[tsteps];   // mixed-layer vapor pressure [Pa]
    //esat       = new double[tsteps];   // mixed-layer saturated vapor pressure [Pa]
    dq         = new double[tsteps];   // initial specific humidity jump at h [kg kg-1]
    gammaq     = new double[tsteps];   // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    advq       = new double[tsteps];   // advection of moisture [kg kg-1 s-1]
    wq         = new double[tsteps];   // surface kinematic moisture flux [kg kg-1 m s-1]
    
    u          = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
    du         = new double[tsteps];   // initial u-wind jump at h [m s-1]
    gammau     = new double[tsteps];   // free atmosphere u-wind speed lapse rate [s-1]
    advu       = new double[tsteps];   // advection of u-wind [m s-2]
    
    v          = new double[tsteps];   // initial mixed-layer u-wind speed [m s-1]
    dv         = new double[tsteps];   // initial u-wind jump at h [m s-1]
    gammav     = new double[tsteps];   // free atmosphere v-wind speed lapse rate [s-1]
    advv       = new double[tsteps];   // advection of v-wind [m s-2]
    
    ustar      = new double[tsteps];   // friction velocity [m s-1]
    uw         = new double[tsteps];   // u-momentum flux [m2 s-2]
    vw         = new double[tsteps];   // v-momentum flux [m2 s-2]
  }
};

