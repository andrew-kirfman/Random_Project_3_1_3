import sys
import gdb
import traceback
from subprocess import call
"""
	My wish list for this program:
		1. Find a more elegant way to select outermost stack frame.
			-> regular expressions, along with to_string parameter of gdb.execute?
		2. Take arguments for run instead of hard-coding them.
		3. Record grading-relevant final results cleanly and efficiently
		4. Error-checked
		5. Access and observe request_buffer directly from Python,
		rather than parsing enormous amounts of program output.
"""
def stop_handler (event):
	try:
		print ("event is something like " + event.__class__.__name__)
		if event.__class__.__name__ == "BreakpointEvent":
			for bp in event.breakpoints:
				print("Hit breakpoint at " + str(bp.location))
				if bp.location == "request_thread_function" and bp.hit_count == 1:
					""" Wish List #1 """
					gdb.execute("thread 1")
					gdb.execute("up 99999")
					""" ------------ """
					if gdb.execute("whatis request_buffer", False, True) == "type = SafeBuffer\n":
						""" Wish List #3 """
						W3_1 = True
						print("Student gained points for W3.1!")
					else:
						W3_1 = False
						print("Student lost points for W3.1...")
						""" ------------ """
					"""
						Setting the data watchpoint is of critical importance,
						but I haven't been able to get it to work in python.
						To work it HAS to be a location watchpoint, since
						most of the relevant modifications take place in threads
						where request_buffer is not in scope. However, setting
						a location watchpoint using the Breakpoint constructor
						fails with "no symbol -l in current context", and setting it
						directly using gdb.execute works but raises a TypeError exception,
						supposedly from trying to convert a NoneType to a str.
						What's going on here?
					"""
					gdb.execute("watch -l request_buffer.data")
		print("Un-breaking/continuing...")
		gdb.execute("cont")
	except TypeError:
		traceback.print_exc()
		gdb.execute("quit")

def main():
	try:
		gdb.events.stop.connect (stop_handler)
		rtf_break = gdb.Breakpoint("request_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
		wtf_break = gdb.Breakpoint("worker_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
		""" Wish List #2 """
		gdb.execute("run -v 3 -w 20 -n 100")
		""" ------------ """
		gdb.execute("quit")
	except TypeError:
		traceback.print_exc()
		gdb.execute("quit")
	
main()
