import lrssoc

import buck

host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

ocp = lrssoc.ocp.iface.Interface(comm_type='ethernet', settings=settings)

def data_controller(ts=2e-3, os=10, dt=1/100e3, v_ref=6):

    ki, kv, k_ev = buck.get_gains(2e-3, 10)
    data_bin = buck.format_params(ki, kv, k_ev, dt, v_ref)

    return data_bin
    
    
