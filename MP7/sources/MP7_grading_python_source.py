import sys
import gdb
from subprocess import call

def stop_handler (event):
    if event.__class__.__name__ == "BreakpointEvent":
        for bp in event.breakpoints:
            print("Hit breakpoint at " + bp.location)
    gdb.execute("cont")

rtf_break = gdb.Breakpoint("request_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
wtf_break = gdb.Breakpoint("worker_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
gdb.events.stop.connect (stop_handler)
gdb.execute("run -v 3 -w 40 -n 100")
gdb.execute("quit")