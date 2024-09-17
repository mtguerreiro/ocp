"""
Module ``buck_trace``
======================


"""
import lrssoc
import struct

class Trace:
    """

    Parameters
    ----------

    Raises
    ------

    Attributes
    ----------
        
    """
    def __init__(self, ocp_if, tr_id=0):

        self._tr_id = tr_id
        self._ocp_if = ocp_if
        
        
    def read(self):
        """
        """
        status, trace_data = self._ocp_if.trace_read( self._tr_id)
        if status != 0 :
            print('Error reading trace data')
            return (-1, status)

        status, n_traces = self._ocp_if.trace_get_number_signals( self._tr_id )
        if status != 0 :
            print('Error getting number of traces')
            return (-1, status)

        n = int( len(trace_data) / 4 )
        fmt = '<' + 'f' * n
        unpacked_data = struct.unpack(fmt, trace_data)

        status, trace_names = self._ocp_if.trace_get_signals_names( self._tr_id )
        if status != 0 :
            print('Error getting names of trace signals')
            return (-1, status)

        data = [ list(unpacked_data[i::n_traces]) for i in range(n_traces) ]
            
        return (0, (trace_names, data))


    def reset(self):
        """
        """
        status = self._ocp_if.trace_reset( self._tr_id )

        if status[0] < 0 :
            return (-1, status)

        return (0,)


    def set_size(self, size):
        """
        """
        status, n_traces = self._ocp_if.trace_get_number_signals( self._tr_id )
        if status != 0 :
            print('Error getting number of traces')
            return (-1, status)

        trace_size = int( 4 * n_traces * size )

        status = self._ocp_if.trace_set_size( self._tr_id, trace_size )

        if status[0] < 0 :
            return (-1, status[0])

        return (0,)


    def get_size(self):
        """
        """
        status, n_traces = self._ocp_if.trace_get_number_signals( self._tr_id )
        if status != 0 :
            print('Error getting number of traces')
            return (-1, status)
        
        status, trace_size = self._ocp_if.trace_get_size( self._tr_id )
        if status != 0 :
            return (-1, status)

        size = int( trace_size / n_traces / 4 )

        return (0, size)

    def enable_trig_mode(self):
        """
        """
        status = self._ocp_if.trace_enable_trig_mode( self._tr_id )

        if status[0] < 0 :
            return (-1, status)

        return (0,)

    def enable_manual_mode(self):
        """
        """
        status = self._ocp_if.trace_enable_manual_mode( self._tr_id )

        if status[0] < 0 :
            return (-1, status)

        return (0,)

    def set_num_pre_trig_samples(self, num):
        """
        """
        status = self._ocp_if.trace_set_num_pre_trig_samples( self._tr_id, num )

        if status[0] < 0 :
            return (-1, status[0])

        return (0,)

    def set_trace_to_track(self, trace):
        """
        """
        status = self._ocp_if.trace_set_trace_to_track( self._tr_id, trace )

        if status[0] < 0 :
            return (-1, status[0])

        return (0,)

    def set_trig_bound(self, bound):
        """
        """
        status = self._ocp_if.trace_set_trig_bound( self._tr_id, bound )

        if status[0] < 0 :
            return (-1, status[0])

        return (0,)

    def trig_mode_reset(self):
        """
        """
        status = self._ocp_if.trace_trig_mode_reset( self._tr_id )

        if status[0] < 0 :
            return (-1, status)

        return (0,)
    
