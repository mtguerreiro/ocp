import pyocp
import struct

import numpy as np
import matplotlib.pyplot as plt
plt.ion()

import time

import pickle

import scipy.signal

# --- Input ---
port = 8080

host = '131.246.75.197'
settings = {'host':host, 'port':port}

bb = pyocp.lrs.fsbuckboost.iface.Interface('ethernet', settings, cs_id=1, tr_id=1)

