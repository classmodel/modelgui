# mixed-layer model
# Chiel van Heerwaarden & Jordi Vila-Guerau de Arellano, 2009

import copy
import numpy

class model:
  def __init__(self, modelinput):
    # initialize the different components of the model
    self.input = copy.deepcopy(modelinput)
    # initialize mixed-layer
    self.mlmodel = mlmodel(self.input)
    # initialize surface-layer
    self.slmodel = slmodel(self.input)

    # initialize time variables
    self.tsteps = int(numpy.floor(self.input.runtime / self.input.dt))
    self.dt     = self.input.dt

    # initialize output
    self.out = modeloutput(self.tsteps)
    
    # store initial values in output
    self.store(0)
    self.mlmodel.store(self.out, 0)
    self.slmodel.store(self.out, 0)

  def runmodel(self):

    # time integrate model 
    for t in range(self.tsteps):
      # run components
      self.slmodel.run(self.mlmodel)

      # time integrate components
      self.mlmodel.integrate(self.dt)
      
      # store output for current time step
      self.store(t)
      self.mlmodel.store(self.out, t)
      self.slmodel.store(self.out, t)

  def store(self, t):
    self.out.t[t] = t * self.dt

   
# class for storing mixed-layer model input data
class modelinput:
  def __init__(self):
    # general model variables
    self.runtime    = -1. # duration of model run [s]
    self.dt         = -1. # time step [s]

    # mixed-layer variables
    self.h          = -1. # initial ABL height [m]
    self.ws         = -1. # large scale vertical velocity [m s-1]
    
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
    self.ustar      = -1. # surface friction velocity [m s-1]


    
class mlmodel:
  def __init__(self, modelinput):
    self.h          =  modelinput.h          # initial ABL height [m]
    self.ws         =  modelinput.ws         # large scale vertical velocity [m s-1]
    
    self.theta      =  modelinput.theta      # initial mixed-layer potential temperature [K]
    self.dtheta     =  modelinput.dtheta     # initial temperature jump at h [K]
    self.gammatheta =  modelinput.gammatheta # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   =  modelinput.advtheta   # advection of heat [K s-1]
    self.beta       =  modelinput.beta       # entrainment ratio for virtual heat [-]
    self.wtheta     =  modelinput.wtheta     # surface kinematic heat flux [K m s-1]
    self.thetasurf  =  -1                    # surface potential temperature [K]
    
    self.q          =  modelinput.q          # initial mixed-layer specific humidity [kg kg-1]
    self.dq         =  modelinput.dq         # initial specific humidity jump at h [kg kg-1]
    self.gammaq     =  modelinput.gammaq     # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
    self.advq       =  modelinput.advq       # advection of moisture [kg kg-1 s-1]
    self.wq         =  modelinput.wq         # surface kinematic moisture flux [kg kg-1 m s-1]
    
    self.u          =  modelinput.u          # initial mixed-layer u-wind speed [m s-1]
    self.du         =  modelinput.du         # initial u-wind jump at h [m s-1]
    self.gammau     =  modelinput.gammau     # free atmosphere u-wind speed lapse rate [s-1]
    self.advu       =  modelinput.advu       # advection of u-wind [m s-2]
    
    self.v          =  modelinput.v          # initial mixed-layer u-wind speed [m s-1]
    self.dv         =  modelinput.dv         # initial u-wind jump at h [m s-1]
    self.gammav     =  modelinput.gammav     # free atmosphere v-wind speed lapse rate [s-1]
    self.advv       =  modelinput.advv       # advection of v-wind [m s-2]
    self.ustar      =  modelinput.ustar      # surface friction velocity [m s-1]

  def run(self):
    # compute mixed-layer tendencies
    # first compute necessary virtual temperature units
    thetav     = self.theta  + 0.61 * self.theta * self.q
    wthetav    = self.wtheta + 0.61 * self.theta * self.wq
    dthetav    = (self.theta + self.dtheta) * (1. + 0.61 * (self.q + self.dq)) - self.theta * (1. + 0.61 * self.q)
    
    # compute tendencies
    we         = (self.beta * wthetav) / dthetav
    #we         = (beta * wthetav + 5. * ustar ** 3. * thetav / (g * h)) / dthetav
    htend      = we + self.ws
    
    thetatend  = (self.wtheta + we * self.dtheta) / self.h + self.advtheta 
    qtend      = (self.wq     + we * self.dq)     / self.h + self.advq
    
    dthetatend = self.gammatheta * we - thetatend
    dqtend     = self.gammaq     * we - qtend

    self.thetasurf
    
  def integrate(self, dt):
    # set values previous time step
    theta0  = self.theta
    h0      = self.h
    dtheta0 = self.dtheta
    q0      = self.q
    dq0     = self.dq
    
    # integrate mixed-layer equations
    self.h        = h0      + dt * htend
    self.theta    = theta0  + dt * thetatend
    self.dtheta   = dtheta0 + dt * dthetatend
    self.q        = q0      + dt * qtend
    self.dq       = dq0     + dt * dqtend

  def store(self, modeloutput, t):
    modeloutput.h[t]        = self.h
    modeloutput.ws[t]       = self.ws
    
    modeloutput.theta[t]    = self.theta
    modeloutput.dtheta[t]   = self.dtheta
    modeloutput.gammatheta  = self.gammatheta
    modeloutput.advtheta[t] = self.advtheta
    modeloutput.beta[t]     = self.beta
    modeloutput.wtheta[t]   = self.wtheta
    
    modeloutput.q[t]        = self.q
    modeloutput.dq[t]       = self.dq
    modeloutput.gammaq[t]   = self.gammaq
    modeloutput.advq[t]     = self.advq
    modeloutput.wq[t]       = self.wq
    
    modeloutput.u[t]        = self.u
    modeloutput.du[t]       = self.du
    modeloutput.gammau[t]   = self.gammau
    modeloutput.advu[t]     = self.advu
    
    modeloutput.v[t]        = self.v
    modeloutput.dv[t]       = self.dv
    modeloutput.gammav[t]   = self.gammav
    modeloutput.advv[t]     = self.advv
    modeloutput.ustar[t]    = self.ustar
 

# class for storing mixed-layer model output data
class modeloutput:
  def __init__(self, tsteps):
    self.t          = numpy.zeros(tsteps)    # time [s]

    self.h          = numpy.zeros(tsteps)    # initial ABL height [m]
    self.ws         = numpy.zeros(tsteps)    # large scale vertical velocity [m s-1]
    
    self.theta      = numpy.zeros(tsteps)    # initial mixed-layer potential temperature [K]
    self.dtheta     = numpy.zeros(tsteps)    # initial temperature jump at h [K]
    self.gammatheta = numpy.zeros(tsteps)    # free atmosphere potential temperature lapse rate [K m-1]
    self.advtheta   = numpy.zeros(tsteps)    # advection of heat [K s-1]
    self.beta       = numpy.zeros(tsteps)    # entrainment ratio for virtual heat [-]
    self.wtheta     = numpy.zeros(tsteps)    # surface kinematic heat flux [K m s-1]
    self.thetasurf  = numpy.zeros(tsteps)    # surface potential temperature [K]
    
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
    self.ustar      = numpy.zeros(tsteps)    # surface friction velocity [m s-1]

    self.Cm         = numpy.zeros(tsteps)    # drag coefficient for momentum []
    self.Cs         = numpy.zeros(tsteps)    # drag coefficient for scalars []
    self.L          = numpy.zeros(tsteps)    # Obukhov length [m]


class slmodel:
  def __init__(self, modelinput):
    self.g          = 9.81
    self.z0m        = 0.05
    self.z0h        = 0.01

    self.Cm         = -1.    # drag coefficient for momentum []
    self.Cs         = -1.    # drag coefficient for scalars []
    self.L          = -1.    # Obukhov length [m]

  def run(self, mlmodel):
    thetavsurf = mlmodel.thetasurf * (1. + 0.61 * qsatsurf)
    thetav     = mlmodel.theta * (1. + 0.61 * mlmodel.q)
    wthetav    = mlmodel.wtheta + 0.61 * mlmodel.theta * mlmodel.wq

    zsl  = 0.1 * mlmodel.h
          
    Rib  = self.g / thetav * 0.1 * mlmodel.h * (thetav - thetavsurf) / (mlmodel.u ** 2. + mlmodel.v ** 2.)

    if(Rib > 0.):
      L    = 1.
      L0   = 2.
    else:
      L  = -1.
      L0 = -2.
     
    while (abs(L - L0) > 0.0001):
      L0      = L
      fx      = Rib - zsl / L * (numpy.log(zsl / self.z0h) - psih(zsl / L) + psih(self.z0h / L)) / (numpy.log(zsl / self.z0m) - psim(zsl / L) + psim(self.z0m / L)) ** 2.
      Lstart  = L - 0.001*L
      Lend    = L + 0.001*L
      fxdif   = ( (- zsl / Lstart * (numpy.log(zsl / self.z0h) - psih(zsl / Lstart) + psih(self.z0h / Lstart)) / (numpy.log(zsl / self.z0m) - psim(zsl / Lstart) + psim(self.z0m / Lstart)) ** 2.) - (-zsl / Lend * (numpy.log(zsl / self.z0h) - psih(zsl / Lend) + psih(self.z0h / Lend)) / (numpy.log(zsl / self.z0m) - psim(zsl / Lend) + psim(self.z0m / Lend)) ** 2.) ) / (Lstart - Lend)
      L       = L - fx / fxdif

    self.L    = L
    self.Cm   = k ** 2. / (numpy.log(zsl / self.z0m) - psim(zsl / L) + psim(self.z0m / L)) ** 2.
    self.Cs   = k ** 2. / (numpy.log(zsl / self.z0m) - psim(zsl / L) + psim(self.z0m / L)) / (numpy.log(zsl / self.z0h) - psih(zsl / L) + psih(self.z0h / L))
      
    #if(wthetav > 0.):
    #  wstar     = (g / thetav * h * wthetav) ** (1./3.)
    #else:
    #  wstar     = 0.0001
    #ueff      = numpy.sqrt(u ** 2. + wstar ** 2.)
    
    ueff       = numpy.sqrt(u ** 2. + v ** 2.)
    self.ustar = numpy.sqrt(self.Cm) * ueff

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

  def store(self, modeloutput, t):
    modeloutput.L[t]       = self.L
    modeloutput.Cm[t]      = self.Cm
    modeloutput.Cs[t]      = self.Cs

