import sys
import gdb
from subprocess import call

call(["make", "clean"])
call(["make", "all"])
gdb.execute('quit')