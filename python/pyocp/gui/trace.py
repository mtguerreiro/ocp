import time
import threading
            
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

from dataclasses import dataclass

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

import pyocp


class RepeatTimer(threading.Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)


class _OcpTrace(pyocp.trace.TraceTemplate):

    def __init__(self, ocp_if, tr_id=0):

        super().__init__(tr_id=tr_id, ocp_if=ocp_if)


@dataclass
class _ConnType:
    ocp : pyocp.ocp.Interface
    trace : _OcpTrace
    tbf : list


class TraceBarFrame(tk.Frame):

    def __init__(self, parent, trace='', ctrl='', tr_if=0, *args, **options):
        tk.Frame.__init__(self, parent, *args, **options)

        self.parent = parent

        self.tr_if = tr_if

        self.show = tk.IntVar()
        self.show.set(0)

        self.plot_windows = {}

        self.title_frame = ttk.Frame(self)
        self.title_frame.pack(fill='x', expand=1)

        ttk.Label(self.title_frame, text=trace, width=20).pack(side='left', fill='x', expand=1, pady=2, padx=2, anchor='n')
        ttk.Label(self.title_frame, text=ctrl, width=20).pack(side='left', fill='x', expand=1, pady=2, padx=2, anchor='n')

        # Settings button
        self.toggle_button = ttk.Checkbutton(self.title_frame, width=1, text='+', command=self.toggle,
                                            variable=self.show, style='Toolbutton')
        self.toggle_button.pack(side='left')

        # Start button
        self.start = tk.IntVar()
        self.start.set(0)
        self.start_button = ttk.Checkbutton(self.title_frame, width=6, text='Start', command=self.acq_start,
                                            variable=self.start, style='Toolbutton')
        self.start_button.pack(side='left')

        # New plot button
        self.new_plot_button = ttk.Button(self.title_frame, width=8, text='New plot', command=self.new_plot, style='Toolbutton')
        self.new_plot_button.pack(side='left')
        
        self.sub_frame = tk.Frame(self, relief='sunken', borderwidth=1)

        row = 0
        
        # Size
        status, size = self.tr_if.get_size()
        if status < 0:
            raise RuntimeError("Failed to read the size of the trace.")
        size = int(size)
        ttk.Label(self.sub_frame, text='Size (samples)', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.size_entry = ttk.Entry(self.sub_frame, width=10)
        self.size_entry.insert(0, size)
        self.size_entry.bind('<Return>', self.update_size)
        self.size_entry.bind('<FocusOut>', self.update_size)
        self.size_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        
        # Trace mode
        status, mode = self.tr_if.get_mode()
        if status < 0:
            raise RuntimeError("Failed to read the mode of the trace.")
        mode = int(mode)
        row = row + 1
        self.trace_mode = tk.IntVar(master=self.sub_frame, value=0)
        self.trace_mode.set(mode)
        ttk.Label(self.sub_frame, text='Trace mode', width=15).grid(row=row, column=0, padx=10, pady=2)
        ttk.Radiobutton(self.sub_frame, text='Manual', variable=self.trace_mode, value=0, command=self.update_trace_mode).grid(row=row, column=1, padx=10, sticky='w')
        ttk.Radiobutton(self.sub_frame, text='Trigger', variable=self.trace_mode, value=1, command=self.update_trace_mode).grid(row=row, column=2, padx=10, sticky='w')

        # Acquisition
        row = row + 1
        self.acq_mode = tk.IntVar(master=self.sub_frame, value=0)
        ttk.Label(self.sub_frame, text='Acquisition mode', width=15).grid(row=row, column=0, padx=10, pady=2)
        ttk.Radiobutton(self.sub_frame, text='One-time', variable=self.acq_mode, value=0, command=self.update_acq_mode).grid(row=row, column=1, padx=10, sticky='w')
        ttk.Radiobutton(self.sub_frame, text='Continuous', variable=self.acq_mode, value=1, command=self.update_acq_mode).grid(row=row, column=2, padx=10, sticky='w')
        
        # Refresh rate
        row = row + 1
        ttk.Label(self.sub_frame, text='Refresh rate (ms)', width=15).grid(row=row, column=0, padx=10, pady=2)
        self.refresh_rate_entry = ttk.Entry(self.sub_frame, width=10)
        self.refresh_rate_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        self.refresh_rate_entry.bind('<Return>', self.update_refresh_rate)
        self.refresh_rate_entry.bind('<FocusOut>', self.update_refresh_rate)
        
        # Trigger
        row = row + 1
        self.trigger = tk.StringVar(master=self.sub_frame)
        ttk.Label(self.sub_frame, text='Trigger', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.trigger_option_menu = ttk.OptionMenu(self.sub_frame, self.trigger, '')
        self.trigger_option_menu.grid(row=row, column=1, columnspan=2, padx=10, pady=2, sticky='ew') 
        self.trigger_option_menu.bind('<Button>', self.update_trigger_list)
        self.update_trigger_list(0)
        
        # Pretrig samples
        status, n = self.tr_if.get_n_pre_trig_samples()
        if status < 0:
            raise RuntimeError("Failed to get the number of pre trig samples.")        
        row = row + 1
        ttk.Label(self.sub_frame, text='Pre trig. samples', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.pre_trig_samples_entry = ttk.Entry(self.sub_frame, width=10)
        self.pre_trig_samples_entry.insert(0, n)
        self.pre_trig_samples_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        self.pre_trig_samples_entry.bind('<Return>', self.update_pre_trig_samples)
        self.pre_trig_samples_entry.bind('<FocusOut>', self.update_pre_trig_samples)

        
    def toggle(self):
        if bool(self.show.get()):
            self.sub_frame.pack(fill='x', expand=1)
            self.toggle_button.configure(text='-')
        else:
            self.sub_frame.forget()
            self.toggle_button.configure(text='+')


    def acq_start(self):
        if bool(self.start.get()):
            #self.sub_frame.pack(fill='x', expand=1)
            self.start_button.configure(text='Stop')
            self.timer = RepeatTimer(3, self.update_plot_windows)
            self.timer.start()
        else:
            #self.sub_frame.forget()
            self.start_button.configure(text='Start')
            self.timer.cancel()

    
    def update_size(self, event):

        size = self.size_entry.get()
        
        if not size.isdigit():
            messagebox.showerror('Input Error', 'Size must be a number.')
            return

        status, = self.tr_if.set_size( int(size) )    

        if status < 0:
            messagebox.showerror('Error', 'Failed to update size.')
            

    def update_trace_mode(self):

        mode = self.trace_mode.get()
        status, = self.tr_if.set_mode( mode )

        if status < 0:
            messagebox.showerror('Error', 'Failed to update size.')

    
    def update_acq_mode(self):
        print('new acq mode: {:}'.format(self.acq_mode.get()))


    def update_refresh_rate(self, event):
        print('new refresh rate: {:}'.format(self.refresh_rate_entry.get()))


    def update_trigger(self):
        
        trigger_str = str(self.trigger.get())
        trigger_idx = trigger_str.split('-')[0].strip()

        status, = self.tr_if.set_trig_signal( int(trigger_idx) )
        if status < 0:
            messagebox.showerror('Error', 'Failed to set trigger.')
            return
        

    def update_trigger_list(self, event):

        # Trigger selected in the hw
        status, trigger_hw = self.tr_if.get_trig_signal()
        if status < 0:
            messagebox.showerror('Error', 'Failed to update the list of triggers: could not read trigger.')
            return

        # List of signals in the hw
        status, signals = self.tr_if.get_signals_dict()
        if status < 0:
            messagebox.showerror('Error', 'Failed to update the list of triggers: could not read signals.')
            return

        # Creates new list, selecting the trigger previously set
        self.trigger_options = []
        for key, val in signals.items():
            self.trigger_options.append('{:} - {:}'.format(key, val.decode()))

        menu = self.trigger_option_menu['menu']
        menu.delete(0, tk.END)
        for opt in self.trigger_options:
            menu.add_radiobutton(label=opt, command=self.update_trigger, variable=self.trigger)

        self.trigger.set(self.trigger_options[trigger_hw])

        
    def update_pre_trig_samples(self, event):

        n = self.pre_trig_samples_entry.get()
        
        if not n.isdigit():
            messagebox.showerror('Input Error', 'Number of samples must be a number.')
            return

        status, = self.tr_if.set_n_pre_trig_samples( int(n) )    

        if status < 0:
            messagebox.showerror('Error', 'Failed to update number of pre trig samples.')


    def new_plot(self):
        new_window = tk.Toplevel()
        new_window.title("New Plot Window")
        tww = TraceWaveformWindow(new_window, self.trigger_options)
        self.plot_windows[tww] = [tww, new_window]
        new_window.protocol("WM_DELETE_WINDOW", lambda w=tww:self.new_plot_close(w))
        print('adding window', tww)


    def new_plot_close(self, tww):
        print('destroying window', tww)
        self.plot_windows[tww][1].destroy()
        del self.plot_windows[tww]

    def update_plot_windows(self):
    
        status, data = self.tr_if.read()
        for t in self.plot_windows:
            self.plot_windows[t][0].update_waveforms(data)
        self.tr_if.reset()
        
        
@dataclass
class _TWWData:
    label : str
    data : np.ndarray
    x_data : np.ndarray
    visible : bool

class TraceWaveformWindow:
    def __init__(self, parent, signals):
        # Create a frame to hold the waveform controls and the plot
        self.idx_dict = {}
        self.signals = signals
        self.parent = parent
        self.frame = ttk.Frame(parent)
        self.frame.pack(fill=tk.BOTH, expand=True)

        # Create a frame for waveform controls
        self.control_frame = ttk.Frame(self.frame)
        self.control_frame.pack(side=tk.TOP, fill=tk.X)

        # Initialize the plot
        self.fig, self.ax = plt.subplots()
        self.ax.set_title("Waveform Plot")
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Amplitude")

        # Add the matplotlib canvas to the frame
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas_widget = self.canvas.get_tk_widget()
        self.canvas_widget.pack(fill=tk.BOTH, expand=True)

        # Storage for waveforms
        self.waveforms = []

        # Menu bar setup
        self.menu_bar = tk.Menu(parent)
        parent.config(menu=self.menu_bar)

        self.waveform_menu = tk.Menu(self.menu_bar, tearoff=0)
        self.menu_bar.add_cascade(label='Waveforms', menu=self.waveform_menu)
        self.waveform_menu.add_command(label='Add', command=self.add_new_waves)
        self.waveform_menu.add_separator()
        self.waveform_menu.add_command(label='Clear all', command=self.clear_waves)


    def add_new_waves(self):

        x = tk.StringVar()
        y = tk.StringVar()

        # Function to handle the "Add" button
        def add():
            print('add')
            print(f"x-data selected: {x.get()}")
            print(f"y-data selected: {y.get()}")
            self.add_waveform([], y.get())
            self.update_signals_list( [y.get()] )
            
            new_window.destroy()

        new_window = tk.Toplevel(self.parent)
        new_window.title('Add waveforms')
        new_window.grab_set()

        # X-data selection (optional)
        tk.Label(new_window, text='x-data (optional):').grid(row=0, column=0, padx=10, pady=10)
        x_data_dropdown = tk.OptionMenu(new_window, x, '', *self.signals)
        x_data_dropdown.grid(row=0, column=1, padx=10, pady=10)

        # Y-data selection (multiple options)
        tk.Label(new_window, text='y-data:').grid(row=1, column=0, padx=10, pady=10)
        y_data_dropdown = tk.OptionMenu(new_window, y, '', *self.signals)
        y_data_dropdown.grid(row=1, column=1, padx=10, pady=10)

        # Add button
        add_button = tk.Button(new_window, text='Add', command=add)
        add_button.grid(row=2, column=1, padx=10, pady=10, sticky='e')
        new_window.wait_window()


    def update_signals_list(self, signals):

        for s in signals:
            idx = int( s.split('-')[0].strip() )
            if idx not in self.idx_dict:
                self.idx_dict[idx] = _TWWData(label=s, data=0, x_data=0, visible=True)

        print(self.idx_dict)

        
    def update_waveforms(self, data):
        for idx in self.idx_dict:
            self.idx_dict[idx].data = data[:, idx]
            
        self.ax.clear()
        self.ax.set_title("Waveform Plot")
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Amplitude")

        for idx in self.idx_dict:
            if self.idx_dict[idx].visible:
                self.ax.plot(self.idx_dict[idx].data, label=self.idx_dict[idx].label)
        if any(self.idx_dict[idx].visible for idx in self.idx_dict):
            self.ax.legend()
        self.canvas.draw()
        
##        for wave in self.waveforms:
##            if wave["visible"]:
##                self.ax.plot(wave["y"], label=wave["label"])
##        if any(wave["visible"] for wave in self.waveforms):
##            self.ax.legend()
##        self.canvas.draw()
        

    def add_sine_wave(self):
        sine_wave = np.sin(self.x_data)
        self.add_waveform(sine_wave, 'Sine Wave')

    def add_cosine_wave(self):
        cosine_wave = np.cos(self.x_data)
        self.add_waveform(cosine_wave, 'Cosine Wave')

    def add_waveform(self, y_data, label):
        waveform = {"y": y_data, "label": label, "visible": True, "var": tk.IntVar(value=1)}
        self.waveforms.append(waveform)
        self.update_waveform_controls()
        self.update_plot()

    def clear_waves(self):
        self.waveforms.clear()
        for widget in self.control_frame.winfo_children():
            widget.destroy()
        self.update_plot()

    def update_waveform_controls(self):
        for widget in self.control_frame.winfo_children():
            widget.destroy()

        # Create checkbuttons for waveforms
        for i, wave in enumerate(self.waveforms):
            label = wave["label"][:15] + ("..." if len(wave["label"]) > 15 else "")
            cb = ttk.Checkbutton(
                self.control_frame,
                text=label,
                variable=wave["var"],
                command=self.update_visibility
            )
            cb.grid(row=i // 3, column=i % 3, sticky=tk.W, padx=5, pady=5)

    def update_visibility(self):
        for wave in self.waveforms:
            wave["visible"] = bool(wave["var"].get())
        self.update_plot()

    def update_plot(self):
        self.ax.clear()
        self.ax.set_title("Waveform Plot")
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Amplitude")
        for wave in self.waveforms:
            if wave["visible"]:
                self.ax.plot(wave["y"], label=wave["label"])
        if any(wave["visible"] for wave in self.waveforms):
            self.ax.legend()
        self.canvas.draw()


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

        # Function to handle the "Connect" button
        def connect():
            host = ip_entry.get().strip()
            port = port_entry.get().strip()

            # Validate the IP and Port
            if not host:
                messagebox.showerror("Input Error", "IP address cannot be empty.")
                new_window.destroy()
                return

            if not port.isdigit():
                messagebox.showerror("Input Error", "Port must be a number.")
                new_window.destroy()
                return
            
            settings = {'host':host, 'port':int(port)}

            conn = '{:}:{:}'.format(host, port)
            if conn not in self.connections:
                self.connections[conn] = _ConnType
                self.connections[conn].tbf = []
                self.connections[conn].traces = []
            else:
                messagebox.showerror("Host error", "Host already connected.")
                new_window.destroy()
                return                

            self.connections[conn].ocp = pyocp.ocp.Interface(comm_type='ethernet', settings=settings)

            try:
                status, traces = self.connections[conn].ocp.trace_get_traces_names()
        
                if status < 0:
                    print('Error reading traces names')
                    del self.connections[conn]
                    new_window.destroy()  # Close the pop-up
                    return

                for n, tr in enumerate(traces):
                    self.connections[conn].traces.append(_OcpTrace(self.connections[conn].ocp, tr_id=n) )
                    tbf = TraceBarFrame(self.trace_frame, trace=tr, tr_if = self.connections[conn].traces[n], ctrl=host, relief="raised", borderwidth=1)
                    tbf.grid(row=self.n_traces, column=0, pady=5)
                    self.connections[conn].tbf.append(tbf)
                    self.n_traces += 1

                self.menu_connection_remove_update()
            
            except:
                messagebox.showerror("Host error", "Could not reach host.")
                del self.connections[conn]

            new_window.destroy()

        new_window = tk.Toplevel(self.root)
        new_window.title('New Connection')
        new_window.grab_set()

        ttk.Label(new_window, text='IP:').grid(row=0, column=0, padx=10, pady=5)
        ip_entry = ttk.Entry(new_window)
        ip_entry.insert(tk.END, 'localhost')
        ip_entry.grid(row=0, column=1, padx=10, pady=5)

        ttk.Label(new_window, text='Port:').grid(row=1, column=0, padx=10, pady=5)
        port_entry = ttk.Entry(new_window)
        port_entry.insert(tk.END, '8080')
        port_entry.grid(row=1, column=1, padx=10, pady=5)

        # Connect Button
        ttk.Button(new_window, text='Connect', command=connect).grid(row=2, column=0, columnspan=2, pady=10)

        new_window.wait_window()

    
    def menu_connection_remove(self, conn):
        if conn in self.connections:
            for tbf in self.connections[conn].tbf:
                tbf.destroy()
            del self.connections[conn]
            self.menu_connection_remove_update()


    def menu_connection_remove_update(self):
        self.rm_menu.delete(0, tk.END)
        for conn in self.connections:
            self.rm_menu.add_command(label=conn, command=lambda c=conn: self.menu_connection_remove(c))
        


if __name__ == "__main__":

    MainWindow()
