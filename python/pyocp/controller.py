"""
Module ``controller``
=====================


"""
import pyocp
import struct

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
