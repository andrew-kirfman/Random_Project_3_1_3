#!/usr/bin/env python

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #5                                        #
#                                                                             #
# File ./mp5_test.py                                                          #
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# Standard Library Includes                                                   #
# --------------------------------------------------------------------------- #

import unittest
import sys
import re
from subprocess import check_output, CalledProcessError
sys.path.append(".")
from proctest import Proctest

# --------------------------------------------------------------------------- #
# Global Variables                                                            #
# --------------------------------------------------------------------------- #

process_pid = -1
process_data = None
stat_array = []
status_array = []
num_fds = -1
mem_array = []


def main():
    global process_pid, process_data, stat_array, status_array,\
        num_fds, mem_array

    sys.stdout.write("Enter a pid to test: ")
    sys.stdout.flush()
    process_pid = sys.stdin.readline()

    stat_fname = ""
    status_fname = ""
    stat_file = None

    while True:
        try:
            process_pid = int(process_pid)
            break
        except ValueError:
            pass

        sys.stdout.write("[ERROR]: PID is not an integer.  Try again: ")
        sys.stdout.flush()
        process_pid = sys.stdin.readline()

    while True:
        stat_fname = "/proc/%s/stat" % (process_pid)
        status_fname = "/proc/%s/status" % (process_pid)

        try:
            stat_file = open(stat_fname, "r")
            break
        except IOError:
            sys.stdout.write("[ERROR]: Process does not exist.  Try again: ")
            sys.stdout.flush()
            process_pid = sys.stdin.readline()

            while True:
                try:
                    process_pid = int(process_pid)
                    break
                except IOError:
                    sys.stdout.write("[ERROR]: PID is not an integer.  Try again: ")
                    sys.stdout.flush()
                    process_pid = sys.stdin.readline()

    if stat_file is None:
        sys.stdout.write("[ERROR]: Could not open /proc/%s/stat." % process_pid)

    # Read and then close the stat file
    stat_string = stat_file.readline()
    stat_file.close()

    # Turn the stat string into an array of values
    stat_array = stat_string.strip().split(' ')

    # Read in the status file
    status_file = open(status_fname, "r")
    if status_file is None:
        sys.stdout.write("[ERROR]: Could not open /proc/%s/status." % process_pid)

    status_array = status_file.readlines()
    for i in range(0, len(status_array)):
        temp_item = status_array[i]
        if len(temp_item) < 2:
            sys.stdout.write("[ERROR]: Could not properly read /proc/%s/status." % process_pid)
            continue

        status_array[i] = temp_item.strip("\n\t ").split('\t')
    status_file.close()

    # Read in file containing file handle information
    try:
        command_output = check_output("ls /proc/%s/fd 2> /dev/null" % process_pid, shell=True)
        num_fds = len(command_output.split("\n")) - 1
    except CalledProcessError:
        print "[ERROR]: File descriptor count access permission denied."
        num_fds = "permission denied"

    # Read in memory map
    mem_fname = "/proc/%s/smaps" % process_pid
    mem_file = open(mem_fname, "r")
    try:
        mem_array = mem_file.readlines()

        filter_mem_array = []
        for i in range(0, len(mem_array) - 1):
            if re.match("^[A-Z]", mem_array[i]) is None:
                filter_mem_array.append(mem_array[i].strip())

        mem_array = filter_mem_array
        mem_file.close()
    except IOError:
        print "[ERROR]: Memory map access permission denied."
        mem_array = ["permission denied"]


    process_data = Proctest(process_pid)
    unittest.main()


class TestHandlers(unittest.TestCase):
    def setUp(self):
        pass


    def test_getpid(self):
        student_pid = process_data.getpid()
        test_pid = stat_array[0]

        self.assertEqual(str(test_pid), str(student_pid))


    def test_getppid(self):
        student_ppid = process_data.getppid()
        test_ppid = stat_array[3]

        self.assertEqual(str(test_ppid), str(student_ppid))


    def test_geteuid(self):
        student_euid = process_data.geteuid()
        test_euid = ""

        for i in range(0, len(status_array)):
            if "Uid" in status_array[i][0]:
                test_euid = status_array[i][2]

        self.assertEqual(str(test_euid), str(student_euid))


    def test_getegid(self):
        student_egid = process_data.getegid()
        test_egid = ""

        for i in range(0, len(status_array)):
            if "Gid" in status_array[i][0]:
                test_egid = status_array[i][2]

        self.assertEqual(str(test_egid), str(student_egid))


    def test_getruid(self):
        student_ruid = process_data.getruid()
        test_ruid = ""

        for i in range(0, len(status_array)):
            if "Uid" in status_array[i][0]:
                test_ruid = status_array[i][1]

        self.assertEqual(str(test_ruid), str(student_ruid))


    def test_getrgid(self):
        student_rgid = process_data.getrgid()
        test_rgid = ""

        for i in range(0, len(status_array)):
            if "Gid" in status_array[i][0]:
                test_rgid = status_array[i][1]

        self.assertEqual(str(test_rgid), str(student_rgid))


    def test_getfsuid(self):
        student_fsuid = process_data.getfsuid()
        test_fsuid = ""

        for i in range(0, len(status_array)):
            if "Uid" in status_array[i][0]:
                test_fsuid = status_array[i][4]

        self.assertEqual(str(test_fsuid), str(student_fsuid))


    def test_getfsgid(self):
        student_fsgid = process_data.getfsgid()
        test_fsgid = ""

        for i in range(0, len(status_array)):
            if "Gid" in status_array[i][0]:
                test_fsgid = status_array[i][4]

        self.assertEqual(str(test_fsgid), str(student_fsgid))


    def test_getstate(self):
        student_state = process_data.getstate()
        test_state = stat_array[2]

        self.assertEqual(str(test_state), str(student_state))


    def test_getthread_count(self):
        student_thread_count = process_data.getthread_count()
        test_thread_count = stat_array[19]

        self.assertEqual(str(test_thread_count), str(student_thread_count))


    def test_getpriority(self):
        student_priority = process_data.getpriority()
        test_priority = stat_array[17]

        self.assertEqual(str(test_priority), str(student_priority))


    def test_getniceness(self):
        student_niceness = process_data.getniceness()
        test_niceness = stat_array[18]

        self.assertEqual(str(test_niceness), str(student_niceness))


    def test_getstime(self):
        student_stime = process_data.getstime()
        test_stime = stat_array[14]

        self.assertEqual(str(test_stime), str(student_stime))


    def test_getutime(self):
        student_utime = process_data.getutime()
        test_utime = stat_array[13]

        self.assertEqual(str(test_utime), str(student_utime))


    def test_getcstime(self):
        student_cstime = process_data.getcstime()
        test_cstime = stat_array[16]

        self.assertEqual(str(test_cstime), str(student_cstime))


    def test_getcutime(self):
        student_cutime = process_data.getcutime()
        test_cutime = stat_array[15]

        self.assertEqual(str(test_cutime), str(student_cutime))
        

    def test_getstartcode(self):
        student_startcode = process_data.getstartcode()
        test_startcode = stat_array[25]

        self.assertEqual(str(test_startcode), str(student_startcode))


    def test_getendcode(self):
        student_endcode = process_data.getendcode()
        test_endcode = stat_array[26]

        self.assertEqual(str(test_endcode), str(student_endcode))


    def test_getesp(self):
        student_esp = process_data.getesp()
        test_esp = stat_array[28]

        self.assertEqual(str(test_esp), str(student_esp))


    def test_geteip(self):
        student_eip = process_data.geteip()
        test_eip = stat_array[29]

        self.assertEqual(str(test_eip), str(student_eip))


    def test_getfiles(self):
        student_files = process_data.getfiles()
        test_files = num_fds

        self.assertEqual(str(test_files), str(student_files))


    def test_getvoluntary_context_switches(self):
        student_voluntary = process_data.getvoluntary_context_switches()
        test_voluntary = ""
    
        for i in range(0, len(status_array)):
            if "voluntary" in status_array[i][0] and "nonvoluntary" not in status_array[i][0]:
                test_voluntary = status_array[i][1]

        self.assertEqual(str(test_voluntary), str(student_voluntary))


    def test_getnonvoluntary_context_switches(self):
        student_nonvoluntary = process_data.getnonvoluntary_context_switches()
        test_nonvoluntary = ""

        for i in range(0, len(status_array)):
            if "nonvoluntary" in status_array[i][0]:
                test_nonvoluntary = status_array[i][1]

        self.assertEqual(str(test_nonvoluntary), str(student_nonvoluntary))


    def test_getlast_cpu(self):
        student_lastcpu = process_data.getlast_cpu()
        test_lastcpu = stat_array[38]

        self.assertEqual(str(test_lastcpu), str(student_lastcpu))


    def test_getallowed_cpus(self):
        student_allowedcpus = process_data.getallowed_cpus()
        test_allowedcpus = ""

        for i in range(0, len(status_array)):
            if "Cpus_allowed" in status_array[i][0] and "list" not in status_array[i][0]:
                test_allowedcpus = status_array[i][1]

        self.assertEqual(str(test_allowedcpus), str(student_allowedcpus))


    def test_getmemory_map(self):
        student_memory = process_data.getmemory_map()
        test_memory = mem_array

        mem_equality = True

        for i in range(0, len(test_memory)):
            if test_memory[i] not in student_memory:
                mem_equality = False

        self.assertEqual(mem_equality, True)


if __name__ == "__main__":
    main()
