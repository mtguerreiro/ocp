import time
import threading
            
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

from dataclasses import dataclass

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

import pyocp


class _RepeatTimer(threading.Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)


class _OcpTrace(pyocp.trace.TraceTemplate):

    def __init__(self, ocp_if, tr_id=0):

        super().__init__(tr_id=tr_id, ocp_if=ocp_if)


_ACQ_STATE_IDLE = 0
_ACQ_STATE_RUNNING = 1
_ACQ_STATE_STOP = 2
class _Acq:

    def __init__(self, trace, callback):

        self.trace = trace
        self.timer = _RepeatTimer(0, self.on_timer)
        self.callback = callback

        self.trace_mode = 0
        self.acq_mode = 0
        self.rrate = 0

        self._TRIG_SET = 4

        self.state = _ACQ_STATE_IDLE
        

    def set_acq_mode(self, mode):

        self.acq_mode = mode


    def set_trace_mode(self, mode):

        self.trace_mode = mode


    def set_refresh_rate(self, rate):

        self.rrate = rate
        

    def start(self, acq_mode=None, trace_mode=None, rrate=None):

        if acq_mode:
            self.acq_mode = acq_mode
        if trace_mode:
            self.trace_mode = trace_mode
        if rrate:
            self.rrate = rrate

        self.timer.cancel()
        #del self.timer
        self.timer = _RepeatTimer(self.rrate, self.on_timer)
        self.state = _ACQ_STATE_RUNNING
        self.trace.reset()
        self.timer.start()


    def stop(self):

        self.timer.cancel()
        self.state = _ACQ_STATE_IDLE
        

    def on_timer(self):
        #print('on timer')

        self.timer.cancel()
        #del self.timer

        if self.trace_mode:
            # trace mode is trigger
            status, trig_state = self.trace.get_trig_state()
            if trig_state != self._TRIG_SET:
                self.timer = _RepeatTimer(1, self.on_timer)
                self.timer.start()
                return

        if self.acq_mode == 0:
            self.state = _ACQ_STATE_STOP

        self.callback(self.state)

        if self.state == _ACQ_STATE_RUNNING:
            self.trace.reset()
            self.timer = _RepeatTimer(self.rrate, self.on_timer)
            self.timer.start()
                

class Bar(tk.Frame):

    def __init__(self, parent, ocp, tr_id=0, tr_name='', ctrl_name='', *args, **options):
        tk.Frame.__init__(self, parent, *args, **options)

        self.parent = parent

        self.trace = _OcpTrace(ocp, tr_id)

        self.plot_windows = []

        self.acq = _Acq(self.trace, self.update_plot_windows)

        self.title_frame = ttk.Frame(self)
        self.title_frame.pack(fill='x', expand=1)

        # Trace and controller names
        ttk.Label(self.title_frame, text=tr_name, width=20).pack(side='left', fill='x', expand=1, pady=2, padx=2, anchor='n')
        ttk.Label(self.title_frame, text=ctrl_name, width=20).pack(side='left', fill='x', expand=1, pady=2, padx=2, anchor='n')

        # Settings button
        self.settings_toggle_state = tk.IntVar()
        self.settings_toggle_state.set(0)
        self.settings_button = ttk.Checkbutton(
            self.title_frame, width=1, text='+', command=self.settings_toggle,
            variable=self.settings_toggle_state, style='Toolbutton')
        self.settings_button.pack(side='left')

        # Start button
        self.start = tk.IntVar()
        self.start.set(0)
        self.start_button = ttk.Checkbutton(self.title_frame, width=6, text='Start', command=self.acq_start,
                                            variable=self.start, style='Toolbutton')
        self.start_button.pack(side='left')

        # New plot button
        self.new_plot_button = ttk.Button(self.title_frame, width=8, text='New plot', command=self.new_plot, style='Toolbutton')
        self.new_plot_button.pack(side='left')

        # Sub frame that holds settings when expanded
        self.sub_frame = tk.Frame(self, relief='sunken', borderwidth=1)

        row = 0
        
        # Size
        ttk.Label(self.sub_frame, text='Size (samples)', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.size_entry = ttk.Entry(self.sub_frame, width=10)
        self.size_entry.bind('<Return>', self.update_size)
        self.size_entry.bind('<FocusOut>', self.update_size)
        self.size_entry.bind('<Leave>', self.update_size)
        self.size_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        
        # Trace mode
        row = row + 1
        self.trace_mode = tk.IntVar(master=self.sub_frame, value=0)
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
        ttk.Label(self.sub_frame, text='Refresh rate (s)', width=15).grid(row=row, column=0, padx=10, pady=2)
        self.refresh_rate_entry = ttk.Entry(self.sub_frame, width=10)
        self.refresh_rate_entry.insert(tk.END, '1')
        self.refresh_rate_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        self.refresh_rate_entry.bind('<Return>', self.update_refresh_rate)
        self.refresh_rate_entry.bind('<FocusOut>', self.update_refresh_rate)
        self.refresh_rate_entry.bind('<Leave>', self.update_refresh_rate)
        
        # Trigger
        row = row + 1
        self.trigger = tk.StringVar(master=self.sub_frame)
        ttk.Label(self.sub_frame, text='Trigger', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.trigger_option_menu = ttk.OptionMenu(self.sub_frame, self.trigger, '')
        self.trigger_option_menu.grid(row=row, column=1, columnspan=2, padx=10, pady=2, sticky='ew') 
        self.trigger_option_menu.bind('<Button>', self.update_trigger_list)
        
        # Trigger level
        row = row + 1
        ttk.Label(self.sub_frame, text='Trigger level', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.trigger_level_entry = ttk.Entry(self.sub_frame, width=10)
        self.trigger_level_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        self.trigger_level_entry.bind('<Return>', self.update_trigger_level)
        self.trigger_level_entry.bind('<FocusOut>', self.update_trigger_level)
        self.trigger_level_entry.bind('<Leave>', self.update_trigger_level)

        # Pretrig samples
        row = row + 1
        ttk.Label(self.sub_frame, text='Pre trig. samples', width=15).grid(row=row, column=0, padx=10, pady=2, sticky='w')
        self.pre_trig_samples_entry = ttk.Entry(self.sub_frame, width=10)
        self.pre_trig_samples_entry.grid(row=row, column=1, columnspan=2, padx=10, sticky='w')
        self.pre_trig_samples_entry.bind('<Return>', self.update_pre_trig_samples)
        self.pre_trig_samples_entry.bind('<FocusOut>', self.update_pre_trig_samples)
        self.pre_trig_samples_entry.bind('<Leave>', self.update_pre_trig_samples)
        
        self.update_widgets(0)
        #self.title_frame.bind('<Enter>', self.update_widgets)
        #self.sub_frame.bind('<Enter>', self.update_widgets)


    def update_widgets(self, event):

        # Size
        status, size = self.trace.get_size()
        if status < 0:
            raise RuntimeError("Failed to read the size of the trace.")
        size = int(size)
        self.size_entry.delete(0, tk.END)
        self.size_entry.insert(0, size)

        # Trace mode
        status, mode = self.trace.get_mode()
        if status < 0:
            raise RuntimeError("Failed to read the mode of the trace.")
        mode = int(mode)
        self.trace_mode.set(mode)

        # Trigger list
        self.update_trigger_list(0)

        # Trigger level
        status, level = self.trace.get_trig_level()
        if status < 0:
            raise RuntimeError("Failed to get the trigger level.")
        self.trigger_level_entry.delete(0, tk.END)
        self.trigger_level_entry.insert(0, level)

        status, n = self.trace.get_n_pre_trig_samples()
        if status < 0:
            raise RuntimeError("Failed to get the number of pre trig samples.")
        self.pre_trig_samples_entry.delete(0, tk.END)
        self.pre_trig_samples_entry.insert(0, n)

        self.update_widgets_visibility()


    def update_widgets_visibility(self):

        if bool( self.trace_mode.get() ):
            # Trace mode is trigger
            self.refresh_rate_entry.config(state='disabled')
            self.trigger_option_menu.config(state='enabled')
            self.trigger_level_entry.config(state='enabled')
            self.pre_trig_samples_entry.config(state='enabled')
        else:
            # Trace mode is manual
            self.refresh_rate_entry.config(state='enabled')
            self.trigger_option_menu.config(state='disabled')
            self.trigger_level_entry.config(state='disabled')
            self.pre_trig_samples_entry.config(state='disabled')

    
    def settings_toggle(self):
        if bool( self.settings_toggle_state.get() ):
            self.sub_frame.pack(fill='x', expand=1)
            self.settings_button.configure(text='-')
        else:
            self.sub_frame.forget()
            self.settings_button.configure(text='+')


    def acq_start(self):

        if bool( self.start.get() ):
            self.start_button.configure(text='Stop')
            acq_mode = self.acq_mode.get()
            trace_mode = self.trace_mode.get()
            try:
                rrate = float( self.refresh_rate_entry.get() )
            except:
                messagebox.showerror('Input Error', 'Refresh rate must be a number (int or float).')
                return
            self.acq.start(acq_mode=acq_mode, trace_mode=trace_mode, rrate=rrate)
        else:
            self.start_button.configure(text='Start')
            self.acq.stop()


    def update_size(self, event):

        size = self.size_entry.get()
        
        if not size.isdigit():
            messagebox.showerror('Input Error', 'Size must be an integer.')
            return

        status, = self.trace.set_size( int(size) )    

        if status < 0:
            messagebox.showerror('Error', 'Failed to update size.')
            

    def update_trace_mode(self):

        mode = self.trace_mode.get()
        status, = self.trace.set_mode( mode )

        if status < 0:
            messagebox.showerror('Error', 'Failed to update mode.')

        self.update_widgets_visibility()
        
        self.acq.set_trace_mode(mode)

    
    def update_acq_mode(self):

        mode = self.acq_mode.get()
        self.acq.set_acq_mode(mode)


    def update_refresh_rate(self, event):

        try:
            rrate = float( self.refresh_rate_entry.get() )
        except:
            messagebox.showerror('Input Error', 'Refresh rate must be a number (int or float).')
            return

        self.acq.set_refresh_rate(rrate)


    def update_trigger(self):
        
        trigger_str = str(self.trigger.get())
        trigger_idx = trigger_str.split('-')[0].strip()

        status, = self.trace.set_trig_signal( int(trigger_idx) )
        if status < 0:
            messagebox.showerror('Error', 'Failed to set trigger.')
            return
        

    def update_trigger_list(self, event):

        # Trigger selected in the hw
        status, trigger_hw = self.trace.get_trig_signal()
        if status < 0:
            messagebox.showerror('Error', 'Failed to update the list of triggers: could not read trigger.')
            return

        # List of signals in the hw
        status, signals = self.trace.get_signals_dict()
        if status < 0:
            messagebox.showerror('Error', 'Failed to update the list of triggers: could not read signals.')
            return

        # Creates new list, selecting the trigger previously set
        self.trace_signals = []
        for key, val in signals.items():
            self.trace_signals.append('{:} - {:}'.format(key, val.decode()))

        menu = self.trigger_option_menu['menu']
        menu.delete(0, tk.END)
        for opt in self.trace_signals:
            menu.add_radiobutton(label=opt, command=self.update_trigger, variable=self.trigger)

        self.trigger.set(self.trace_signals[trigger_hw])

        
    def update_pre_trig_samples(self, event):

        n = self.pre_trig_samples_entry.get()
        
        if not n.isdigit():
            messagebox.showerror('Input Error', 'Number of samples must be an integer.')
            return

        status, = self.trace.set_n_pre_trig_samples( int(n) )    

        if status < 0:
            messagebox.showerror('Error', 'Failed to update number of pre trig samples.')


    def update_trigger_level(self, event):

        level = self.trigger_level_entry.get()

        try:
            level = float(level)
        except:
            messagebox.showerror('Input Error', 'Level must be a number.')
            return

        status, = self.trace.set_trig_level( level )    

        if status < 0:
            messagebox.showerror('Error', 'Failed to update trigger level.')

            
    def new_plot(self):
        
        self.plot_windows.append( PlotWindow(self.trace_signals) )


    def update_plot_windows(self, acq_state):

        status, data = self.trace.read()
        for pw in self.plot_windows:
            pw.update_waveforms( data )

        if acq_state == _ACQ_STATE_STOP:
            # If acq ended, updates "Start" button
            mode = self.start.set(0)
            self.acq_start()


@dataclass
class _PlotWindowData:
    label : str
    data : np.ndarray
    x_data : np.ndarray
    visible : bool
    var : int


class _PlotWindowSelSignals:
    
    def __init__(self, parent, signals):

        self.x = None
        self.y = None
        
        self.signals = signals
        self.x_wfm = tk.StringVar()
        self.y_wfm = tk.StringVar()
        
        self.window = tk.Toplevel(parent)
        self.window.title('Add waveforms')
        self.window.grab_set()

        # X-data selection (optional)
        tk.Label(self.window, text='x-data (optional):').grid(row=0, column=0, padx=10, pady=10, sticky='w')
        x_data_dropdown = tk.OptionMenu(self.window, self.x_wfm, '', *self.signals)
        x_data_dropdown.configure(width=20, anchor='w')
        x_data_dropdown.grid(row=0, column=1, pady=10, sticky='ew')

        # Y-data selection (multiple options)
        tk.Label(self.window, text='y-data:').grid(row=1, column=0, padx=10, pady=10, sticky='w')
        self.y_data_lb = tk.Listbox(self.window, selectmode=tk.EXTENDED, height=4, width=20)
        for s in self.signals:
            self.y_data_lb.insert(tk.END, s)
        self.y_data_lb.grid(row=1, column=1, sticky='nsew')
        self.y_scrollbar = tk.Scrollbar(self.window, orient=tk.VERTICAL, command=self.y_data_lb.yview)
        self.y_scrollbar.grid(row=1, column=2, sticky="ns")
        self.y_data_lb.config(yscrollcommand=self.y_scrollbar.set)

        # Add button
        add_button = tk.Button(self.window, text='Add', command=self.add)
        add_button.configure(width=8)
        add_button.grid(row=2, column=1, padx=10, pady=10, sticky='e')

        self.window.wait_window()
        

    def add(self):

        selected_signals_idx = self.y_data_lb.curselection()
    
        self.x = self.x_wfm.get()
        self.y = selected_signals_idx
        
        self.window.destroy()


class PlotWindow:
    def __init__(self, signal_list):

        self.signal_list = signal_list
        
        self.signals_idx = {}
        self.signals = {}

        self.window = tk.Toplevel()
        self.window.title('Plot Window')        

        # Create a frame to hold the waveform controls and the plot
        self.frame = ttk.Frame(self.window)
        self.frame.pack(fill=tk.BOTH, expand=True)

        # Create a frame for waveform controls
        self.control_frame = ttk.Frame(self.frame)
        self.control_frame.pack(side=tk.TOP, fill=tk.X)

        # Initialize the plot
        set_matplotlib_theme()
        self.fig, self.ax = plt.subplots()
        self.ax.set_title('Waveform Plot')
        self.ax.set_xlabel('Time')
        self.ax.set_ylabel('Amplitude')

        # Add the matplotlib canvas to the frame
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas_widget = self.canvas.get_tk_widget()
        self.canvas_widget.pack(fill=tk.BOTH, expand=True)

        # Storage for waveforms
        self.waveforms = []

        # Menu bar setup
        self.menu_bar = tk.Menu(self.window)
        self.window.config(menu=self.menu_bar)

        self.waveform_menu = tk.Menu(self.menu_bar, tearoff=0)
        self.menu_bar.add_cascade(label='Waveforms', menu=self.waveform_menu)
        self.waveform_menu.add_command(label='Add', command=self.add_new_signals)
        self.waveform_menu.add_separator()
        self.waveform_menu.add_command(label='Clear all', command=self.clear_waves)


    def add_new_signals(self):

        sel = _PlotWindowSelSignals(self.window, self.signal_list)
        
        for idx in sel.y:
            if idx not in self.signals:
                label = self.signal_list[idx]
                self.signals[idx] = _PlotWindowData(label=label, data=[], x_data=[], visible=True, var=tk.IntVar(value=1))

        self.update_waveform_controls()
        self.update_plot()
                

    def update_waveform_controls(self):
        
        for widget in self.control_frame.winfo_children():
            widget.destroy()

        # Create checkbuttons for waveforms
        i = 0
        for signal in self.signals.values():
            label = signal.label[:15] + ("..." if len(signal.label) > 15 else "")
            cb = ttk.Checkbutton(
                self.control_frame,
                text=label,
                variable=signal.var,
                command=self.update_visibility
            )
            cb.grid(row=i // 3, column=i % 3, sticky=tk.W, padx=5, pady=5)
            i += 1

    
    def update_waveforms(self, data):
        
        for idx, sig in self.signals.items():
            sig.data = data[:, idx]
        
        self.update_plot()

    
    def clear_waves(self):
        
        self.signals = {}
        for widget in self.control_frame.winfo_children():
            widget.destroy()
        self.update_plot()


    def update_visibility(self):
        
        for sig in self.signals.values():
            sig.visible = bool( sig.var.get() )

        self.update_plot()
        

    def update_plot(self):
        
        self.ax.clear()
        self.ax.set_title('Waveform Plot')
        self.ax.set_xlabel('Time')
        self.ax.set_ylabel('Amplitude')
        for sig in self.signals.values():
            if sig.visible:
                self.ax.plot(sig.data, label=sig.label)
        if any(sig.visible for sig in self.signals.values()):
            self.ax.legend()
        #self.canvas.draw()
        self.canvas.draw_idle()
        self.canvas.flush_events()

def set_matplotlib_theme():

    title_fontsize = 12
    legend_fontsize = 10
    label_fontsize = 12
    tick_fontsize = 12

    line_width = 1.25

    # Theme
    try:
        plt.style.use('seaborn-v0_8-bright')
    except:
        plt.style.use('seaborn-bright')

    # Fonts
    matplotlib.rcParams['mathtext.fontset'] = 'cm'
    matplotlib.rcParams['font.family'] = 'serif'
    #matplotlib.rcParams['font.serif'] = 'CMU Serif'

    # Misc
    plt.rcParams['axes.unicode_minus'] = False

    # Grid
    plt.rcParams['axes.grid'] = True
    plt.rcParams['axes.grid.which'] = 'major'
    plt.rcParams['grid.linestyle'] = ':'
    plt.rcParams['grid.alpha'] = 0.7

    # Ticks
    plt.rcParams['xtick.labelsize'] = tick_fontsize
    plt.rcParams['ytick.labelsize'] = tick_fontsize

    # Lines
    plt.rcParams['lines.linewidth'] = line_width

    # Labels
    plt.rcParams['axes.labelsize'] = label_fontsize

    # Legend
    plt.rcParams['legend.fontsize'] = legend_fontsize

    # Title
    plt.rcParams['axes.titlesize'] = title_fontsize
    #plt.rcParams['figure.titlesize'] = title_fontsize
    #plt.rcParams['figure.titleweight'] = title_fontsize

    # Tight layout
    plt.rcParams['figure.autolayout'] = True
    plt.rcParams['savefig.bbox'] = 'tight'
