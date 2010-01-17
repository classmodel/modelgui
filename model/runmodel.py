# test model
# run mlmodel

from pylab import *
from model import *

run1input = modelinput()

run1input.dt         = 60.       # time step [s]
run1input.runtime    = 43200.    # total run time [s]

# mixed-layer input
run1input.h          = 200.      # initial ABL height [m]
run1input.Ps         = 101300.   # surface pressure [Pa]
run1input.ws         = 0.        # large scale vertical velocity [m s-1]
run1input.fc         = 1.e-4     # Coriolis parameter [m s-1]

run1input.theta      = 288.      # initial mixed-layer potential temperature [K]
run1input.dtheta     = 5.        # initial temperature jump at h [K]
run1input.gammatheta = 0.006     # free atmosphere potential temperature lapse rate [K m-1]
run1input.advtheta   = 0.        # advection of heat [K s-1]
run1input.beta       = 0.2       # entrainment ratio for virtual heat [-]
run1input.wtheta     = 0.1       # surface kinematic heat flux [K m s-1]

run1input.q          = 0.009     # initial mixed-layer specific humidity [kg kg-1]
run1input.dq         = -0.003    # initial specific humidity jump at h [kg kg-1]
run1input.gammaq     = 0.        # free atmosphere specific humidity lapse rate [kg kg-1 m-1]
run1input.advq       = 0.        # advection of moisture [kg kg-1 s-1]
run1input.wq         = 0.000001  # surface kinematic moisture flux [kg kg-1 m s-1]

run1input.u          = 4.        # initial mixed-layer u-wind speed [m s-1]
run1input.du         = 2.        # initial u-wind jump at h [m s-1]
run1input.gammau     = 0.        # free atmosphere u-wind speed lapse rate [s-1]
run1input.advu       = 0.        # advection of u-wind [m s-2]

run1input.v          = 1.        # initial mixed-layer u-wind speed [m s-1]
run1input.dv         = 0.        # initial u-wind jump at h [m s-1]
run1input.gammav     = 0.        # free atmosphere v-wind speed lapse rate [s-1]
run1input.advv       = 0.        # advection of v-wind [m s-2]

# surface layer input
run1input.ustar      = 0.3       # surface friction velocity [m s-1]
run1input.z0m        = 0.05      # roughness length for momentum [m]
run1input.z0h        = 0.01      # roughness length for scalars [m]

# radiation parameters
run1input.lat        = 43.5      # latitude [deg]
run1input.lon        = 1.5       # longitude [deg]
run1input.doy        = 268.      # day of the year [-]
run1input.tstart     = 7.        # time of the day [h UTC]
run1input.S0         = 1300.     # maximum incoming shortwave radiation [W m-2]

# land surface parameters
run1input.wg         = 0.22      # volumetric water content top soil layer [m3 m-3]
run1input.w2         = 0.22      # volumetric water content deeper soil layer [m3 m-3]
run1input.cveg       = 0.8       # vegetation fraction [-]
run1input.Tsoil      = 285.      # temperature top soil layer [K]
run1input.T2         = 286.      # temperature deeper soil layer [K]
run1input.a          = 0.219     # Clapp and Hornberger retention curve parameter a
run1input.b          = 4.90      # Clapp and Hornberger retention curve parameter b
run1input.p          = 4.        # Clapp and Hornberger retention curve parameter c
run1input.CGsat      = 3.56e-6   # saturated soil conductivity for heat

run1input.wsat       = 0.472     # saturated volumetric water content ECMWF config [-]
run1input.wfc        = 0.323     # volumetric water content field capacity [-]
run1input.wwilt      = 0.171     # volumetric water content wilting point [-]

run1input.C1sat      = 0.132     
run1input.C2ref      = 1.8

run1input.LAI        = 2.        # leaf area index [-]
run1input.rsmin      = 40.       # minimum resistance transpiration [s m-1]
run1input.rssoilmin  = 50.       # minimun resistance soil evaporation [s m-1]
run1input.alpha      = 0.25      # surface albedo [-]

run1input.Ts         = 280.      # initial mixed layer potential temperature [K]

run1input.Wmax       = 0.0002    # thickness of water layer on wet vegetation [m]
run1input.Wl         = 0.0002    # equivalent water layer depth for wet vegetation [m]
run1input.cl         = 0.        # wet fraction [-]

run1input.Lambda     = 5.9       # thermal diffusivity soil [-]

run1 = model(run1input)
run1.runmodel()

