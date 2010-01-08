# !/usr/bin/python
# Mixed-layer model with interactive land surface, CO2, chemistry and clouds
# Chiel van Heerwaarden & Jordi Vila, 2009

class mlmodel():
  def psim(zeta):
    if(zeta <= 0):
      #x     = (1. - 16. * zeta) ** (0.25)
      #psim  = 3.14159265 / 2. - 2. * numpy.arctan(x) + numpy.log( (1.+x) ** 2. * (1. + x ** 2.) / 8.)
      x     = (1. + 3.6 * abs(zeta) ** (2./3.)) ** (-0.5)
      psim = 3. * numpy.log( (1. + 1. / x) / 2.)
    else:
      psim  = -2./3. * (zeta - 5./0.35)*exp(-0.35 * zeta) - zeta - (10./3.) / 0.35
    return psim
    
  def psih(zeta):
    if(zeta <= 0):
      #x     = (1. - 16. * zeta) ** (0.25)
      #psih  = 2. * numpy.log( (1. + x ** 2.) / 2. )
      x     = (1. + 7.9 * abs(zeta) ** (2./3.)) ** (-0.5)
      psih  = 3. * numpy.log( (1. + 1. / x) / 2.)
    else:
      psih  = -2./3. * (zeta - 5./0.35)*exp(-0.35 * zeta) - (1. + (2./3.) * zeta) ** (1.5) - (10./3.) / 0.35 + 1.
    return psih


  def getQ(lat, lon, doy, t):
    # compute net radiation
    sda    = 0.409 * numpy.cos(2 * numpy.pi * (doy - 173.) / 365.)
    lea    = numpy.sin(2. * numpy.pi * lat / 360.) * numpy.sin(sda) - numpy.cos(2. * numpy.pi * lat / 360.) * numpy.cos(sda) * numpy.cos(2. * numpy.pi * (t * dt + tstart * 3600.) / 86400. - 2. * numpy.pi * lon / 360.)
    lea    = max(lea, 1.e-6)
  
    Ta     = theta * ((P0 - 0.1 * h * rho * g) / P0 ) ** (Rd / cp)
  
    Swin   = S0 * numpy.sin(lea)
    Swout  = alpha * S0 * numpy.sin(lea)
    Lwin   = 0.8 * bolz * Ta ** 4.
    Lwout  = bolz * Ts ** 4.
    
    Q      = Swin - Swout + Lwin - Lwout
    return
  
  #def getrs():
  
  #def getra():
  
  #def psim():
  
  #def psih():
  
  # initialize mixed-layer model
  def __init__(self):
    self.input = mlmodelinput()
  
  def run(self):
    # model core

    # initialize model

    # universal constants
    Lv         = 2.45e6    # heat of vaporization [J kg-1]
    cp         = 1005.     # specific heat of dry air [J kg-1 K-1]
    rho        = 1.2       # density of air [kg m-3]
    k          = 0.4       # Von Karman constant [-]
    g          = 9.81      # gravity acceleration [m s-2]
    Rd         = 287.      # gas constant for dry air [J kg-1 K-1]
    Rv         = 461.5     # gas constant for moist air [J kg-1 K-1]
    bolz       = 5.67e-8   # Bolzman constant [-]

    # read input
    # time variables
    dt         = self.input.dt      # time step [s]
    time       = self.input.time    # total time [s]
    tstart     = self.input.tstart  # start of model [h UTC]

    # mixed-layer variables
    h          = self.input.h       # initial ABL height [m]
    ws         = self.input.ws      # large scale vertical velocity [m s-1]
    
    theta      = self.input.theta   # initial mixed-layer potential temperature [K]
    dtheta     = self.input.dtheta  # initial temperature jump at h [K]
    gammatheta = self.input.gammatheta   # free atmosphere potential temperature lapse rate [K m-1]
    advtheta   = self.input.advtheta     # advection of heat [K s-1]
    beta       = self.input.beta         # entrainment ratio for virtual heat [-]
    
    q          = self.input.q       # initial mixed-layer specific humidity [kg kg-1]
    dq         = self.input.dq      # initial specific humidity jump at h [kg kg-1]
    gammaq     = self.input.gammaq  # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    advq       = self.input.advq    # advection of moisture [kg kg-1 s-1]
    
    u          = self.input.u       # initial mixed-layer u-wind speed [m s-1]
    du         = self.input.du      # initial u-wind jump at h [m s-1]
    gammau     = self.input.gammau  # free atmosphere u-wind speed lapse rate [s-1]
    advu       = self.input.advu    # advection of u-wind [m s-2]

    v          = self.input.v       # initial mixed-layer u-wind speed [m s-1]
    dv         = self.input.dv      # initial u-wind jump at h [m s-1]
    gammav     = self.input.gammav  # free atmosphere v-wind speed lapse rate [s-1]
    advv       = self.input.advv    # advection of v-wind [m s-2]
    
    wtheta     = self.input.wtheta  # surface kinematic heat flux [K m s-1]
    wq         = self.input.wq      # surface kinematic moisture flux [kg kg-1 m s-1]
    
    # mixed-layer switches
    sw_mlwind    = self.input.sw_mlwind    # switch to enable mixed-layer wind 
    sw_entrshear = self.input.sw_entrshear # switch to enable shear effects on entrainment
    
    # initialize computed variables
    tsteps     = time / dt
  
    for t in range(tsteps):

      # set values previous time step
      theta0  = theta
      h0      = h
      dtheta0 = dtheta
      q0      = q
      dq0     = dq
      u0      = u
      du0     = du
            
      # compute mixed-layer tendencies
      
      # compute necessary virtual temperature units
      thetav     = theta  + 0.61 * theta * q
      wthetav    = wtheta + 0.61 * theta * wq
      dthetav    = (theta + dtheta) * (1. + 0.61 * (q + dq)) - theta * (1. + 0.61 * q)
      
      # compute tendencies
      we         = (beta * wthetav) / dthetav
      #we         = (beta * wthetav + 5. * ustar ** 3. * thetav / (g * h)) / dthetav
      htend      = we + ws
      
      thetatend  = (wtheta + we * dtheta) / h + advtheta 
      qtend      = (wq     + we * dq)     / h + advq
      
      dthetatend = gammatheta * we - thetatend
      dqtend     = gammaq     * we - qtend
      
      if(sw_mlwind):
        utend      = (uw ** 2. + we * du)   / h + advu
        vtend      = (vw ** 2. + we * dv)   / h + advv
        dutend     = gammau     * we - utend
        dvtend     = gammav     * we - vtend
      
      # integrate mixed-layer equations
      h          = h0      + dt * htend
      theta      = theta0  + dt * thetatend
      dtheta     = dtheta0 + dt * dthetatend
      q          = q0      + dt * qtend
      dq         = dq0     + dt * dqtend
    
      if(sw_mlwind):
        u          = u0      + dt * utend
        du         = du0     + dt * dutend 
        v          = v0      + dt * vtend
        dv         = dv0     + dt * dvtend
      
      print((t * dt) / 3600. + tstart, theta, q, h)

class mlmodelinput():
  def __init__(self):
    # model variables

    # TIME
    self.dt         = 60.       # time step [s]
    self.time       = 60000.    # total time [s]
    self.tstart     = 4.0       # start of model [h UTC]

    # MIXED-LAYER
    self.h          = 200.      # initial ABL height [m]
    self.ws         = 0.        # large scale vertical velocity [m s-1]
    
    self.theta      = 288.      # initial mixed-layer potential temperature [K]
    self.dtheta     = 3.        # initial temperature jump at h [K]
    self.gammatheta = 0.006     # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   = 0.        # advection of heat [K s-1]
    self.beta       = 0.2       # entrainment ratio for virtual heat [-]
    
    self.q          = 0.009     # initial mixed-layer specific humidity [kg kg-1]
    self.dq         = -0.003    # initial specific humidity jump at h [kg kg-1]
    self.gammaq     = 0.        # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    self.advq       = 0.        # advection of moisture [kg kg-1 s-1]
    
    self.u          = 5.        # initial mixed-layer u-wind speed [m s-1]
    self.du         = 3.        # initial u-wind jump at h [m s-1]
    self.gammau     = 0.003     # free atmosphere u-wind speed lapse rate [s-1]
    self.advu       = 0.        # advection of u-wind [m s-2]

    self.v          = 5.        # initial mixed-layer u-wind speed [m s-1]
    self.dv         = 3.        # initial u-wind jump at h [m s-1]
    self.gammav     = 0.003     # free atmosphere v-wind speed lapse rate [s-1]
    self.advv       = 0.        # advection of v-wind [m s-2]
    
    self.wtheta     = 0.1       # surface kinematic heat flux [K m s-1]
    self.wq         = 0.000001  # surface kinematic moisture flux [kg kg-1 m s-1]
   
    # switches
    self.sw_mlwind    = False   # switch to enable mixed-layer wind
    self.sw_entrshear = False   # switch to enable shear effects on entrainment  


    # LAND SURFACE MODEL
    # soil properties
    self.a          = 0.219      # Clapp and Hornberger retention curve parameter
    self.b          = 4.90       # Clapp and Hornberger retention curve parameter
    self.p          = 4.         # Clapp and Hornberger retention curve parameter
    self.CGsat      = 3.56e-6    # saturated soil conductivity for heat
    self.C1sat      = 0.132      
    self.C2ref      = 1.8
    self.wsat       = 0.472      # saturated volumetric water content ECMWF config
    self.wfc        = 0.323      # volumetric water content field capacity
    self.wwilt      = 0.171      # volumetric water content wilting point
    
    # soil initial conditions
    self.wg         = 0.30       # volumetric water content top soil layer [m3 m-3]
    self.w2         = 0.22       # volumetric water content deeper soil layer [m3 m-3]
    self.Tsoil      = 285.       # temperature top soil layer [K]
    self.T2         = 286.       # temperature deeper soil layer [K]
   
    # land surface properties
    self.cveg       = 0.8        # vegetation fraction [-]
    self.Wl         = 0.0002     # equivalent water layer depth for wet vegetation [m]
    self.LAI        = 2.         # leaf area index [-]
    self.rsmin      = 110.       # minimum resistance transpiration
    self.rssoilmin  = 50.        # minimun resistance soil evaporation
    self.z0m        = 0.1        # roughness length for momentum [m]
    self.z0h        = 0.02       # roughness length for heat and moisture [m]
    self.alpha      = 0.25       # surface albedo
    self.Lambda     = 5.9        # thermal diffusivity skin layer [W K-1 m-2]

    # land surface initial conditions
    self.Ts         = 280.       # initial skin temperature [K]
    self.Wmax       = 0.0002     # thickness of water layer on wet vegetation [m]
        
    # switches
    self.sw_ls      = False      # switch to enable land surface model
    self.sw_lsland  = False      # switch to set sea (False) or land (True)
    self.sw_lsrs    = False      # switch to enable dynamic surface resistance
    self.sw_lsstab  = False      # switch to enable stability corrections


    # RADIATION MODEL
    # radiation parameters
    self.lat        = 43.5       # latitude [deg]
    self.lon        = 1.5        # longitude [deg]
    self.doy        = 167.       # day of the year [-]
    self.S0         = 1300.      # maximum incoming shortwave radiation [W m-2]

    # switches
    self.sw_rad     = False      # switch to enable dynamic radiation

