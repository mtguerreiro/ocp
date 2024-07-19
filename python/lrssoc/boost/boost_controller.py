"""
Module ``boost_controller``
===========================


"""
import lrssoc
import struct
import numpy as np
import scipy.signal
import control

class References:
    """
    """
    def __init__(self):
        pass
    

    def encode(self, ref):

        r = float(ref)
        data = list(struct.pack('<f', r))

        return data

    
    def decode(self, data):

        ref = struct.unpack('<f', data)[0]
        
        return ref


class Controllers:
    """
    """
    def __init__(self):
        self.ctl = {
            0            : {'id':0, 'if':None},
            'startup'    : {'id':1, 'if':Startup()},
            'energy_int' : {'id':2, 'if':EnergyInt()},
            'energy_mpc' : {'id':3, 'if':EnergyMpc()},
            }


class Startup:
    def __init__(self):
        pass
    

    def set(self, params):

        uinc = params['uinc']
        ufinal = params['ufinal']
        data = list(struct.pack('<ff', uinc, ufinal))
        
        return data
    

    def get(self, data):

        pars = struct.unpack('<ff', data)

        params = {
            'uinc': pars[0],
            'ufinal': pars[1]
            }

        return params


class EnergyInt:
    def __init__(self):
        pass
    

    def set(self, params):

        k1 = params['k1']
        k2 = params['k2']
        k3 = params['k3']
        dt = params['dt']
        
        data = list(struct.pack('<ffff', k1, k2, k3, dt))
        
        return data
    

    def get(self, data):

        pars = struct.unpack('<ffff', data)

        params = {
            'k1': pars[0],
            'k2': pars[1],
            'k3': pars[2],
            'dt': pars[3],
            }

        return params

    
    def gains(self, ts, os=5, method='approx', alpha=5.0, dt=1.0):

        # Poles
        if method == 'approx':
            zeta = -np.log(os/100) / np.sqrt(np.pi**2 + (np.log(os/100))**2)
            wn = 4/ts/zeta

            p1 = -zeta * wn + wn * np.sqrt(zeta**2 - 1, dtype=complex)
            p2 = np.conj(p1)
            p3 = alpha * p1.real

            poles = [p1, p2, p3]

        elif method == 'bessel':
            p1 = (-3.9668 + 3.7845j) / ts
            p2 = np.conj(p1)
            p3 = -5.0093 / ts

        elif method == 'itae':
            p1 = (-4.35 + 8.918j) / ts
            p2 = np.conj(p1)
            p3 = -5.913 / ts

        else:
            print('Unknown method')
            return 0
            
        poles = [p1, p2, p3]
        #print('Pole placement.\nMethod: {:}'.format(method))
        #print('Poles: {:}'.format(poles))
        
        # Augmented model        
        A = np.array([[ 0.0, 1.0, 0.0],
                      [ 0.0, 0.0, 0.0],
                      [-1.0, 0.0, 0.0]])

        B = np.array([[0.0], [1.0], [0.0]])

        # Gains
        K = scipy.signal.place_poles(A, B, poles).gain_matrix.reshape(-1)

        gains = {'k1':K[0], 'k2':K[1], 'k3':K[2], 'dt':dt}

        #print('Gains: {:}'.format(gains))

        return gains


class EnergyMpc:
    def __init__(self):
        pass
    

    def set(self, params):

        return []
    

    def get(self, data):

        return []


class Controller:
    """

    Parameters
    ----------

    Raises
    ------

    Attributes
    ----------
        
    """
    def __init__(self, ocp_if, cs_id=0):

        self._ocp_if = ocp_if
        self._ctl_if = lrssoc.controller.controller.Controller(self._ocp_if)

        self._cs_id = cs_id
        self._ctl = Controllers().ctl

        self._ref = References()
        

    def set(self, controller):
        """

        Parameters
        ----------

        Raises
        ------

        """
        if controller not in self._ctl:
            print('Uknown controller')
            return -1

        ctl_id = self._ctl[controller]['id']

        status, = self._ctl_if.set(self._cs_id, ctl_id)

        if status < 0 :
            return (-1, status)
            
        return (0,)


    def get(self):
        """

        Parameters
        ----------

        Raises
        ------

        """

        status, controller = self._ctl_if.get(self._cs_id)
        
        return (status, controller)


    def set_params(self, controller, params):
        """

        Parameters
        ----------

        Raises
        ------
        """
        if controller not in self._ctl:
            print('Uknown controller')
            return (-1,)

        if self._ctl[controller]['if'] is None:
            print('Error setting controller params. Undefined interface\r\n'.format(status))
            return (-1,)

        ctl_id = self._ctl[controller]['id']
        ctl_data = self._ctl[controller]['if'].set(params)

        status = self._ctl_if.set_params(self._cs_id, ctl_id, ctl_data)
          
        return status


    def get_params(self, controller):
        """
        """
        if controller not in self._ctl:
            print('Uknown controller')
            return (-1,)

        if self._ctl[controller]['if'] is None:
            print('Error getting controller params. Undefined interface.\r\n'.format(status))
            return (-1,)

        ctl_id = self._ctl[controller]['id']

        status, data = self._ctl_if.get_params(self._cs_id, ctl_id)

        if status != 0:
            return (-1, status)
        
        params = self._ctl[controller]['if'].get(data)
        
        return (0, params)


    def reset(self, controller):
        """
        """
        if controller not in self._ctl:
            print('Uknown controller')
            return -1

        ctl_id = self._ctl[controller]['id']

        status = self._ctl_if.reset(self._cs_id, ctl_id)
      
        return status


    def set_ref(self, ref):
        """
        """
        ref_data = self._ref.encode( float(ref) )

        status = self._ctl_if.set_ref(self._cs_id, ref_data)

        return status


    def get_ref(self):
        """
        """
        status, data = self._ctl_if.get_ref(self._cs_id)

        if status != 0:
            return (-1, status)
        
        refs = self._ref.decode(data)
        
        return (0, refs)
