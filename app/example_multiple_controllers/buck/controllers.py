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
        self._model_params = _ModelParams


    def _decode(self, params_bin):
        
        fmt = '<' + 'f' * round( len(params_bin) / 4 )
        data = struct.unpack(fmt, params_bin)

        params = {}
        params['ki'] = data[0]
        params['k_ei'] = data[1]
        params['kv'] = data[2]
        params['k_ev'] = data[3]

        params['i_max'] = data[4]
        params['i_min'] = data[5]

        return params


    def _encode(self, params):

        fmt = '<' + 'f' * len(params)
        
        ki = params['ki']
        k_ei = params['k_ei']
        kv = params['kv']
        k_ev = params['k_ev']

        i_max = params['i_max']
        i_min = params['i_min']

        params_bin = struct.pack(fmt, ki, k_ei, kv, k_ev, i_max, i_min)

        return params_bin


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
        CO = self._model_params.Co
        
        ts_v = ts
        os_v = os
        
        os_i = os_v
        ts_i = ts_v / 5

        zeta_i, wn_i = self._zeta_wn(ts_i, os_i)
        ki = (L / V_in) * 2 * zeta_i * wn_i
        k_ei = (L / V_in) * ( - wn_i**2 )

        zeta_v, wn_v = self._zeta_wn(ts_v, os_v)
        kv = ( CO ) * ( 2 * zeta_v * wn_v - 1 / R / CO )
        k_ev = ( CO ) * ( - wn_v**2 )

        return {'ki': ki, 'k_ei': k_ei, 'kv': kv, 'k_ev':k_ev}


    def _zeta_wn(self, ts, os):

        zeta = -np.log(os / 100) / np.sqrt( np.pi**2 + np.log(os / 100)**2 )
        wn = 4 / ts / zeta

        return (zeta, wn)


class _SFB(pyocp.controller.ControllerTemplate):
    
    def __init__(self, ctl_id, ctl_if):
        
        super().__init__(ctl_id, ctl_if)
        self._model_params = _ModelParams


    def _decode(self, params_bin):
        
        fmt = '<' + 'f' * round( len(params_bin) / 4 )
        data = struct.unpack(fmt, params_bin)

        params = {}
        params['ki'] = data[0]
        params['kv'] = data[1]
        params['k_ev'] = data[2]

        return params


    def _encode(self, params):

        fmt = '<' + 'f' * len(params)
        
        ki = params['ki']
        kv = params['kv']
        k_ev = params['k_ev']

        params_bin = struct.pack(fmt, ki, kv, k_ev)

        return params_bin


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
        
        return {'ki':ki, 'kv':kv, 'k_ev':k_ev}
