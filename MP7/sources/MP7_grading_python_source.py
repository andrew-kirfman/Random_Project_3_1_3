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
		
	A quick guide to some unclear variable names:
		Variables like R3_gained refer to whether the student has won or lost
		the points corresponding to the rubric item referred to by the alphanumeric
		code (in the above case, R3 for "3rd item for Request Threads"). Here is a list
		of the rubric item codes (note that not all of them are used in the script, 
		the ones that don't appear are evaluated elsewhere):
		Request Threads:
			R1. 3 request threads created in main.
			R2. Only one request thread function used.
			R3. All request threads joined before any worker threads start. - coded
		Worker Threads:
			W1. All necessary quit requests pushed from main.
			W2. Final histogram is correct.
			W3. Thread-safety
				W3.1. SafeBuffer used for request_buffer, and implementation is correct.
				W3.2. *frequency_count vectors are properly mutexed.
		Globals:
			G. No global variables used
		Cleanup:
			C1. No memory leaks.
			C2. No leftover FIFOs.
		Report:
			N/A, must be hand-graded (rubric in MP-handout)
			
	To clear up the use of breakpoint list indicies:
		This script creates breakpoints in a tightly-controlled order,
		so that they will always have the same index in gdb.breakpoints()
		once they have been created.
		For reference:
		gdb.breakpoints()[0] - request_thread_function
		gdb.breakpoints()[1] - worker_thread_function
		gdb.breakpoints()[2] - display_histograms
		gdb.breakpoints()[3] - -location request_buffer.data
"""

rtf_hitcount = 0
wtf_hitcount = 0

def stop_handler (event):
	try:
		if event.__class__.__name__ == "BreakpointEvent":
			for bp in event.breakpoints:
				print("Hit breakpoint at " + str(bp.location))
				if bp.location == "request_thread_function":
					rtf_hitcount = bp.hit_count
					if bp.hit_count == 1:
						gdb.execute("thread 1")
						gdb.execute("up 99999")
						gdb.execute("watch -l request_buffer.data")
						if gdb.execute("whatis request_buffer", False, True) == "type = SafeBuffer\n":
							W3_1_gained = True
							print("Student gained points for W3.1!")
							"""
								Odds are that the student will lose a lot more points than
								just W3.1 if they don't use the SafeBuffer class.
								
								Anyway, check here that request_buffer.data.size is 0.
								Can't have them pushing requests and stuff before the
								data watchpoint is added, now can we?
							"""
						else:
							W3_1_gained = False
							print("Student lost points for W3.1...")
		
				elif bp.location == "worker_thread_function":
					wtf_hitcount = bp.hit_count
					if bp.hit_count == 1:
						if len(gdb.inferiors()[0].threads()) == 2:
							R3_gained = True
							print("Student gained points for R3!")
						else:
							R3_gained = False
							print("Student lost points for R3...")
	
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
		display_break = gdb.Breakpoint("display_histograms", gdb.BP_BREAKPOINT, 0, False, False)
		gdb.execute("run -v 3 -w 20 -n 100")
		gdb.execute("quit")
	except TypeError:
		traceback.print_exc()
		gdb.execute("quit")
	
main()
