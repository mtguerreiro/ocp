"""
Module ``trace``
================


"""
import pyocp
import struct
import numpy as np

_TRACE_MODE_MANUAL  = 0
_TRACE_MODE_TRIGGER = 1

_TRACE_DATA_SIZE_BYTES = 4

class TraceTemplate:
    """
    """
    def __init__(self, tr_id, ocp_if):

        self._tr_id = tr_id
        self._ocp_if = ocp_if


    def read(self):

        status, data_bin = self._ocp_if.trace_read(self._tr_id)
        if status < 0:
            return (-1, status)

        status, mode = self.get_mode()
        if status < 0:
            return (-1, status)

        if mode == _TRACE_MODE_TRIGGER:
            status, data_bin = self._reorder_data(data_bin)
            if status < 0:
                return (-1, status)

        status, data = self._decode(data_bin)
        if status < 0:
            return(-1, status)
        
        return (status, data)


    def reset(self):

        return self._ocp_if.trace_reset(self._tr_id)


    def get_signals(self):

        return self._ocp_if.trace_get_signals_names(self._tr_id)


    def get_signals_dict(self):

        status, signals = self._ocp_if.trace_get_signals_names(self._tr_id)
        if status < 0:
            return (-1, status)

        signals = dict(enumerate(signals))

        return (0, signals)

    
    def get_size(self):

        status, tr_size = self._ocp_if.trace_get_size(self._tr_id)
        if status < 0:
            return (-1, status)

        status, n_signals = self._ocp_if.trace_get_number_signals(self._tr_id)
        if status < 0:
            return (-1, status)

        size = tr_size / n_signals / _TRACE_DATA_SIZE_BYTES

        return (0, size)
        

    def set_size(self, size):

        status, n_signals = self._ocp_if.trace_get_number_signals(self._tr_id)
        if status < 0:
            return (-1, status)

        tr_size = size * n_signals * _TRACE_DATA_SIZE_BYTES

        return self._ocp_if.trace_set_size(self._tr_id, tr_size)

    
    def set_mode(self, mode):

        return self._ocp_if.trace_set_mode(self._tr_id, mode)


    def get_mode(self):

        return self._ocp_if.trace_get_mode(self._tr_id)


    def set_n_pre_trig_samples(self, n):

        return self._ocp_if.trace_set_num_pre_trig_samples(self._tr_id, n)


    def get_n_pre_trig_samples(self):

        return self._ocp_if.trace_get_num_pre_trig_samples(self._tr_id)
    

    def set_trig_signal(self, signal):

        return self._ocp_if.trace_set_trig_signal(self._tr_id, signal)


    def get_trig_signal(self):

        return self._ocp_if.trace_get_trig_signal(self._tr_id)


    def set_trig_level(self, level):

        return self._ocp_if.trace_set_trig_level(self._tr_id, level)


    def get_trig_level(self):

        return self._ocp_if.trace_get_trig_level(self._tr_id)
    

    def _reorder_data(self, data):

        status, tail = self._ocp_if.trace_get_tail(self._tr_id)
        if status < 0:
            return (-1, status)

        data_size = _TRACE_DATA_SIZE_BYTES
        data_ro = data[data_size*tail:] + data[:data_size*tail]

        return (0, data_ro)


    def _decode(self, data_bin):

        status, n_signals = self._ocp_if.trace_get_number_signals(self._tr_id)
        if status < 0:
            return (-1, status)

        n = len(data_bin)
        
        fmt = '<' + 'f' * round(n / _TRACE_DATA_SIZE_BYTES)

        data = np.array(struct.unpack(fmt, data_bin))

        data = data.reshape(round(n / n_signals / _TRACE_DATA_SIZE_BYTES), n_signals)

        return (status, data)
