import pyocp
import struct

import numpy as np
import matplotlib.pyplot as plt
plt.ion()

import time

# --- Input ---
host = '131.246.75.197'
port = 8080
settings = {'host':host, 'port':port}

ocp = pyocp.ocp.Interface(comm_type='ethernet', settings=settings)

