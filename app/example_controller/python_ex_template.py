import lrssoc

host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

ocp = lrssoc.ocp.iface.Interface(comm_type='ethernet', settings=settings)

