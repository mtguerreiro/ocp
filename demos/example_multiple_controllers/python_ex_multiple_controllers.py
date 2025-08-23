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

b = buck.Interface('ethernet', settings)

def cfg_cascaded():
    b.cascaded.set_gains(ts=1e-3, os=2)
    b.cascaded.set_params({'i_max':4})
    b.cascaded.enable()

def cfg_trace_trig():
    b.trace.set_size(200)
    b.trace.set_trig_signal(4)
    b.trace.set_n_pre_trig_samples(20)
    b.trace.set_trig_level(5.75)
    b.trace.set_mode(1)
