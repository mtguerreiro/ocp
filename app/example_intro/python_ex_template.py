import pyocp

host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

ocp = pyocp.ocp.Interface(comm_type='ethernet', settings=settings)

