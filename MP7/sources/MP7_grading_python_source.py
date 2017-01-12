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
		
	Questions I have yet to answer:
		1. Should calls to pthread_create and pthread_join
		be breakpoint-checked, to ensure correct orderings? Their
		arguments could also be checked each time, if it would help.
"""

#Constants
ulong_ptr_type = gdb.lookup_type("unsigned long int").pointer()

#Result booleans, which will be used directly in grade calculation
#via a boolean AND of all values (negated as appropriate)
#corresponding to a given rubric item, after being passed to the
#controlling shell script (somehow)
#NOTE: Make sure default values are correct and helpful
R1_three_request_threads_created = False
R1_rthread_joined_before_all_rthreads_created = False
R2_data_request_pushed_outside_rtf = False
R2_quit_pushed_inside_rtf = False
R2_n_requests_pushed_per_rtf = False
R3_wthread_started_before_rthreads_joined = False
R3_rthreads_joined = False
W1_w_quits_pushed_from_main = False
W1_wthread_started_before_quits_pushed = False
W1_wthread_joined_before_all_wthreads_created = False
W1_wthreads_joined = False
W1_wthreads_joined_before_mht_called = False
W2_mht_function_used = False
W2_mht_output_correct = False
W3_1_SafeBuffer_used = False

#Intermediate, progress-tracking booleans
rthreads_created = False
rthreads_joined = False
wthreads_created = False
wthreads_joined = False

#Client parameters, will need to extract somehow,
#but for now they can be hard-coded
n = 100
w = 20

#Program state information, converted to more usable Python types
rtf_hitcount = 0
wtf_hitcount = 0
rthreads_running = 0
wthreads_running = 0
newest_thread_number = 1#Starts with main
client_threads = {1:("main",1)}#maps pthread_t values to corresponding function names and gdb thread numbers
gdb_threads = {1:("main",1)}#maps gdb thread numbers to corresponding function names and pthread_t values
"""
	Note that main is given an arbitrary pthread_t value:
	main's pthread_t value is never actually needed, and would be too
	much trouble to obtain anyway, but it's fine to use strings
	to represent these numbers since no arithmetic is ever done with them.
	pthread_t values for all other threads will be hexadecimal numbers
	converted to strings.
"""

def stop_handler (event):
	#Constants
	global ulong_ptr_type
	
	#Results
	global R1_three_request_threads_created
	global R1_rthread_joined_before_all_rthreads_created
	global R2_data_request_pushed_outside_rtf
	global R2_quit_pushed_inside_rtf
	global R2_n_requests_pushed_per_rtf
	global R3_wthread_started_before_rthreads_joined
	global R3_rthreads_joined
	global W1_w_quits_pushed_from_main
	global W1_wthread_started_before_quits_pushed
	global W1_wthread_joined_before_all_wthreads_created
	global W1_wthreads_joined
	global W1_wthreads_joined_before_mht_called
	global W2_mht_function_used
	global W2_mht_output_correct
	global W3_1_SafeBuffer_used

	#Intermediate
	global rthreads_created
	global rthreads_joined
	global wthreads_created
	global wthreads_joined

	#Parameters
	global n
	global w

	#Program state
	global rtf_hitcount
	global wtf_hitcount
	global rthreads_running
	global wthreads_running
	global newest_thread_number
	global client_threads
	global gdb_threads

	try:
		if event.__class__.__name__ == "BreakpointEvent":
			for bp in event.breakpoints:
				print("Hit breakpoint at " + str(bp.location))
				if bp.location == "request_thread_function":
					print("python: rtf breakpoint")
					rtf_hitcount = bp.hit_count
					if bp.hit_count == 1:
						print("First rtf breakpoint hit!")
						gdb.execute("thread 1")
						gdb.execute("up 99999")
						gdb.execute("watch -l request_buffer.data")
						if gdb.execute("whatis request_buffer", False, True) == "type = SafeBuffer\n":
							W3_1_SafeBuffer_used = True
							print("Student used SafeBuffer!")
							"""
								Odds are that the student will lose a lot more points than
								just W3.1 if they don't use the SafeBuffer class.
								
								Anyway, check here that request_buffer.data.size is 0.
								Can't have them pushing requests and stuff before the
								data watchpoint is added, now can we?
							"""
						else:
							W3_1_SafeBuffer_used = False
							print("Student did not use SafeBuffer...")
					elif bp.hit_count == 3:
						R1_three_request_threads_created = True
						print("Third request thread created!")
					elif R1_three_request_threads_created == True and bp.hit_count > 3:
						R1_three_request_threads_created = False
						print("Seriously, why would you make more than 3 request threads?")
		
				elif bp.location == "worker_thread_function":
					wtf_hitcount = bp.hit_count
					print("python: worker_thread_function")
					if bp.hit_count == 1:
						pass
				elif bp.location == "make_histogram_table":
					print("python: make_histogram_table")
				elif bp.location == "pthread_create":
					print("python: pthread_create")
					newest_thread_number += 1
					print("newest thread number is " + str(newest_thread_number))
					thread_id_addr = gdb.selected_frame().read_var("newthread")
					print("thread_in_addr == " + str(thread_id_addr))
					thread_id_ptr = thread_id_addr.cast(ulong_ptr_type)
					print("thread_id_ptr == " + str(thread_id_ptr))
					gdb.execute("fin")
					#thread_id = long(thread_id_ptr.dereference())
					thread_id = long(thread_id_ptr.referenced_value())
					print("obtained thread_id " + str(thread_id) + ", switching to thread " + str(newest_thread_number))
					gdb.execute("thread " + str(newest_thread_number))
					print("inside thread " + str(newest_thread_number))
					tfunc = str(gdb.selected_frame().function().print_name.rstrip())
					print ("name of function: " + tfunc + ", adding to dictionaries...")
					client_threads[thread_id] = (tfunc,newest_thread_number)
					gdb_threads[newest_thread_number] = (tfunc,thread_id)
					print("New thread: id == " + str(thread_id) + ", number == " + str(newest_thread_number) + ", tfunc == " + tfunc)
					if tfunc == "request_thread_function(void*)":
						print("It works! My time machine works!")
					elif tfunc == "worker_thread_function(void*)":
						print("I am a stegosaurus!")
				elif bp.location == "pthread_join":
					print("python: pthread_join")
					thread_id = gdb.selected_frame().read_var("threadid")
					if thread_id in client_threads:
						print("Hooray! This thread we're joining was actually created!")
				elif bp.number == 7:
					print("python: watchpoint")
		print("Un-breaking/continuing...")
		gdb.execute("cont")
	except TypeError:
		traceback.print_exc()
		gdb.execute("quit")

def main():
	try:
		#gdb.execute("start -w 20 -n 1")
		gdb.execute("start -v 3 -w 5 -n 1") #student submissions cannot be counted on to have a verbosity parameter called v that can take 3 as a valid argument
		gdb.events.stop.connect (stop_handler)
		rtf_break = gdb.Breakpoint("request_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
		wtf_break = gdb.Breakpoint("worker_thread_function", gdb.BP_BREAKPOINT, 0, False, False)
		mht_break = gdb.Breakpoint("make_histogram_table", gdb.BP_BREAKPOINT, 0, False, False)
		pcreate_break = gdb.Breakpoint("pthread_create", gdb.BP_BREAKPOINT, 0, False, False)
		pjoin_break = gdb.Breakpoint("pthread_join", gdb.BP_BREAKPOINT, 0, False, False)
		gdb.execute("cont")
		"""
			Guessing that this is the place where all
			the points would be tallied up based on the
			variables evaluated at the different
			break points.
		"""
		gdb.execute("quit")
	except TypeError:
		traceback.print_exc()
		gdb.execute("quit")
	
main()
