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

def trace_data_to_np_array(trace_data, n_signals):

    n = len(trace_data)

    fmt = '<' + 'f' * round(n / 4)

    data = np.array(struct.unpack(fmt, trace_data))

    data = data.reshape(round(n / n_signals / 4), n_signals)

    return data
