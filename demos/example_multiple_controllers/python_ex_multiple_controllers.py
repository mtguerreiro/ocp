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
