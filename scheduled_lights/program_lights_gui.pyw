import ctypes
import tkinter as tk
import tkinter.ttk as ttk
import datetime as dt
import os
from threading import Thread
import subprocess as sp
from time import sleep


fname = 'scheduled_lights'
serial_port = 'COM3'
fqbn = 'm5stack:esp32:m5stack-atom'  # "fully-qualified board name"


ctypes.windll.shcore.SetProcessDpiAwareness(1)


window = tk.Tk()
window.title('')
window.configure(bg='white')
window.resizable(width=False, height=False)


ttk.Label(text='Scheduled Lights Programmer', background='white') \
   .grid(row=0, padx=40, pady=10, sticky='w')

ttk.Label(text='On time:', background='white') \
   .grid(row=1, padx=40, pady=10, sticky='w')
on_time_entry = ttk.Entry()
on_time_entry.insert(index=0, string='6:00 AM')
on_time_entry.grid(row=2, padx=40, sticky='w')

ttk.Label(text='Off time:', background='white') \
   .grid(row=3, padx=40, pady=10, sticky='w')
off_time_entry = ttk.Entry()
off_time_entry.insert(index=0, string='8:00 PM')
off_time_entry.grid(row=4, padx=40, sticky='w')


def program_lights():

    on_time = dt.datetime.strptime(on_time_entry.get(), '%I:%M %p').time()
    off_time = dt.datetime.strptime(off_time_entry.get(), '%I:%M %p').time()

    today = dt.date.today()
    midnight = dt.datetime.combine(today, dt.time.min)

    now_datetime = dt.datetime.now()
    on_datetime = dt.datetime.combine(today, on_time)
    off_datetime = dt.datetime.combine(today, off_time)

    hours_on = (off_datetime - on_datetime).seconds / 3600
    hours_off = 24 - hours_on

    if midnight <= now_datetime < on_datetime:
        hours_on_pre = 0
        hours_off_pre = (on_datetime - now_datetime).seconds / 3600
    elif on_datetime <= now_datetime < off_datetime:
        hours_on_pre = (off_datetime - now_datetime).seconds / 3600
        hours_off_pre = hours_off
    elif off_datetime <= now_datetime < midnight + dt.timedelta(days=1):
        hours_on_pre = 0
        hours_off_pre = 24 - (now_datetime - on_datetime).seconds / 3600

    print(f'hours_on_pre = {hours_on_pre}')
    print(f'hours_off_pre = {hours_off_pre}')
    print(f'hours_on = {hours_on}')
    print(f'hours_off = {hours_off}')
    print(os.path.dirname(__file__))
    with open(f'{os.path.dirname(__file__)}\\{fname}_template.ino.txt', 'r') as file:
        lines = file.readlines()

    contents = ''.join(lines)

    new_contents = contents.replace('<hours_on_pre>', str(hours_on_pre)) \
                           .replace('<hours_off_pre>', str(hours_off_pre)) \
                           .replace('<hours_off>', str(hours_off)) \
                           .replace('<hours_on>', str(hours_on))

    with open(f'{os.path.dirname(__file__)}\\{fname}.ino', 'w') as file:
        file.write(new_contents)

    compiling_label = ttk.Label(background='white')
    compiling_label.config(text='Program compiling...')
    compiling_label.grid(row=6, padx=40, sticky='w')
    compiling_progress_bar = ttk.Progressbar(mode='indeterminate', length=335)
    compiling_progress_bar.start(10)
    compiling_progress_bar.grid(row=7, padx=40, pady=40, sticky='w')

    def compile_program():

        sp.run(f'arduino-cli compile --fqbn {fqbn} "{os.path.dirname(__file__)}"', shell=True)

        compiling_label.config(text='Program compiled!')
        compiling_progress_bar = ttk.Progressbar(mode='determinate', length=335)
        compiling_progress_bar['value'] = 100
        compiling_progress_bar.grid(row=7, padx=40, pady=40, sticky='w')

        uploading_label = ttk.Label(background='white')
        uploading_label.config(text='Program uploading...')
        uploading_label.grid(row=8, padx=40, sticky='w')
        uploading_progress_bar = ttk.Progressbar(mode='indeterminate', length=335)
        uploading_progress_bar.start(10)
        uploading_progress_bar.grid(row=9, padx=40, pady=40, sticky='w')

        def upload_program():

            sleep(1)
            sp.run(f'arduino-cli upload --port {serial_port} --fqbn {fqbn} "{os.path.dirname(__file__)}"', shell=True)

            uploading_label.config(text='Program uploaded!')
            uploading_progress_bar = ttk.Progressbar(mode='determinate', length=335)
            uploading_progress_bar['value'] = 100
            uploading_progress_bar.grid(row=9, padx=40, pady=40, sticky='w')

        upload_program_thread = Thread(target=upload_program)
        upload_program_thread.start()

    compile_program_thread = Thread(target=compile_program)
    compile_program_thread.start()
  # compile_program_thread.join()


ttk.Button(text='Program Lights', command=program_lights, width=25) \
   .grid(row=5, padx=40, pady=40, sticky='w')


window.mainloop()
