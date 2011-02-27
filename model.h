#include <cstring>
#include "modelinput.h"
#include "modeloutput.h"
#include "modelchem.h"

class model
{
public:
  model(modelinput*);
  void runmodel();
  modelinput input;
  modeloutput *output;
  void run2file(std::string, std::string);

private:
  void initmodel();

  void runcumodel();

  void runmlmodel();
  void intmlmodel();

  void runslmodel();

  void runradmodel();

  void runlsmodel();
  void intlsmodel();

  void initchemmodel();
  void runchemmodel(double);

  void store();

  double ribtol(double, double, double, double);
  inline double psim(double);
  inline double psih(double);

  // model constants
  double Lv;        // heat of vaporization [J kg-1]
  double cp;        // specific heat of dry air [J kg-1 K-1]
  double rho;       // density of air [kg m-3]
  double k;         // Von Karman constant [-]
  double g;         // gravity acceleration [m s-2]
  double Rd;        // gas constant for dry air [J kg-1 K-1]
  double Rv;        // gas constant for moist air [J kg-1 K-1]
  double bolz;      // Bolzman constant [-]
  double rhow;      // density of water [kg m-3]
  double S0;        // solar constant [W m-2]
  double pi;

  // Aditions for A-Gs scheme
  double mco2;      // molecular weight CO2 [g mol -1]
  double mair;      // molecular weight air [g mol -1]
  double nuco2q;    // ratio molecular viscosity water to carbon dioxide
  double Cw;        // constant water stress correction (eq. 13 Jacobs et al. 2007) [-]
  double wmax;      // upper reference value soil water [-]
  double wmin;      // lower reference value soil water [-]
  double R10;       // respiration at 10 C [mg CO2 m-2 s-1]
  double E0;        // activation energy [53.3 kJ kmol-1]

  // Shallow-cumulus / variance calculations
  double dz;        // inversion-layer/transition-layer thickness [m]

  // time variables
  double runtime;   // duration of model run [s]
  double dt;        // time step [s]
  double sinperiod; // period for sinusoidal fluxes [s]
  int tsteps;       // total time steps [-]
  int t;            // current time step [-]

  // mixed-layer
  bool   sw_ml;     // mixed-layer model switch;
  bool   sw_wsft;   // compensate free tropospheric warming due to subsidence?
  double h;         // initial ABL height [m]
  double Ps;        // surface pressure [Pa]
  double omegas;    // large scale divergence [s-1]
  double ws;        // large scale vertical velocity [m s-1]
  double fc;        // coriolis parameter [s-1]
  double we;        // entrainment velocity [m s-1]
  double wf;        // mixed-layer growth due to dFz [m s-1]
  double wstar;     // Deardorff vertical velocity scale [m s-1]
  double lcl;       // lifted condensation level [m]
  double RH;        // Relative humidity at T=theta [-]
  double RHtop;     // Relative humidity at mixed-layer top [-]

  double theta;     // initial mixed-layer potential temperature [K]
  double dtheta;    // initial temperature jump at h [K]
  double gammatheta;// free atmosphere potential temperature lapse rate [K m-1]
  double advtheta;  // advection of heat [K s-1]
  double beta;      // entrainment ratio for virtual heat [-]
  double wtheta;    // surface kinematic heat flux [K m s-1]
  double wtheta0;   // maximum surface kinematic heat flux [K m s-1]
  double wthetae;   // entrainment kinematic heat flux [K m s-1]
  double wthetaM;   // mass-flux kinematic heat flux [K m s-1]
  double sigmatheta2; // mixed-layer top potential temperature variance [K2]
  bool   sw_wtheta; // switch for sinusoidal temperature flux

  double T2m;       // 2m temperature [K]
  double q2m;       // 2m specific humidity [kg kg-1]
  double e2m;       // 2m vapor pressure [Pa]
  double esat2m;    // 2m saturated vapor pressure [Pa]
  double u2m;       // 2m u-wind [m s-1]
  double v2m;       // 2m v-wind [m s-1]

  double thetasurf; // surface potential temperature [K]
  double thetav;    // initial mixed-layer potential temperature [K]
  double dthetav;   // initial virtual temperature jump at h [K]
  double thetavsurf;// surface virtual potential temperature [K]
  double qsurf;     // surface specific humidity [g kg-1]
  double wthetav;   // surface kinematic virtual heat flux [K m s-1]

  double q;         // initial mixed-layer specific humidity [kg kg-1]
  double dq;        // initial specific humidity jump at h [kg kg-1]
  double gammaq;    // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  double advq;      // advection of moisture [kg kg-1 s-1]
  double wq;        // surface kinematic moisture flux [kg kg-1 m s-1]
  double wq0;       // maximum surface kinematic moisture flux [kg kg-1 m s-1]
  double wqe;       // entrainment kinematic moisture flux [kg kg-1 m s-1]
  double wqM;       // mass-flux kinematic moisture flux [kg kg-1 m s-1]
  double sigmaq2;   // mixed-layer top specific humidity variance [kg2 kg-2]
  bool   sw_wq;     // switch for sinusoidal moisture flux

  double qsat;      // mixed-layer saturated specific humidity [kg kg-1]
  double esat;      // mixed-layer saturated vapor pressure [Pa]
  double e;         // mixed-layer vapor pressure [Pa]
  double qsatsurf;  // surface saturated specific humidity [g kg-1]
  double dqsatdT;   // slope saturated specific humidity curve [g kg-1 K-1]

  bool   sw_wind;   // prognostic wind switch
  double u;         // initial mixed-layer u-wind speed [m s-1]
  double du;        // initial u-wind jump at h [m s-1]
  double gammau;    // free atmosphere u-wind speed lapse rate [s-1]
  double advu;      // advection of u-wind [m s-2]
  double uw;        // surface momentum flux in u-direction [m2 s-2]
  double uwe;       // Entrainment momentum flux in u-direction [m2 s-2]

  double v;         // initial mixed-layer u-wind speed [m s-1]
  double dv;        // initial u-wind jump at h [m s-1]
  double gammav;    // free atmosphere v-wind speed lapse rate [s-1]
  double advv;      // advection of v-wind [m s-2]
  double vw;        // surface momentum flux in v-direction [m2 s-2]
  double vwe;       // Entrainment momentum flux in v-direction [m2 s-2]

  // BvS; a scalar, without the need for the chemistry scheme :)
  double sca;       // initial mixed-layer scalar [kg kg-1]
  double dsca;      // initial scalar jump at h [kg kg-1]
  double gammasca;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double advsca;    // advection of scalar [kg kg-1 s-1]
  double wsca;      // surface kinematic scalar flux [kg kg-1 m s-1]
  double wscae;     // entrainment kinematic scalar flux [kg kg-1 m s-1]
  double wscaM;     // mass-flux kinematic scalar flux [kg kg-1 m s-1]
  double sigmasca2; // mixed-layer top scalar variance [kg2 kg-2]

  double CO2;       // initial mixed-layer CO2 [ppm]
  double dCO2;      // initial CO2 jump at h [ppm]
  double gammaCO2;  // free atmosphere CO2 lapse rate [ppm]
  double advCO2;    // advection of CO2 [ppm]
  double wCO2;      // surface kinematic CO2 flux [ppm]
  double wCO2e;     // entrainment kinematic CO2 flux [ppm]
  double wCO2M;     // mass-flux kinematic CO2 flux [ppm]
  double sigmaCO22; // mixed-layer top CO2 variance [ppm2]

  // Other scalars/species (chemistry code..)
  int nsc;
  double *sc;       // initial mixed-layer scalar
  double *dsc;      // initial scalar jump at h [kg kg-1]
  double *gammasc;  // free atmosphere scalar lapse rate [kg kg-1 m-1]
  double *advsc;    // advection of scalar [kg kg-1 s-1]
  double *wsc;      // surface kinematic scalar flux [kg kg-1 m s-1]
  double *wscM;     // mass-flux kinematic scalar flux [kg kg-1 m s-1]
  double *sigmasc2; // mixed-layer top potential temperature variance [K2]
  int    *sw_wsc;   // switch for sinusoidal wsc
  double *wsc0;
  double *wsce;
  double *sctend, *dsctend;

  double htend;
  double thetatend, qtend, utend, vtend, scatend, CO2tend;
  double dthetatend, dqtend, dutend, dvtend, dscatend, dCO2tend;

  // surface layer variables
  bool   sw_sl;     // surface layer switch
  bool   sw_sea;    // land / sea switch
  double ustar;     // surface friction velocity [m s-1]

  double z0m;       // roughness length for momentum [m]
  double z0h;       // roughness length for scalars [m]
  double Cm;        // drag coefficient for momentum [-]
  double Cs;        // drag coefficient for scalars [-]
  double L;         // Obukhov length [-]
  double Rib;       // bulk Richardson number [-]
  double ra;        // aerodynamic resistance [s m-1]

  // radiation parameters
  bool   sw_rad;    // radiation switch
  double lat;       // latitude [deg]
  double lon;       // longitude [deg]
  double doy;       // day of the year [-]
  double tstart;    // time of the day [h UTC]
  double cc;        // cloud cover fraction [-]

  double Swin;      // incoming short wave radiation [W m-2]
  double Swout;     // outgoing short wave radiation [W m-2]
  double Lwin;      // incoming long wave radiation [W m-2]
  double Lwout;     // outgoing long wave radiation [W m-2]
  double Q;         // net radiation [W m-2]

  // land surface parameters
  bool   sw_ls;     // land surface switch
  bool   sw_jarvis; // Jarvis / A-Gs switch
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

  double C1sat;
  double C2ref;

  double LAI;       // leaf area index [-]
  double gD;        // correction factor transpiration for VPD [-]
  double rsmin;     // minimum resistance transpiration [s m-1]
  double rssoilmin; // minimum resistance soil evaporation [s m-1]
  double alpha;     // surface albedo [-]

  double rs;        // surface resistance [s m-1]
  double rssoil;    // soil resistance [s m-1]

  double Ts;        // initial surface temperature [K]

  double cveg;      // vegetation fraction [-]
  double Wmax;      // thickness of water layer on wet vegetation [m]
  double Wl;        // equivalent water layer depth for wet vegetation [m]
  double cliq;      // wet fraction [-]

  double Lambda;    // thermal diffusivity skin layer [-]

  double H;         // sensible heat flux [W m-2]
  double LE;        // evapotranspiration [W m-2]
  double LEliq;     // open water evaporation [W m-2]
  double LEveg;     // transpiration [W m-2]
  double LEsoil;    // soil evaporation [W m-2]
  double LEpot;     // potential evaporation [W m-2]
  double LEref;     // reference evaporation using rs = rsmin / LAI [W m-2]
  double G;         // ground heat flux [W m-2]

  double Tsoiltend;
  double wgtend;
  double Wltend;

  // initialize plant physilogical model (A-gs)
  double ci;        // CO2 concentration inside the leaf [mg m-3]
  double cfrac;     // CO2 concentration fraction [-]
  double Ds;        // vapor pressure deficit [kPa]
  double D0;        // vapor pressure deficit stomata closes [kPa]
  double gm;        // mesophyll conducatnce [mm s-1]
  double fmin;      // minimum value cfrac [-]
  double fmin0;     // function to calculate fmin [-]
  double Ammax;     // CO2 maximal primary productivity [mg m-2 s-1]
  double Am;        // CO2 primray productivity [mg m-2 s-1]
  double An;        // net CO2 flow into the plant [mg m-2 s-1]
  double Rdark;     // CO2 dark respiration [mg m-2 s-1]
  double PAR;       // Photosyntetically Active Radiation [W m-2]
  double gcCo2;     // CO2 conductance at canopy level [mm s-1]
  // BvS, calculate rs either with Jarvis or A-Gs...
  // double rsAgs;     // surface resistance moisture [s mm-1]
  double rsCO2;     // surface resistance carbon dioxide [s mm-1]
  double betaw;     // function depending on soil moisture content to calculate stress function [-]
  double fstr;      // stress function included in canopy conductance [-]

  // initialize constants depending C3 or C4 plants
  double CO2comp298;// CO2 compensation concentration [mg m-3]
  double Q10CO2;    // function parameter to calculate CO2 compensation concentration [-]
  double gm298;     // mesophyill conductance at 298 K [mm s-1]
  double Ammax298;  // CO2 maximal primary productivity [mg m-2 s-1]
  double Q10gm;     // function parameter to calculate mesophyll conductance [-]
  double T1gm;      // reference temperature to calculate mesophyll conductance gm [K]
  double T2gm;      // reference temperature to calculate mesophyll conductance gm [K]
  double Q10Am;     // function parameter to calculate maximal primary profuctivity Ammax
  double T1Am;      // reference temperature to calculate maximal primary profuctivity Ammax [K]
  double T2Am;      // reference temperature to calculate maximal primary profuctivity Ammax [K]
  double f0;        // maximum value Cfrac [-]
  double ad;        // regression coefficient to calculate Cfrac [kPa-1]
  double alpha0;    // initial low light conditions [mg J-1]
  double frveg;     // fraction of the shortwve radiation contributing to PAR [-]
  double Kx;        // extinction coefficient PAR [-]
  double gmin;      // cuticular (minimum) conductance [mm s-1]

  // initialize soil  -1. ration model (coupled to A-gs)
  double fw;        // water stress correction function [-]
  double Resp;      // soil surface carbon dioxide flux [mg m-2 s-1]

  // Shallow-cumulus parameters
  bool sw_cu;       // shallow-cumulus switch [-]
  double ac;        // cloud core fraction [-]
  double M;         // mass-flux (/rho) [m s-1]

  // Stratocumulus
  double dFz;       // cloud-top radiative divergence [W m-2]

  // chemistry
  modelchem *cm;
  bool   sw_chem;
  bool   sw_chem_constant;
  bool   sw_photo_constant;
  int    rsize;
  int    csize;
  bool   *sw_chemoutput;
  Reaction *reactions;
  bool     *sw_reactions;
  Name_Number *PL_scheme;
  double P_ref;
  double Tcbl_ref;
  double Tfc_ref;
  double qcbl_ref;
  double qfc_ref;
  double tod_ref;
  double stocoef;
  double phi;
  double k_r05;
};

