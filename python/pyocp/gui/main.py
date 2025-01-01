import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

from dataclasses import dataclass

import pyocp

@dataclass
class _Conn:
    host : str
    port : int
    tb : list


class _NewConnectionDialog:

    def __init__(self, parent):

        self.host = None
        self.port = None
        
        self.window = tk.Toplevel(parent)
        self.window.title('New Connection')
        self.window.grab_set()

        ttk.Label(self.window, text='IP:').grid(row=0, column=0, padx=10, pady=5)
        self.ip_entry = ttk.Entry(self.window)
        self.ip_entry.insert(tk.END, 'localhost')
        self.ip_entry.grid(row=0, column=1, padx=10, pady=5)

        ttk.Label(self.window, text='Port:').grid(row=1, column=0, padx=10, pady=5)
        self.port_entry = ttk.Entry(self.window)
        self.port_entry.insert(tk.END, '8080')
        self.port_entry.grid(row=1, column=1, padx=10, pady=5)

        # Connect Button
        ttk.Button(self.window, text='Connect', command=self.connect).grid(row=2, column=0, columnspan=2, pady=10)

        self.window.wait_window()
        

    def connect(self):

        host = self.ip_entry.get().strip()
        port = self.port_entry.get().strip()

        # Validate the IP and Port
        if not host:
            messagebox.showerror("Input Error", "IP address cannot be empty.")
            return

        if not port.isdigit():
            messagebox.showerror("Input Error", "Port must be a number.")
            return

        self.host = host
        self.port = port
        self.window.destroy()

        
class MainWindow:
    
    def __init__(self):

        self.root = tk.Tk()

        self.menubar = tk.Menu(self.root)
        self.root.config(menu=self.menubar)

        # File menu
        self.conn_menu = tk.Menu(self.menubar, tearoff=0)
        self.menubar.add_cascade(label='Connection', menu=self.conn_menu)

        self.conn_menu.add_command(label='New...', command=self.menu_connection_new)

        self.rm_menu = tk.Menu(self.conn_menu, tearoff=0)
        self.conn_menu.add_cascade(label="Remove", menu=self.rm_menu)

        # Traces area
        self.root.columnconfigure(0, weight=1, minsize=250)
        self.root.rowconfigure(0, weight=1, minsize=50)
        self.trace_frame = ttk.LabelFrame(self.root, text='Traces', relief='sunken', borderwidth=1)
        self.trace_frame.grid(row=0, column=0, padx=10, pady=10, sticky="news")

        self.n_traces = 0
        self.connections = {}

        self.root.mainloop()

        
    def menu_connection_new(self):

        ncd = _NewConnectionDialog(self.root)
        if (not ncd.host) and (not ncd.port):
            return

        host = ncd.host
        port = int(ncd.port)

        self.menu_connection_new_add_traces(host, port)


    def menu_connection_new_add_traces(self, host, port):

        conn = '{:}:{:}'.format(host, port)
        
        if conn in self.connections:
            messagebox.showerror("Host error", "Host already connected.")
            return

        ocp = pyocp.ocp.Interface(comm_type='ethernet', settings={'host':host, 'port':port})

        status, traces = ocp.trace_get_traces_names()
        if status < 0:
            print('Error reading traces names. Try adding it again.')
            del self.connections[conn]
            return

        self.connections[conn] = _Conn(host=host, port=port, tb=[])

        self.trace_frame.columnconfigure(0, weight=1)
        for n, tr in enumerate(traces):
            tb = pyocp.gui.trace.Bar(
                self.trace_frame, ocp=ocp, tr_id=n, tr_name=tr, ctrl_name=host,
                relief="raised", borderwidth=1)
            tb.grid(row=self.n_traces, column=0, pady=5, sticky='ew')
            self.trace_frame.rowconfigure(self.n_traces, weight=1)
            self.connections[conn].tb.append(tb)
            self.n_traces += 1

        self.menu_connection_remove_update()

        
    def menu_connection_remove(self, conn):
        if conn in self.connections:
            for t in self.connections[conn].tb:
                t.destroy()
            del self.connections[conn]
            self.menu_connection_remove_update()


    def menu_connection_remove_update(self):
        self.rm_menu.delete(0, tk.END)
        for conn in self.connections:
            self.rm_menu.add_command(label=conn, command=lambda c=conn: self.menu_connection_remove(c))
        
        
if __name__ == "__main__":

    MainWindow()
