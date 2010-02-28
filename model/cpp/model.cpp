#import <cmath>
#import <iostream>
#import "modelinput.h"
#import "modeloutput.h"
#import "model.h"

model::model(modelinput input)
{
  // model constants
  Lv         =  2.5e6;                  // heat of vaporization [J kg-1]
  cp         =  1005.;                  // specific heat of dry air [J kg-1 K-1]
  rho        =  1.2;                    // density of air [kg m-3]
  k          =  0.4;                    // Von Karman constant [-]
  g          =  9.81;                   // gravity acceleration [m s-2]
  Rd         =  287.;                   // gas constant for dry air [J kg-1 K-1]
  Rv         =  461.5;                  // gas constant for moist air [J kg-1 K-1]
  bolz       =  5.67e-8;                // Bolzman constant [-]
  rhow       =  1000.;                  // density of water [kg m-3]
  S0         =  1368.;                  // solar constant [W m-2]
  
  // read initial and boundary conditions from input file
  runtime    =  input.runtime;          // duration of model run [s]
  dt         =  input.dt;               // time step [s]

  // read input for mixed-layer component
  h          =  input.h;                // initial ABL height [m]
  Ps         =  input.Ps;               // surface pressure [Pa]
  ws         =  input.ws;               // large scale vertical velocity [m s-1]
  fc         =  input.fc;               // coriolis parameter [s-1]
  
  theta      =  input.theta;            // initial mixed-layer potential temperature [K]
  dtheta     =  input.dtheta;           // initial temperature jump at h [K]
  gammatheta =  input.gammatheta;       // free atmosphere potential temperature lapse rate [K m-1]
  advtheta   =  input.advtheta;         // advection of heat [K s-1]
  beta       =  input.beta;             // entrainment ratio for virtual heat [-]
  wtheta     =  input.wtheta;           // surface kinematic heat flux [K m s-1]
  
  thetasurf  =  input.theta;            // surface potential temperature [K]
  
  q          =  input.q;                // initial mixed-layer specific humidity [kg kg-1]
  dq         =  input.dq;               // initial specific humidity jump at h [kg kg-1]
  gammaq     =  input.gammaq;           // free atmosphere specific humidity lapse rate [kg kg-1 m-1]
  advq       =  input.advq;             // advection of moisture [kg kg-1 s-1]
  wq         =  input.wq;               // surface kinematic moisture flux [kg kg-1 m s-1]
  
  sw_wind    =  input.sw_wind;          // prognostic wind switch
  u          =  input.u;                // initial mixed-layer u-wind speed [m s-1]
  du         =  input.du;               // initial u-wind jump at h [m s-1]
  gammau     =  input.gammau;           // free atmosphere u-wind speed lapse rate [s-1]
  advu       =  input.advu;             // advection of u-wind [m s-2]
  
  v          =  input.v;                // initial mixed-layer u-wind speed [m s-1]
  dv         =  input.dv;               // initial u-wind jump at h [m s-1]
  gammav     =  input.gammav;           // free atmosphere v-wind speed lapse rate [s-1]
  advv       =  input.advv;             // advection of v-wind [m s-2]

  // initialize time variables
  tsteps = int(runtime / dt);
  //t      = 0;
  // set output array to given value
  output = new modeloutput(tsteps);
} 

void model::runmodel()
{
  double htend, thetatend, qtend, utend, vtend;
  double dthetatend, dqtend, dutend, dvtend;
  double h0, theta0, q0, u0, v0;
  double dtheta0, dq0, du0, dv0;

  for(t = 0; t < tsteps; t++)
  {
    // run radiation model
    // if(sw_rad):
    //   runradmodel()

    // run surface layer model
    //if(sw_sl):
    //  runslmodel()
    //else:
    //  # decompose ustar along the wind components
    uw       = - pow((pow(ustar, 4.) / (pow(v, 2.) / pow(u, 2.) + 1.)), 0.5);
    vw       = - pow((pow(ustar, 4.) / (pow(u, 2.) / pow(v, 2.) + 1.)), 0.5);
    
    // run land surface model
    // if(sw_ls):
    //   runlsmodel()

    // compute mixed-layer tendencies
    // first compute necessary virtual temperature units
    thetav   = theta  + 0.61 * theta * q;
    wthetav  = wtheta + 0.61 * theta * wq;
    dthetav  = (theta + dtheta) * (1. + 0.61 * (q + dq)) - theta * (1. + 0.61 * q);
    
    // compute tendencies
    // we    = (beta * wthetav) / dthetav
    we     = (beta * wthetav + 5. * pow(ustar, 3.) * thetav / (g * h)) / dthetav;
    htend       = we + ws;
    
    thetatend   = (wtheta + we * dtheta) / h + advtheta;
    qtend       = (wq     + we * dq)     / h + advq;
    
    dthetatend  = gammatheta * we - thetatend;
    dqtend      = gammaq     * we - qtend;
   
    // assume u + du = ug, so ug - u = du
    if(sw_wind)
    {
      utend       = -fc * dv + (uw + we * du)  / h + advu;
      vtend       =  fc * du + (vw + we * dv)  / h + advv;

      dutend      = gammau * we - utend;
      dvtend      = gammav * we - vtend;
    }
    
    // set values previous time step
    h0      = h;
    
    theta0  = theta;
    dtheta0 = dtheta;
    q0      = q;
    dq0     = dq;
    
    u0      = u;
    du0     = du;
    v0      = v;
    dv0     = dv;

    // integrate mixed-layer equations
    h        = h0      + dt * htend;

    theta    = theta0  + dt * thetatend;
    dtheta   = dtheta0 + dt * dthetatend;
    q        = q0      + dt * qtend;
    dq       = dq0     + dt * dqtend;

    if(sw_wind)
    {
      u        = u0      + dt * utend;
      du       = du0     + dt * dutend;
      v        = v0      + dt * vtend;
      dv       = dv0     + dt * dvtend;
    }

    std::cout << t * dt << ", " << h << ", " << theta << ", " << q*1000. << std::endl;
  }

  return;
}

