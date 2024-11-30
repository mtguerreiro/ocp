import lrssoc
import buck

import struct
import numpy as np
import matplotlib.pyplot as plt
plt.ion()

host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

ocp = lrssoc.ocp.iface.Interface(comm_type='ethernet', settings=settings)

def data_controller(ts=2e-3, os=10, dt=1/100e3, v_ref=6):

    ki, kv, k_ev = buck.get_gains(2e-3, 10)
    data_bin = buck.format_params(ki, kv, k_ev, dt, v_ref)

    return data_bin


def decode_trace_data(data_bin, n_signals):

    n = len(data_bin)

    fmt = '<' + 'f' * round(n / 4)

    data = np.array(struct.unpack(fmt, data_bin))

    data = data.reshape(round(n / n_signals / 4), n_signals)

    return data
