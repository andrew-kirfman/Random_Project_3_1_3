#!/usr/bin/env python

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #4                                        #
#                                                                             #
# File: ./proctest.py                                                         #
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# Standard Library Includes                                                   #
# --------------------------------------------------------------------------- #

import sys
import re
from subprocess import check_output, CalledProcessError


class Proctest(object):
    """
    Container for all methods that retrieve process data.
    """

    def __init__(self, pid):
        """
        Set up method that reads all data files and formats
        the returned data for easy retrieval later.
        """

        self.pid = -1
        self.stat_array = None
        self.status_array = None
        self.num_fds = 0
        self.mem_array = None
        self.is_initialized = False

        stat_fname = ""
        status_fname = ""
        self.stat_file = None

        while True:
            try:
                self.pid = int(pid)
                break
            except ValueError:
                pass

            sys.stdout.write("[ERROR]: PID is not an integer.  Try again: ")
            sys.stdout.flush()
            self.pid = sys.stdin.readline()


        while True:
            stat_fname = "/proc/%s/stat" % (self.pid)
            status_fname = "/proc/%s/status" % (self.pid)

            try:
                self.stat_file = open(stat_fname, "r")
                break
            except IOError:
                sys.stdout.write("[ERROR]: Process does not exist.  Try again: ")
                sys.stdout.flush()
                pid = sys.stdin.readline()

                while True:
                    try:
                        self.pid = int(pid)
                        break
                    except IOError:
                        sys.stdout.write("[ERROR]: PID is not an integer.  Try again: ")
                        sys.stdout.flush()
                        pid = sys.stdin.readline()

        if self.stat_file is None:
            sys.stdout.write("[ERROR]: Could not open /proc/%s/stat." % self.pid)

        # Read and then close the stat file
        stat_string = self.stat_file.readline()
        self.stat_file.close()

        # Turn the stat string into an array of values
        self.stat_array = stat_string.strip().split(' ')

        # Read in the status file
        status_file = open(status_fname, "r")
        if status_file is None:
            sys.stdout.write("[ERROR]: Could not open /proc/%s/status." % self.pid)

        self.status_array = status_file.readlines()
        for i in range(0, len(self.status_array)):
            temp_item = self.status_array[i]
            if len(temp_item) < 2:
                sys.stdout.write("[ERROR]: Could not properly read /proc/%s/status." % self.pid)
                continue

            self.status_array[i] = temp_item.strip("\n\t ").split('\t')
        status_file.close()

        # Read in file containing file handle information
        try:
            command_output = check_output("ls /proc/%s/fd 2> /dev/null" % self.pid, shell=True)
            self.num_fds = len(command_output.split("\n")) - 1
        except CalledProcessError:
            print "[ERROR]: File descriptor count access permission denied."
            self.num_fds = "permission denied"

        # Read in memory map
        mem_fname = "/proc/%s/smaps" % self.pid
        mem_file = open(mem_fname, "r")
        try:
            self.mem_array = mem_file.readlines()

            filter_mem_array = []
            for i in range(0, len(self.mem_array) - 1):
                if re.match("^[A-Z]", self.mem_array[i]) is None:
                    filter_mem_array.append(self.mem_array[i].strip())

            self.mem_array = filter_mem_array
            mem_file.close()
        except IOError:
            print "[ERROR]: Memory map access permission denied."
            self.mem_array = ["permission denied"]

        self.is_initialized = True


    def getpid(self):
        """
        Returns process id

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[0]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[0]) != "":
            return str(self.stat_array[0])
        else:
            return "ERROR"


    def getppid(self):
        """
        Returns process's parent's ID

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[3]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[3]) != "":
            return str(self.stat_array[3])
        else:
            return "ERROR"


    def geteuid(self):
        """
        Returns process's effective user ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][2], where i is the index of the
              array whose first element is "Uid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Uid" in str(self.status_array[i][0]):

                if str(self.status_array[i][2]) != "":
                    return str(self.status_array[i][2])
                else:
                    return "ERROR"

        return "ERROR"


    def getegid(self):
        """
        Returns process's effective group ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][2], where i is the index of the
              array whose first element is "Gid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Gid" in str(self.status_array[i][0]):

                if str(self.status_array[i][2]) != "":
                    return str(self.status_array[i][2])
                else:
                    return "ERROR"

        return "ERROR"


    def getruid(self):
        """
        Returns process's real user ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1], where i is the index of the
              array whose first element is "Uid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Uid" in str(self.status_array[i][0]):

                if str(self.status_array) != "":
                    return str(self.status_array[i][1])
                else:
                    return "ERROR"

        return "ERROR"


    def getrgid(self):
        """
        Returns process's real group ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1], where is is the inex of the
              array whose first element is "Gid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Gid" in self.status_array[i][0]:

                if str(self.status_array) != "":
                    return str(self.status_array[i][1])
                else:
                    return "ERROR"

        return "ERROR"


    def getfsuid(self):
        """
        Returns process's file system user ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1], where is is the inex of the
              array whose first element is "Uid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Uid" in str(self.status_array[i][0]):

                if str(self.status_array[i][4]) != "":
                    return str(self.status_array[i][4])
                else:
                    return "ERROR"

        return "ERROR"


    def getfsgid(self):
        """
        Returns process's file system group ID

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1], where is is the inex of the
              array whose first element is "Gid".

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Gid" in str(self.status_array[i][0]):

                if str(self.status_array[i][4]) != "":
                    return str(self.status_array[i][4])
                else:
                    return "ERROR"

        return "ERROR"


    def getstate(self):
        """
        Returns process's current state

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[2]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[2]) != "":
            return str(self.stat_array[2])
        else:
            return "ERROR"


    def getthread_count(self):
        """
        Returns process's current thread count

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[19]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if self.stat_array[19] != "":
            return str(self.stat_array[19])
        else:
            return "ERROR"


    def getpriority(self):
        """
        Returns process's current prioroty number

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[17]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[17]) != "":
            return str(self.stat_array[17])
        else:
            return "ERROR"


    def getniceness(self):
        """
        Returns process's current niceness value

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[18]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[18]) != "":
            return str(self.stat_array[18])
        else:
            return "ERROR"


    def getstime(self):
        """
        Returns process's current stime value

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[14]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[14]) != "":
            return str(self.stat_array[14])
        else:
            return "ERROR"


    def getutime(self):
        """
        Returns process's current utime value

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[13]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[13]) != "":
            return str(self.stat_array[13])
        else:
            return "ERROR"


    def getcstime(self):
        """
        Returns process's current cstime value

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[16]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[16]) != "":
            return str(self.stat_array[16])
        else:
            return "ERROR"


    def getcutime(self):
        """
        Returns process's current cutime value

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[15]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[15]) != "":
            return str(self.stat_array[15])
        else:
            return "ERROR"


    def getstartcode(self):
        """
        Returns process's startcode

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[25]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[25]) != "":
            return str(self.stat_array[25])
        else:
            return "ERROR"


    def getendcode(self):
        """
        Returns process's endcode

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[26]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[26]) != "":
            return str(self.stat_array[26])
        else:
            return "ERROR"


    def getesp(self):
        """
        Returns process's stack pointer

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[28]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[28]) != "":
            return str(self.stat_array[28])
        else:
            return "ERROR"


    def geteip(self):
        """
        Returns process's instruction pointer

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[29]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[29]) != "":
            return str(self.stat_array[29])
        else:
            return "ERROR"


    def getfiles(self):
        """
        Returns process's current number of open file descriptors

        Input Arguments:
            - None

        Return Values:
            - string: self.num_fds

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.num_fds) != "":
            return str(self.num_fds)
        else:
            return "ERROR"


    def getvoluntary_context_switches(self):
        """
        Returns the number of times that the process has voluntarily
        yielded control to the kernel.

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1] where i is the index of the
              array whose first element contains "voluntary" and not
              "nonvoluntary"

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "voluntary" in self.status_array[i][0] and "nonvoluntary"\
                not in self.status_array[i][0]:

                if str(self.status_array[i][1]) != "":
                    return str(self.status_array[i][1])
                else:
                    return "ERROR"

        return "ERROR"


    def getnonvoluntary_context_switches(self):
        """
        Returns the number of times that the process has had control
        taken from it forcefully by the kernel.

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][0] where i is the index of the
              array whose first element contains "nonvoluntary"

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if  "nonvoluntary" in self.status_array[i][0]:

                if str(self.status_array[i][1]) != "":
                    return str(self.status_array[i][1])
                else:
                    return "ERROR"

        return "ERROR"


    def getlast_cpu(self):
        """
        Returns the last cpu that the process executed on

        Input Arguments:
            - None

        Return Values:
            - string: self.stat_array[38]

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        if str(self.stat_array[38]) != "":
            return str(self.stat_array[38])
        else:
            return "ERROR"


    def getallowed_cpus(self):
        """
        Returns a list of allowed processors

        Input Arguments:
            - None

        Return Values:
            - string: self.status_array[i][1] where i is the index of the
              element whose first element contains "Cpus_allowed"

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        for i in range(0, len(self.status_array)):
            if "Cpus_allowed" in self.status_array[i][0] and "list" not in self.status_array[i][0]:
                if str(self.status_array[i][1]) != "":
                    return str(self.status_array[i][1])
                else:
                    return "ERROR"

        return "ERROR"


    def getmemory_map(self):
        """
        Returns process's memory map

        Input Arguments:
            - None

        Return Values:
            - array: self.mem_array

        Error Checking:
            - Array data is cast to a string even though it should already
              be one.
            - Notifies the caller if the correct data was not found.
        """

        if self.is_initialized is False:
            return "UNINITIALIZED"

        return self.mem_array


def main():

    # Read in PID
    sys.stdout.write("Enter the PID of a process: ")
    sys.stdout.flush()
    process_pid = sys.stdin.readline()

    process_data = Proctest(process_pid)

    # Start printing out values
    print ""
    print "Process Information:"
    print "  1) Identifiers"
    print "     PID: %s" % process_data.getpid()
    print "     PPID: %s" % process_data.getppid()
    print "     EUID: %s" % process_data.geteuid()
    print "     EGID: %s" % process_data.getegid()
    print "     RUID: %s" % process_data.getruid()
    print "     RGID: %s" % process_data.getrgid()
    print "     FSUID: %s" % process_data.getfsuid()
    print "     FSGID: %s" % process_data.getfsgid()
    print ""
    print "  2) State"
    print "     State: %s" % process_data.getstate()
    print ""
    print "  3) Thread Information"
    print "     Thread Count: %s" % process_data.getthread_count()
    print ""
    print "  4) Priority"
    print "     Priority Number: %s" % process_data.getpriority()
    print "     Niceness Value: %s" % process_data.getniceness()
    print ""
    print "  5) Time Information"
    print "     stime: %s" % process_data.getstime()
    print "     utime: %s" % process_data.getutime()
    print "     cstime: %s" % process_data.getcstime()
    print "     cutime: %s" % process_data.getcutime()
    print ""
    print "  6) Address Space"
    print "     Startcode: %s" % process_data.getstartcode()
    print "     Endcode: %s" % process_data.getendcode()
    print "     ESP: %s" % process_data.getesp()
    print "     EIP: %s" % process_data.geteip()
    print ""
    print "  7) Resources"
    print "     File Handles: %s" % process_data.getfiles()
    print "     Voluntary Context Switches: %s" % process_data.getvoluntary_context_switches()
    print "     Nonvoluntary Context Switches: %s" % process_data.getnonvoluntary_context_switches()
    print ""
    print "  8) Processor"
    print "     Last Processor: %s" % process_data.getlast_cpu()
    print "     Allowed Cores: %s" % process_data.getallowed_cpus()
    print ""
    print "  9) Memory Map"
    temp_mem_array = process_data.getmemory_map()
    for line in temp_mem_array:
        print "     %s" % line


if __name__ == "__main__":
    main()

