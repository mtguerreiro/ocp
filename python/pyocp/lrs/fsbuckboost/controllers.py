"""
Module ``buck_controller``
===========================


"""
import pyocp
import struct
import numpy as np
import scipy.signal
import control

from dataclasses import dataclass

@dataclass
class _ModelParams:
    V_in : float = 16
    R : float = 20
    L : float = 47e-6
    Co : float = 150e-6


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

        self.idle = _Idle(0, ctl_if)
        self.ramp = _Ramp(1, ctl_if)
        #self.sfb = _SFB(2, ctl_if)
        #self.cascaded = _Cascaded(3, ctl_if)


class _Idle(pyocp.controller.ControllerTemplate):
    
    def __init__(self, ctl_id, ctl_if):
        super().__init__(ctl_id, ctl_if)


class _Ramp(pyocp.controller.ControllerTemplate):
    
    def __init__(self, ctl_id, ctl_if):
        super().__init__(ctl_id, ctl_if)
        

    def _decode(self, params_bin):
        
        fmt = '<' + 'f' * round( len(params_bin) / 4 )
        data = struct.unpack(fmt, params_bin)

        params = {}
        params['uinc'] = data[0]
        params['u_upper'] = data[1]

        return params


    def _encode(self, params):

        fmt = '<' + 'f' * len(params)
        
        uinc = params['uinc']
        u_upper = params['u_upper']

        params_bin = struct.pack(fmt, uinc, u_upper)

        return params_bin
