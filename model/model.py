# Coupled land-atmosphere-cloud-chemistry model
# Chiel van Heerwaarden & Jordi Vila-Guerau de Arellano, 2009
"""
Open issues:

wind speed and coriolis force give highly accelerating wind
sensible heat flux always peaks too early in the morning

"""

import copy
import numpy

class model:
  def __init__(self, modelinput):
    # initialize the different components of the model
    self.input = copy.deepcopy(modelinput)

  def runmodel(self):
    # initialize model variables
    self.initmodel()

    # time integrate model 
    for self.t in range(self.tsteps):
      
      # time integrate components
      self.run()
      
      # store output for current time step
      self.store()

    # delete unnecessary variables from memory
    self.exitmodel()
    
  def initmodel(self):
    # assign variables from input data
    # initialize constants
    self.Lv         =  2.45e6                # heat of vaporization [J kg-1]
    self.cp         =  1005.                 # specific heat of dry air [J kg-1 K-1]
    self.rho        =  1.2                   # density of air [kg m-3]
    self.k          =  0.4                   # Von Karman constant [-]
    self.g          =  9.81                  # gravity acceleration [m s-2]
    self.Rd         =  287.                  # gas constant for dry air [J kg-1 K-1]
    self.Rv         =  461.5                 # gas constant for moist air [J kg-1 K-1]
    self.bolz       =  5.67e-8               # Bolzman constant [-]
    self.rhow       =  1000.                 # density of water [kg m-3]

    # initialize mixed-layer
    self.h          =  self.input.h          # initial ABL height [m]
    self.Ps         =  self.input.Ps         # surface pressure [Pa]
    self.ws         =  self.input.ws         # large scale vertical velocity [m s-1]
    self.fc         =  self.input.fc         # coriolis parameter [s-1]
    self.we         =  -1.                   # entrainment velocity [m s-1]
    
    self.theta      =  self.input.theta      # initial mixed-layer potential temperature [K]
    self.dtheta     =  self.input.dtheta     # initial temperature jump at h [K]
    self.gammatheta =  self.input.gammatheta # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   =  self.input.advtheta   # advection of heat [K s-1]
    self.beta       =  self.input.beta       # entrainment ratio for virtual heat [-]
    self.wtheta     =  self.input.wtheta     # surface kinematic heat flux [K m s-1]

    self.thetasurf  =  self.input.theta      # surface potential temperature [K]
    self.thetav     =  -1.                   # initial mixed-layer potential temperature [K]
    self.dthetav    =  -1.                   # initial virtual temperature jump at h [K]
    self.thetavsurf =  -1.                   # surface virtual potential temperature [K]
    self.qsurf      =  -1.                   # surface specific humidit [g kg-1]
    self.wthetav    =  -1.                   # surface kinematic virtual heat flux [K m s-1]
    
    self.q          =  self.input.q          # initial mixed-layer specific humidity [kg kg-1]
    self.dq         =  self.input.dq         # initial specific humidity jump at h [kg kg-1]
    self.gammaq     =  self.input.gammaq     # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    self.advq       =  self.input.advq       # advection of moisture [kg kg-1 s-1]
    self.wq         =  self.input.wq         # surface kinematic moisture flux [kg kg-1 m s-1]
    
    self.u          =  self.input.u          # initial mixed-layer u-wind speed [m s-1]
    self.du         =  self.input.du         # initial u-wind jump at h [m s-1]
    self.gammau     =  self.input.gammau     # free atmosphere u-wind speed lapse rate [s-1]
    self.advu       =  self.input.advu       # advection of u-wind [m s-2]
    
    self.v          =  self.input.v          # initial mixed-layer u-wind speed [m s-1]
    self.dv         =  self.input.dv         # initial u-wind jump at h [m s-1]
    self.gammav     =  self.input.gammav     # free atmosphere v-wind speed lapse rate [s-1]
    self.advv       =  self.input.advv       # advection of v-wind [m s-2]

    # initialize surface layer
    self.ustar      =  self.input.ustar      # surface friction velocity [m s-1]
    self.uw         =  -1.                   # surface momentum flux in u-direction [m2 s-2]
    self.vw         =  -1.                   # surface momentum flux in v-direction [m2 s-2]
    self.z0m        =  self.input.z0m        # roughness length for momentum [m]
    self.z0h        =  self.input.z0h        # roughness length for scalars [m]
    self.Cm         =  -1.                   # drag coefficient for momentum [-]
    self.Cs         =  -1.                   # drag coefficient for scalars [-]
    self.L          =  -1.                   # Obukhov length [m]
    self.Rib        =  -1.                   # bulk Richardson number [-]
    self.ra         =  -1.                   # aerodynamic resistance [s m-1]

    # initialize radiation
    self.lat        =  self.input.lat        # latitude [deg]
    self.lon        =  self.input.lon        # longitude [deg]
    self.doy        =  self.input.doy        # day of the year [-]
    self.tstart     =  self.input.tstart     # time of the day [-]
    self.S0         =  self.input.S0         # maximum incoming shortwave radiation [W m-2]
    self.Swin       =  -1.                   # incoming short wave radiation [W m-2]
    self.Swout      =  -1.                   # outgoing short wave radiation [W m-2]
    self.Lwin       =  -1.                   # incoming long wave radiation [W m-2]
    self.Lwout      =  -1.                   # outgoing long wave radiation [W m-2]
    self.Q          =  -1.                   # net radiation [W m-2]

    # initialize land surface
    self.wg         =  self.input.wg         # volumetric water content top soil layer [m3 m-3]
    self.w2         =  self.input.w2         # volumetric water content deeper soil layer [m3 m-3]
    self.Tsoil      =  self.input.Tsoil      # temperature top soil layer [K]
    self.T2         =  self.input.T2         # temperature deeper soil layer [K]
                       
    self.a          =  self.input.a          # Clapp and Hornberger retention curve parameter a [-]
    self.b          =  self.input.b          # Clapp and Hornberger retention curve parameter b [-]
    self.p          =  self.input.p          # Clapp and Hornberger retention curve parameter p [-]
    self.CGsat      =  self.input.CGsat      # saturated soil conductivity for heat
                       
    self.wsat       =  self.input.wsat       # saturated volumetric water content ECMWF config [-]
    self.wfc        =  self.input.wfc        # volumetric water content field capacity [-]
    self.wwilt      =  self.input.wwilt      # volumetric water content wilting point [-]
                       
    self.C1sat      =  self.input.C1sat      
    self.C2ref      =  self.input.C2ref      
    
    self.LAI        =  self.input.LAI        # leaf area index [-]
    self.rsmin      =  self.input.rsmin      # minimum resistance transpiration [s m-1]
    self.rssoilmin  =  self.input.rssoilmin  # minimum resistance soil evaporation [s m-1]
    self.alpha      =  self.input.alpha      # surface albedo [-]
                       
    self.Ts         =  self.input.Ts         # surface temperature [K]
                       
    self.cveg       =  self.input.cveg       # vegetation fraction [-]
    self.Wmax       =  self.input.Wmax       # thickness of water layer on wet vegetation [m]
    self.Wl         =  self.input.Wl         # equivalent water layer depth for wet vegetation [m]
    self.cl         =  self.input.cl         # wet fraction [-]
                       
    self.Lambda     =  self.input.Lambda     # thermal diffusivity skin layer [-]

    self.H          =  -1.                   # sensible heat flux [W m-2]
    self.LE         =  -1.                   # evapotranspiration [W m-2]
    self.LEliq      =  -1.                   # open water evaporation [W m-2]
    self.LEveg      =  -1.                   # transpiration [W m-2]
    self.LEsoil     =  -1.                   # soil evaporation [W m-2]
    self.LEpot      =  -1.                   # potential evaporation [W m-2]
    self.G          =  -1.                   # ground heat flux [W m-2]


    # initialize time variables
    self.tsteps = int(numpy.floor(self.input.runtime / self.input.dt))
    self.dt     = self.input.dt
    self.t      = 0

    # initialize output
    self.out = modeloutput(self.tsteps)

    # calculate initial diagnostic variables
    self.runradmodel()

    self.runslmodel()

    self.runlsmodel()

    # store initial values in output
    self.store()


  def run(self):
    # run radiation model
    self.runradmodel()

    # run surface layer model
    self.runslmodel()
    
    # run land surface model
    self.runlsmodel()

    # compute mixed-layer tendencies
    # first compute necessary virtual temperature units
    self.thetav   = self.theta  + 0.61 * self.theta * self.q
    self.wthetav  = self.wtheta + 0.61 * self.theta * self.wq
    self.dthetav  = (self.theta + self.dtheta) * (1. + 0.61 * (self.q + self.dq)) - self.theta * (1. + 0.61 * self.q)

    # decompose ustar along the wind components
    self.uw       = - (self.ustar ** 4. / (self.v ** 2. / self.u ** 2. + 1.)) ** (0.5)
    self.vw       = - (self.ustar ** 4. / (self.u ** 2. / self.v ** 2. + 1.)) ** (0.5)
    
    # compute tendencies
    self.we     = (self.beta * self.wthetav) / self.dthetav
    #we         = (beta * wthetav + 5. * ustar ** 3. * thetav / (g * h)) / dthetav
    htend       = self.we + self.ws
    
    thetatend   = (self.wtheta + self.we * self.dtheta) / self.h + self.advtheta 
    qtend       = (self.wq     + self.we * self.dq)     / self.h + self.advq
    
    dthetatend  = self.gammatheta * self.we - thetatend
    dqtend      = self.gammaq     * self.we - qtend
   
    # assume u + du = ug, so ug - u = du
    utend       =  self.fc * self.dv + (self.uw     + self.we * self.du)  / self.h + self.advu
    vtend       = -self.fc * self.du + (self.vw     + self.we * self.dv)  / self.h + self.advv

    dutend      = self.gammau * self.we - utend
    dvtend      = self.gammav * self.we - vtend
    
    # set values previous time step
    h0      = self.h
    
    theta0  = self.theta
    dtheta0 = self.dtheta
    q0      = self.q
    dq0     = self.dq
    
    u0      = self.u
    du0     = self.du
    v0      = self.v
    dv0     = self.dv

    # integrate mixed-layer equations
    self.h        = h0      + self.dt * htend

    self.theta    = theta0  + self.dt * thetatend
    self.dtheta   = dtheta0 + self.dt * dthetatend
    self.q        = q0      + self.dt * qtend
    self.dq       = dq0     + self.dt * dqtend

    self.u        = u0      + self.dt * utend
    self.du       = du0     + self.dt * dutend
    self.v        = v0      + self.dt * vtend
    self.dv       = dv0     + self.dt * dvtend

  def runradmodel(self):
    sda = 0.409 * numpy.cos(2. * numpy.pi * (self.doy - 173.) / 365.)
    lea = numpy.sin(2. * numpy.pi * self.lat / 360.) * numpy.sin(sda) - numpy.cos(2. * numpy.pi * self.lat / 360.) * numpy.cos(sda) * numpy.cos(2. * numpy.pi * (self.t * self.dt + self.tstart * 3600.) / 86400. - 2. * numpy.pi * self.lon / 360.)
    lea = max(lea, 0.0001)
    
    Ta  = self.theta * ((self.Ps - 0.1 * self.h * self.rho * self.g) / self.Ps ) ** (self.Rd / self.cp)
    
    self.Swin  = self.S0 * numpy.sin(lea)
    self.Swout = self.alpha * self.S0 * numpy.sin(lea)
    self.Lwin  = 0.8 * self.bolz * Ta ** 4.
    self.Lwout = self.bolz * self.Ts ** 4.
      
    self.Q     = self.Swin - self.Swout + self.Lwin - self.Lwout

  def runslmodel(self):

    esatsurf  = 0.611e3 * numpy.exp(17.2694 * (self.thetasurf - 273.16) / (self.thetasurf - 35.86))
    self.qsurf  = 0.622 * esatsurf / self.Ps

    self.thetavsurf = self.thetasurf * (1. + 0.61 * self.qsurf)
    self.thetav     = self.theta * (1. + 0.61 * self.q)
    self.wthetav    = self.wtheta + 0.61 * self.theta * self.wq

    zsl  = 0.1 * self.h
          
    self.Rib  = self.g / self.thetav * 0.1 * self.h * (self.thetav - self.thetavsurf) / (self.u ** 2. + self.v ** 2.)

    if(self.Rib > 0.):
      L    = 1.
      L0   = 2.
    else:
      L  = -1.
      L0 = -2.
    
    count = 0
    while (abs(L - L0) > 0.001):
      count = count + 1
      L0      = L
      fx      = self.Rib - zsl / L * (numpy.log(zsl / self.z0h) - self.psih(zsl / L) + self.psih(self.z0h / L)) / (numpy.log(zsl / self.z0m) - self.psim(zsl / L) + self.psim(self.z0m / L)) ** 2.
      Lstart  = L - 0.001*L
      Lend    = L + 0.001*L
      fxdif   = ( (- zsl / Lstart * (numpy.log(zsl / self.z0h) - self.psih(zsl / Lstart) + self.psih(self.z0h / Lstart)) / (numpy.log(zsl / self.z0m) - self.psim(zsl / Lstart) + self.psim(self.z0m / Lstart)) ** 2.) \
          - (-zsl / Lend * (numpy.log(zsl / self.z0h) - self.psih(zsl / Lend) + self.psih(self.z0h / Lend)) / (numpy.log(zsl / self.z0m) - self.psim(zsl / Lend) + self.psim(self.z0m / Lend)) ** 2.) ) / (Lstart - Lend)
      L       = L - fx / fxdif

    self.L    = L
    self.Cm   = self.k ** 2. / (numpy.log(zsl / self.z0m) - self.psim(zsl / self.L) + self.psim(self.z0m / self.L)) ** 2.
    self.Cs   = self.k ** 2. / (numpy.log(zsl / self.z0m) - self.psim(zsl / self.L) + self.psim(self.z0m / self.L)) / (numpy.log(zsl / self.z0h) - self.psih(zsl / self.L) + self.psih(self.z0h / self.L))
      
    #if(wthetav > 0.):
    #  wstar     = (g / thetav * h * wthetav) ** (1./3.)
    #else:
    #  wstar     = 0.0001
    #ueff      = numpy.sqrt(u ** 2. + wstar ** 2.)
    
    ueff       = numpy.sqrt(self.u ** 2. + self.v ** 2.)
    self.ustar = numpy.sqrt(self.Cm) * ueff

    self.ra    = (self.Cm * ueff) ** (-1.)
    
    self.thetasurf = self.theta + self.wtheta / (self.Cs * ueff)

  def psim(self, zeta):
    if(zeta <= 0):
      #x     = (1. - 16. * zeta) ** (0.25)
      #psim  = 3.14159265 / 2. - 2. * numpy.arctan(x) + numpy.log( (1.+x) ** 2. * (1. + x ** 2.) / 8.)
      x     = (1. + 3.6 * abs(zeta) ** (2./3.)) ** (-0.5)
      psim = 3. * numpy.log( (1. + 1. / x) / 2.)
    else:
      psim  = -2./3. * (zeta - 5./0.35) * numpy.exp(-0.35 * zeta) - zeta - (10./3.) / 0.35
    return psim
    
  def psih(self, zeta):
    if(zeta <= 0):
      #x     = (1. - 16. * zeta) ** (0.25)
      #psih  = 2. * numpy.log( (1. + x ** 2.) / 2. )
      x     = (1. + 7.9 * abs(zeta) ** (2./3.)) ** (-0.5)
      psih  = 3. * numpy.log( (1. + 1. / x) / 2.)
    else:
      psih  = -2./3. * (zeta - 5./0.35) * numpy.exp(-0.35 * zeta) - (1. + (2./3.) * zeta) ** (1.5) - (10./3.) / 0.35 + 1.
    return psih

  def runlsmodel(self):
    # first calculate essential thermodynamic variables
    self.esat  = 0.611e3 * numpy.exp(17.2694 * (self.theta - 273.16) / (self.theta - 35.86))
    self.qsat  = 0.622 * self.esat / self.Ps
    desatdT  = self.esat * (17.2694 / (self.theta - 35.86) - 17.2694 * (self.theta - 273.16) / (self.theta - 35.86)**2.)
    self.dqsatdT  = 0.622 * desatdT / self.Ps
    e = self.q * self.Ps / 0.622

    # calculate surface resistances using Jarvis-Stewart model
    f1          = 1. / ((0.004 * self.Swin + 0.05) / (0.81 * (0.004 * self.Swin + 1.)))
    if(self.w2 > self.wwilt and self.w2 <= self.wfc):
      f2          = (self.wfc - self.wwilt) / (self.w2 - self.wwilt)
    else:
      f2        = 1.e8
    f3          = 1.
    self.rs     = self.rsmin / self.LAI * f1 * f2 * f3
    self.rssoil = self.rssoilmin * f2 


    self.Wlmx = self.LAI * self.Wmax
    self.cliq = min(1., self.Wl / self.Wlmx) 
   
    # calculate skin temperature implictly
    self.Ts   = (self.Q  + self.rho * self.cp / self.ra * self.theta \
        + self.cveg * (1. - self.cliq) * self.rho * self.Lv / (self.ra + self.rs) * (self.dqsatdT * self.theta - self.qsat + self.q) \
        + (1. - self.cveg) * self.rho * self.Lv / (self.ra + self.rssoil) * (self.dqsatdT * self.theta - self.qsat + self.q) \
        + self.cveg * self.cliq * self.rho * self.Lv / self.ra * (self.dqsatdT * self.theta - self.qsat + self.q) + self.Lambda * self.Tsoil) \
      * (self.rho * self.cp / self.ra + self.cveg * (1. - self.cliq) * self.rho * self.Lv / (self.ra + self.rs) * self.dqsatdT + (1. - self.cveg) * self.rho * self.Lv / (self.ra + self.rssoil) * self.dqsatdT + self.cveg * self.cliq * self.rho * self.Lv / self.ra * self.dqsatdT + self.Lambda) ** (-1.)

    esatsurf  = 0.611e3 * numpy.exp(17.2694 * (self.Ts - 273.16) / (self.Ts - 35.86))
    self.qsatsurf  = 0.622 * esatsurf / self.Ps
    
    self.LEveg  = (1. - self.cliq) * self.cveg * self.rho * self.Lv / (self.ra + self.rs) * (self.dqsatdT * (self.Ts - self.theta) + self.qsat - self.q)
    self.LEliq  = self.cliq * self.cveg * self.rho * self.Lv / self.ra * (self.dqsatdT * (self.Ts - self.theta) + self.qsat - self.q)
    self.LEsoil = (1. - self.cveg) * self.rho * self.Lv / (self.ra + self.rssoil) * (self.dqsatdT * (self.Ts - self.theta) + self.qsat - self.q)
    self.LEpot  = self.rho * self.Lv / self.ra * (self.dqsatdT * (self.Ts - self.theta) + self.qsat - self.q)

    Wltend      = - self.LEliq / (self.rhow * self.Lv)

    self.LE     = self.LEsoil + self.LEveg + self.LEliq
    self.H      = self.rho * self.cp / self.ra * (self.Ts - self.theta)
    self.G      = self.Lambda * (self.Ts - self.Tsoil)
    
    CG          = self.CGsat * (self.wsat / self.w2) ** (self.b / (2. * numpy.log(10.)))

    Tsoiltend   = CG * self.G - 2. * numpy.pi / 86400. * (self.Tsoil - self.T2)
 
    d1          = 0.1
    C1          = self.C1sat * (self.wsat / self.wg) ** (self.b / 2. + 1.)
    C2          = self.C2ref * (self.w2 / (self.wsat - self.w2) )
    wgeq        = self.w2 - self.wsat * self.a * ( (self.w2 / self.wsat) ** self.p * (1. - (self.w2 / self.wsat) ** (8. * self.p)) )
    wgtend      = - C1 / (self.rhow * d1) * self.LEsoil / self.Lv - C2 / 86400. * (self.wg - wgeq)

    # calculate kinematic heat fluxes
    self.wtheta   = self.H / (self.rho * self.cp)
    self.wq       = self.LE / (self.rho * self.Lv)

    # integrate soil equations
    Tsoil0        = self.Tsoil
    wg0           = self.wg
    Wl0           = self.Wl

    self.Tsoil    = Tsoil0  + self.dt * Tsoiltend
    self.wg       = wg0     + self.dt * wgtend
    self.Wl       = Wl0     + self.dt * Wltend
 

  # store model output
  def store(self):
    t                      = self.t
    self.out.t[t]          = t * self.dt / 3600. + self.tstart

    self.out.h[t]          = self.h
    self.out.ws[t]         = self.ws
    
    self.out.theta[t]      = self.theta
    self.out.thetav[t]     = self.thetav
    self.out.dtheta[t]     = self.dtheta
    self.out.dthetav[t]    = self.dthetav
    self.out.gammatheta    = self.gammatheta
    self.out.advtheta[t]   = self.advtheta
    self.out.beta[t]       = self.beta
    self.out.wtheta[t]     = self.wtheta
    self.out.wthetav[t]    = self.wthetav
    
    self.out.q[t]          = self.q
    self.out.dq[t]         = self.dq
    self.out.gammaq[t]     = self.gammaq
    self.out.advq[t]       = self.advq
    self.out.wq[t]         = self.wq
    
    self.out.u[t]          = self.u
    self.out.du[t]         = self.du
    self.out.gammau[t]     = self.gammau
    self.out.advu[t]       = self.advu
    
    self.out.v[t]          = self.v
    self.out.dv[t]         = self.dv
    self.out.gammav[t]     = self.gammav
    self.out.advv[t]       = self.advv
    
    self.out.thetasurf[t]  = self.thetasurf
    self.out.thetavsurf[t] = self.thetavsurf
    self.out.qsurf[t]      = self.qsurf
    self.out.ustar[t]      = self.ustar
    self.out.Cm[t]         = self.Cm
    self.out.Cs[t]         = self.Cs
    self.out.L[t]          = self.L
    self.out.Rib[t]        = self.Rib

    self.out.Swin[t]       = self.Swin
    self.out.Swout[t]      = self.Swout
    self.out.Lwin[t]       = self.Lwin
    self.out.Lwout[t]      = self.Lwout
    self.out.Q[t]          = self.Q

    self.out.H[t]          = self.H
    self.out.LE[t]         = self.LE
    self.out.LEliq[t]      = self.LEliq
    self.out.LEveg[t]      = self.LEveg
    self.out.LEsoil[t]     = self.LEsoil
    self.out.LEpot[t]      = self.LEpot
    self.out.G[t]          = self.G


  # delete class variables to facilitate analysis in ipython
  def exitmodel(self):
    del(self.Lv)
    del(self.cp)
    del(self.rho)
    del(self.k)
    del(self.g)
    del(self.Rd)
    del(self.Rv)
    del(self.bolz)

    del(self.t)
    del(self.dt)
    del(self.tsteps)
     
    del(self.h)          
    del(self.Ps)        
    del(self.ws)
    del(self.we)
    
    del(self.theta)
    del(self.dtheta)
    del(self.gammatheta)
    del(self.advtheta)
    del(self.beta)
    del(self.wtheta)
    
    del(self.thetasurf)
    del(self.thetav)
    del(self.dthetav)
    del(self.thetavsurf)
    del(self.qsurf)
    del(self.wthetav)
    
    del(self.q)
    del(self.dq)
    del(self.gammaq)
    del(self.advq)
    del(self.wq)
    
    del(self.u)
    del(self.du)
    del(self.gammau)
    del(self.advu)
    
    del(self.v)
    del(self.dv)
    del(self.gammav)
    del(self.advv)
    
    del(self.ustar)
    del(self.uw)
    del(self.vw)
    del(self.z0m)
    del(self.z0h)        
    del(self.Cm)         
    del(self.Cs)
    del(self.L)
    del(self.Rib)

    del(self.lat)
    del(self.lon)
    del(self.doy)
    del(self.tstart)
    del(self.S0)
 
    del(self.Swin)
    del(self.Swout)
    del(self.Lwin)
    del(self.Lwout)

    del(self.wg)
    del(self.w2)
    del(self.cveg)
    del(self.Tsoil)
    del(self.T2)
    del(self.a)
    del(self.b)
    del(self.p)
    del(self.CGsat)

    del(self.wsat)
    del(self.wfc)
    del(self.wwilt)

    del(self.C1sat)
    del(self.C2ref)

    del(self.LAI)
    del(self.rsmin)
    del(self.rssoilmin)
    del(self.alpha)

    del(self.Ts)

    del(self.Wmax)
    del(self.Wl)
    del(self.cl)

    del(self.Lambda)
    
    del(self.Q)
    del(self.H)
    del(self.LE)
    del(self.LEliq)
    del(self.LEveg)
    del(self.LEsoil)
    del(self.LEpot)
    del(self.G)


# class for storing mixed-layer model output data
class modeloutput:
  def __init__(self, tsteps):
    self.t          = numpy.zeros(tsteps)    # time [s]

    # mixed-layer variables
    self.h          = numpy.zeros(tsteps)    # initial ABL height [m]
    self.Ps         = numpy.zeros(tsteps)    # surface pressure [Pa]
    self.ws         = numpy.zeros(tsteps)    # large scale vertical velocity [m s-1]
    
    self.theta      = numpy.zeros(tsteps)    # initial mixed-layer potential temperature [K]
    self.thetav     = numpy.zeros(tsteps)    # initial mixed-layer virtual potential temperature [K]
    self.dtheta     = numpy.zeros(tsteps)    # initial potential temperature jump at h [K]
    self.dthetav    = numpy.zeros(tsteps)    # initial virtual potential temperature jump at h [K]
    self.gammatheta = numpy.zeros(tsteps)    # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   = numpy.zeros(tsteps)    # advection of heat [K s-1]
    self.beta       = numpy.zeros(tsteps)    # entrainment ratio for virtual heat [-]
    self.wtheta     = numpy.zeros(tsteps)    # surface kinematic heat flux [K m s-1]
    self.wthetav    = numpy.zeros(tsteps)    # surface kinematic virtual heat flux [K m s-1]
    
    self.q          = numpy.zeros(tsteps)    # initial mixed-layer specific humidity [kg kg-1]
    self.dq         = numpy.zeros(tsteps)    # initial specific humidity jump at h [kg kg-1]
    self.gammaq     = numpy.zeros(tsteps)    # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    self.advq       = numpy.zeros(tsteps)    # advection of moisture [kg kg-1 s-1]
    self.wq         = numpy.zeros(tsteps)    # surface kinematic moisture flux [kg kg-1 m s-1]
    
    self.u          = numpy.zeros(tsteps)    # initial mixed-layer u-wind speed [m s-1]
    self.du         = numpy.zeros(tsteps)    # initial u-wind jump at h [m s-1]
    self.gammau     = numpy.zeros(tsteps)    # free atmosphere u-wind speed lapse rate [s-1]
    self.advu       = numpy.zeros(tsteps)    # advection of u-wind [m s-2]
    
    self.v          = numpy.zeros(tsteps)    # initial mixed-layer u-wind speed [m s-1]
    self.dv         = numpy.zeros(tsteps)    # initial u-wind jump at h [m s-1]
    self.gammav     = numpy.zeros(tsteps)    # free atmosphere v-wind speed lapse rate [s-1]
    self.advv       = numpy.zeros(tsteps)    # advection of v-wind [m s-2]
   
    # surface-layer variables
    self.thetasurf  = numpy.zeros(tsteps)    # surface potential temperature [K]
    self.thetavsurf = numpy.zeros(tsteps)    # surface virtual potential temperature [K]
    self.qsurf      = numpy.zeros(tsteps)    # surface specific humidity [kg kg-1]
    self.ustar      = numpy.zeros(tsteps)    # surface friction velocity [m s-1]
    self.z0m        = numpy.zeros(tsteps)    # roughness length for momentum [m]
    self.z0h        = numpy.zeros(tsteps)    # roughness length for scalars [m]
    self.Cm         = numpy.zeros(tsteps)    # drag coefficient for momentum []
    self.Cs         = numpy.zeros(tsteps)    # drag coefficient for scalars []
    self.L          = numpy.zeros(tsteps)    # Obukhov length [m]
    self.Rib        = numpy.zeros(tsteps)    # bulk Richardson number [-]

    # radiation variables
    self.Swin       = numpy.zeros(tsteps)    # incoming short wave radiation [W m-2]
    self.Swout      = numpy.zeros(tsteps)    # outgoing short wave radiation [W m-2]
    self.Lwin       = numpy.zeros(tsteps)    # incoming long wave radiation [W m-2]
    self.Lwout      = numpy.zeros(tsteps)    # outgoing long wave radiation [W m-2]
    self.Q          = numpy.zeros(tsteps)    # net radiation [W m-2]

    # land surface variables
    self.H          = numpy.zeros(tsteps)    # sensible heat flux [W m-2]
    self.LE         = numpy.zeros(tsteps)    # evapotranspiration [W m-2]
    self.LEliq      = numpy.zeros(tsteps)    # open water evaporation [W m-2]
    self.LEveg      = numpy.zeros(tsteps)    # transpiration [W m-2]
    self.LEsoil     = numpy.zeros(tsteps)    # soil evaporation [W m-2]
    self.LEpot      = numpy.zeros(tsteps)    # potential evaporation [W m-2]
    self.G          = numpy.zeros(tsteps)    # ground heat flux [W m-2]


# class for storing mixed-layer model input data
class modelinput:
  def __init__(self):
    # general model variables
    self.runtime    = -1. # duration of model run [s]
    self.dt         = -1. # time step [s]

    # mixed-layer variables
    self.h          = -1. # initial ABL height [m]
    self.Ps         = -1. # surface pressure [Pa]
    self.ws         = -1. # large scale vertical velocity [m s-1]
    self.fc         = -1. # Coriolis parameter [s-1]
    
    self.theta      = -1. # initial mixed-layer potential temperature [K]
    self.dtheta     = -1. # initial temperature jump at h [K]
    self.gammatheta = -1. # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   = -1. # advection of heat [K s-1]
    self.beta       = -1. # entrainment ratio for virtual heat [-]
    self.wtheta     = -1. # surface kinematic heat flux [K m s-1]
    
    self.q          = -1. # initial mixed-layer specific humidity [kg kg-1]
    self.dq         = -1. # initial specific humidity jump at h [kg kg-1]
    self.gammaq     = -1. # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    self.advq       = -1. # advection of moisture [kg kg-1 s-1]
    self.wq         = -1. # surface kinematic moisture flux [kg kg-1 m s-1]
    
    self.u          = -1. # initial mixed-layer u-wind speed [m s-1]
    self.du         = -1. # initial u-wind jump at h [m s-1]
    self.gammau     = -1. # free atmosphere u-wind speed lapse rate [s-1]
    self.advu       = -1. # advection of u-wind [m s-2]

    self.v          = -1. # initial mixed-layer u-wind speed [m s-1]
    self.dv         = -1. # initial u-wind jump at h [m s-1]
    self.gammav     = -1. # free atmosphere v-wind speed lapse rate [s-1]
    self.advv       = -1. # advection of v-wind [m s-2]

    # surface layer variables
    self.ustar      = -1. # surface friction velocity [m s-1]
    self.z0m        = -1. # roughness length for momentum [m]
    self.z0h        = -1. # roughness length for scalars [m]
    self.Cm         = -1. # drag coefficient for momentum [-]
    self.Cs         = -1. # drag coefficient for scalars [-]
    self.L          = -1. # Obukhov length [-]
    self.Rib        = -1. # bulk Richardson number [-]

    # radiation parameters
    self.lat        = -1. # latitude [deg]
    self.lon        = -1. # longitude [deg]
    self.doy        = -1. # day of the year [-]
    self.S0         = -1. # maximum incoming shortwave radiation [W m-2]
    self.tstart     = -1  # time of the day [h UTC]

    # land surface parameters
    self.wg         = -1. # volumetric water content top soil layer [m3 m-3]
    self.w2         = -1. # volumetric water content deeper soil layer [m3 m-3]
    self.Tsoil      = -1. # temperature top soil layer [K]
    self.T2         = -1. # temperature deeper soil layer [K]
    
    self.a          = -1. # Clapp and Hornberger retention curve parameter a
    self.b          = -1. # Clapp and Hornberger retention curve parameter b
    self.p          = -1. # Clapp and Hornberger retention curve parameter p 
    self.CGsat      = -1. # saturated soil conductivity for heat
    
    self.wsat       = -1. # saturated volumetric water content ECMWF config [-]
    self.wfc        = -1. # volumetric water content field capacity [-]
    self.wwilt      = -1. # volumetric water content wilting point [-]
    
    self.C1sat      = -1. 
    self.C2ref      = -1.
    
    self.LAI        = -1. # leaf area index [-]
    self.rsmin      = -1. # minimum resistance transpiration [s m-1]
    self.rssoilmin  = -1. # minimum resistance soil evaporation [s m-1]
    self.alpha      = -1. # surface albedo [-]
    
    self.Ts         = -1. # initial mixed layer potential temperature [K]
    
    self.cveg       = -1. # vegetation fraction [-]
    self.Wmax       = -1. # thickness of water layer on wet vegetation [m]
    self.Wl         = -1. # equivalent water layer depth for wet vegetation [m]
    self.cl         = -1. # wet fraction [-]
    
    self.Lambda     = -1. # thermal diffusivity soil [-]


