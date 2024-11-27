import struct
import numpy as np
import scipy.signal

def get_gains(ts=1e-3, os=5):

    # Model
    V_in = 12
    R = 10
    L = 47e-6
    Co = 220e-6
    
    A = np.array([
        [0,         -1 / L],
        [1 / Co,    -1 / R / Co]
        ])
    
    B = np.array([
        [V_in / L],
        [0]
        ])

    C = np.array([0, 1])

    # Augmented model (integrator)
    Aa = np.zeros((3,3))
    Aa[:2, :2] = A
    Aa[2, :2] = -C

    Ba = np.zeros((3,1))
    Ba[:2, 0] = B[:, 0]

    # Poles
    zeta = -np.log(os/100) / np.sqrt( np.pi**2 + (np.log(os/100))**2 )
    wn = 4 / ts / zeta

    p1 = -zeta * wn + 1j * wn * np.sqrt(1 - zeta**2)
    p2 = p1.conj()
    p3 = 5 * p1.real
    p = [p1, p2, p3]
    
    # Controller design
    K = scipy.signal.place_poles(Aa, Ba, p).gain_matrix[0]
    
    ki = K[0]
    kv = K[1]
    k_ev = K[2]
    
    return (ki, kv, k_ev)


def format_gains(ki, kv, k_ev):

    data_bin = struct.pack('<fff', ki, kv, k_ev)

    return data_bin


def format_params(ki, kv, k_ev, dt, v_ref):

    data_bin = struct.pack('<fffff', ki, kv, k_ev, dt, v_ref)

    return data_bin
