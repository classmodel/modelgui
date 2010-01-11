# test model
# run mlmodel

from model import *

run1input = modelinput()

run1input.dt         = 90.       # time step [s]
run1input.runtime    = 43200.    # total run time [s]

run1input.h          = 200.      # initial ABL height [m]
run1input.Ps         = 101300.   # surface pressure [Pa]
run1input.ws         = 0.        # large scale vertical velocity [m s-1]

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

run1input.u          = 5.        # initial mixed-layer u-wind speed [m s-1]
run1input.du         = 3.        # initial u-wind jump at h [m s-1]
run1input.gammau     = 0.003     # free atmosphere u-wind speed lapse rate [s-1]
run1input.advu       = 0.        # advection of u-wind [m s-2]

run1input.v          = 0.001     # initial mixed-layer u-wind speed [m s-1]
run1input.dv         = 0.        # initial u-wind jump at h [m s-1]
run1input.gammav     = 0.        # free atmosphere v-wind speed lapse rate [s-1]
run1input.advv       = 0.        # advection of v-wind [m s-2]

run1input.ustar      = 0.3       # surface friction velocity [m s-1]
run1input.z0m        = 0.05      # roughness length for momentum [m]
run1input.z0h        = 0.01      # roughness length for scalars [m]

run1 = model(run1input)
run1.runmodel()
