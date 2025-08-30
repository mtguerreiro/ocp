import pyocp
import buck

import struct
import numpy as np
import matplotlib.pyplot as plt
plt.ion()

host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

ocp = pyocp.ocp.Interface(comm_type='ethernet', settings=settings)

buck_if = buck.Interface('ethernet', settings)

def cfg_cascaded():
    buck_if.cascaded.set_gains(ts=1e-3, os=2)
    buck_if.cascaded.set_params({'i_max':4, 'i_min':-4})
    buck_if.cascaded.enable()

def cfg_trace_trig():
    buck_if.trace.set_size(200)
    buck_if.trace.set_trig_signal(4)
    buck_if.trace.set_n_pre_trig_samples(20)
    buck_if.trace.set_trig_level(5.75)
    buck_if.trace.set_mode(1)
