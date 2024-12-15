"""
Module ``controller``
=====================


"""
import pyocp
import struct

class ControllerTemplate:
    """
    """
    def __init__(self, ctl_id, ctl_if):

        self._ctl_id = ctl_id
        self._ctl_if = ctl_if
        

    def enable(self):

        return self._ctl_if.set(self._ctl_id)


    def set_params(self, params):

        status, new_params = self.get_params()
        if status < 0:
            return (-1, status)
        
        for param, val in params.items():
            if param in new_params:
                new_params[param] = val

        params = self._encode(new_params)
            
        return self._ctl_if.set_params(self._ctl_id, params)

    
    def get_params(self):

        status, params = self._ctl_if.get_params(self._ctl_id)

        if status < 0:
            return (-1, status)

        params = self._decode(params)
        
        return (0, params)


    def _encode(self, params):

        return params


    def _decode(self, params):

        return params


class ReferenceTemplate:
    """
    """
    def __init__(self, ctl_if):

        self._ctl_if = ctl_if
        

    def set_ref(self, ref):

        ref_bin = self._encode(ref)

        return self._ctl_if.set_ref(ref_bin)


    def get_ref(self):

        status, ref_bin = self._ctl_if.get_ref()

        if status < 0:
            return (-1, status)

        ref = self._decode(ref_bin)
        
        return (0, ref)


    def _encode(self, params):

        return params


    def _decode(self, params):

        return params


class Commands:
    """
    """
    def __init__(self):
        self.set_refs = 0
        self.get_refs = 1
        self.set = 2
        self.get = 3
        self.reset = 4
        self.set_params = 5
        self.get_params = 6


class Interface:
    """

    Parameters
    ----------

    Raises
    ------

    Attributes
    ----------
        
    """
    def __init__(self, ctl_if, cs_id=0):

        self._cmd = Commands()
        self._cs_id = cs_id
        self._ctl_if = ctl_if


    def set_ref(self, ref):
        cmd = self._cmd.set_refs

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        tx_data.extend( ref )

        status, _ = self._ctl_if(self._cs_id, tx_data)

        if status < 0:
            print('Error setting controller refs. Error code {:}\r\n'.format(status))
            return (-1, status)
        
        return (0,)


    def get_ref(self):
        """
        """
        cmd = self._cmd.get_refs

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        
        status, rx_data = self._ctl_if(self._cs_id, tx_data)

        if status < 0:
            print('Error getting controller refs. Error code {:}\r\n'.format(status))
            return (-1, status)
       
        return (0, rx_data)
    

    def set(self, ctl_id):
        """

        Parameters
        ----------

        Raises
        ------

        """    
        cmd = self._cmd.set

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(ctl_id, msb=False) )

        status, _ = self._ctl_if(self._cs_id, tx_data)

        if status < 0:
            print('Error setting the controller. Error code {:}\r\n'.format(status))
            return (-1, status)
        
        return (0,)


    def get(self):
        """

        Parameters
        ----------

        Raises
        ------

        """
        cmd = self._cmd.get

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )

        status, rx_data = self._ctl_if(self._cs_id, tx_data)

        if status < 0:
            print('Error getting the controller. Error code {:}\r\n'.format(status))
            return (-1, status)
        
        controller = pyocp.conversions.u8_to_u32(rx_data, msb=False)
        
        return (0, controller)


    def reset(self, ctl_id):
        """
        """
        cmd = self._cmd.reset

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(ctl_id, msb=False) )

        status, _ = self._ctl_if(self._cs_id, tx_data)   

        if status < 0:
            print('Error resetting controller. Error code {:}\r\n'.format(status))
            return (-1, status)
        
        return (0,)


    def set_params(self, ctl_id, params):
        """

        Parameters
        ----------

        Raises
        ------
        """
        cmd = self._cmd.set_params

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(ctl_id, msb=False) )
        tx_data.extend( params )

        status, _ = self._ctl_if(self._cs_id, tx_data)

        if status < 0:
            print('Error setting controller params. Error code {:}\r\n'.format(status))
            return (-1, status)

        return (0,)


    def get_params(self, ctl_id):
        """
        """
        cmd = self._cmd.get_params

        tx_data = []
        tx_data.extend( pyocp.conversions.u32_to_u8(cmd, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(self._cs_id, msb=False) )
        tx_data.extend( pyocp.conversions.u32_to_u8(ctl_id, msb=False) )

        status, rx_data = self._ctl_if(self._cs_id, tx_data)   

        if status < 0:
            print('Error getting controller params. Error code {:}\r\n'.format(status))
            return (-1, status)
       
        return (0, rx_data)
