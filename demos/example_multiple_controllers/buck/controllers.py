import struct
import numpy as np
import scipy.signal

import pyocp

from dataclasses import dataclass

@dataclass
class _ModelParams:
    V_in : float = 12
    R : float = 10
    L : float = 47e-6
    Co : float = 220e-6
    dt : float = 1/100e3


class Reference(pyocp.controller.ReferenceTemplate):

    def __init__(self, ctl_if):
        super().__init__(ctl_if)


    def _decode(self, ref_bin):
        
        fmt = '<f'
        ref = struct.unpack(fmt, ref_bin)

        return ref


    def _encode(self, ref):

        fmt = '<f'
        
        ref_bin = struct.pack(fmt, ref)

        return ref_bin


class Controllers:

    def __init__(self, ctl_if):

        self.sfb = _SFB(0, ctl_if)
        self.cascaded = _Cascaded(1, ctl_if)


class _Cascaded(pyocp.controller.ControllerTemplate):

    def __init__(self, ctl_id, ctl_if):

        super().__init__(ctl_id, ctl_if)

        self.keys = (
            'ki', 'k_ei', 'kv', 'k_ev',
            'i_max', 'i_min', 'dt'
        )
        self._model_params = _ModelParams


    def _decode(self, params_bin):
        
        return _dict_decode(self.keys, params_bin)


    def _encode(self, params):

        return _dict_encode(self.keys, params)


    def get_model_params(self):

        return self._model_params
        

    def set_model_params(self, params):

        self._model_params = params


    def set_gains(self, ts=1e-3, os=5):

        params = self._get_gains(ts=ts, os=os)

        return self.set_params(params)


    def _get_gains(self, ts=1e-3, os=5):

        # Model
        V_in = self._model_params.V_in
        R = self._model_params.R
        L = self._model_params.L
        Co = self._model_params.Co
        dt = self._model_params.dt
        
        ts_v = ts
        os_v = os
        
        os_i = os_v
        ts_i = ts_v / 5

        zeta_i, wn_i = _zeta_wn(ts_i, os_i)
        ki = (L / V_in) * 2 * zeta_i * wn_i
        k_ei = (L / V_in) * ( - wn_i**2 )

        zeta_v, wn_v = _zeta_wn(ts_v, os_v)
        kv = ( Co ) * ( 2 * zeta_v * wn_v - 1 / R / Co )
        k_ev = ( Co ) * ( - wn_v**2 )

        return {'ki': ki, 'k_ei': k_ei, 'kv': kv, 'k_ev':k_ev, 'dt':dt}


class _SFB(pyocp.controller.ControllerTemplate):
    
    def __init__(self, ctl_id, ctl_if):
        
        super().__init__(ctl_id, ctl_if)
        self.keys = (
            'ki', 'k_ei', 'kv', 'dt'
        )
        self._model_params = _ModelParams


    def _decode(self, params_bin):
        
        return _dict_decode(self.keys, params_bin)


    def _encode(self, params):

        return _dict_encode(self.keys, params)


    def get_model_params(self):

        return self._model_params
    

    def set_model_params(self, params):

        self._model_params = params


    def set_gains(self, ts=1e-3, os=5):

        params = self._get_gains(ts=ts, os=os)

        return self.set_params(params)


    def _get_gains(self, ts=1e-3, os=5):

        # Model
        V_in = self._model_params.V_in
        R = self._model_params.R
        L = self._model_params.L
        Co = self._model_params.Co
        dt = self._model_params.dt
        
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
        zeta, wn = _zeta_wn(ts, os)
        p1 = -zeta * wn + 1j * wn * np.sqrt(1 - zeta**2)
        p2 = p1.conj()
        p3 = 5 * p1.real
        p = [p1, p2, p3]
        
        # Controller design
        K = scipy.signal.place_poles(Aa, Ba, p).gain_matrix[0]
        
        ki = K[0]
        kv = K[1]
        k_ev = K[2]
        
        return {'ki':ki, 'kv':kv, 'k_ev':k_ev, 'dt':dt}


def _dict_encode(keys, params):
    
    _params = [params[key] for key in keys]
    params_bin = struct.pack(f'<{len(keys)}f', *_params)

    return params_bin    


def _dict_decode(keys, params_bin):
    
    _params = struct.unpack(f'<{len(keys)}f', params_bin)
    params = dict(zip(keys, _params))

    return params


def _zeta_wn(ts, os):

    zeta = -np.log(os / 100) / np.sqrt( np.pi**2 + np.log(os / 100)**2 )
    wn = 4 / ts / zeta

    return (zeta, wn)
