import sys
import gdb
import traceback
from subprocess import call

#ulong_ptr_type = gdb.lookup_type("unsigned long int").pointer()
#thread_id_addr = gdb.Value(0x7effec60)
#thread_id_ptr = thread_id_addr.cast(ulong_ptr_type)
#thread_id_raw = long(thread_id_ptr.dereference())
#thread_id_hex = str(hex(thread_id_raw)).rstrip("L\n")
#print thread_id_hex
newest_thread_number = 2
gdb.execute("up 9999")
ulong_ptr_type = gdb.lookup_type("unsigned long int").pointer()
gdb.execute("down 9999")
thread_id_addr = gdb.selected_frame().read_var("newthread")
thread_id_ptr = thread_id_addr.cast(ulong_ptr_type)
gdb.execute("fin")
thread_id = long(thread_id_ptr.dereference())
gdb.execute("thread " + str(newest_thread_number))
tfunc = gdb.selected_frame().function().print_name.rstrip()
client_threads[thread_id] = (tfunc, newest_thread_number)
gdb_threads[newest_thread_number] = (tfunc, thread_id)
print("New thread: id == " + str(thread_id) + ", number == " + str(newest_thread_number) + ", tfunc == " + tfunc)
if tfunc == "request_thread_function(void*)":
	print("It works! My time machine works!")
else:
	pass
