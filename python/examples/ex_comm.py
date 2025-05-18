import pyocp

# --- Input ---
host = 'localhost'
port = 8080
settings = {'host':host, 'port':port}

# --- OCP object ---
ocp = pyocp.ocp.Interface(comm_type='ethernet', settings=settings)

