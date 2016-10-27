#!/usr/bin/env python

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #4                                        #
#                                                                             #
# File: ./student/proctest.py                                                 #
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# Standard Library Includes                                                   #
# --------------------------------------------------------------------------- #

import sys
import re
from subprocess import check_output, CalledProcessError


"""
Note to students: Your program must be written using the provided class
template below.  Functions that require your implementation are marked
with "Work needed here" in their method docstrings.

Be sure to delete all of these extraneous comments before submitting your
code for grading.  Also, delete all extra 'pass' statements.  They are
noop instructions and only there because Python does not like empty functions.
"""


class Proctest(object):
    """
    Object to contain process data and getter functions
    """

    def __init__(self, pid):
        """
        Read all data from pertinent files and save as class members

        Work needed here!
        """
        pass


    def getpid(self):
        """
        Returns the process id

        Work needed here!
        """
        pass


    def getppid(self):
        """
        Returns process's parent's ID

        Work needed here!
        """
        pass


    def geteuid(self):
        """
        Returns the process's effective user ID

        Work needed here!
        """
        pass


    def getegid(self):
        """
        Returns process's effective group ID

        Work needed here!
        """
        pass


    def getruid(self):
        """
        Returns process's real user ID

        Work needed here!
        """
        pass


    def getrgid(self):
        """
        Returns process's real group ID

        Work needed here!
        """
        pass


    def getfsuid(self):
        """
        Returns process's file system user ID

        Work needed here!
        """
        pass


    def getfsgid(self):
        """
        Returns process's file system group ID

        Work needed here!
        """
        pass


    def getstate(self):
        """
        Returns process's current state

        Work needed here!
        """
        pass


    def getthread_count(self):
        """
        Returns process's current thread count

        Work needed here!
        """
        pass


    def getpriority(self):
        """
        Returns process's current prioroty number

        Work needed here!
        """
        pass


    def getniceness(self):
        """
        Returns process's current niceness value

        Work needed here!
        """
        pass


    def getstime(self):
        """
        Returns process's current stime value

        Work needed here!
        """
        pass


    def getutime(self):
        """
        Returns process's current utime value

        Work needed here!
        """
        pass


    def getcstime(self):
        """
        Returns process's current cstime value

        Work needed here!
        """
        pass


    def getcutime(self):
        """
        Returns process's current cutime value

        Work needed here!
        """
        pass


    def getstartcode(self):
        """
        Returns process's startcode

        Work needed here!
        """
        pass


    def getendcode(self):
        """
        Returns process's endcode

        Work needed here!
        """
        pass


    def getesp(self):
        """
        Returns process's stack pointer

        Work needed here!
        """
        pass


    def geteip(self):
        """
        Returns process's instruction pointer

        Work needed here!
        """
        pass


    def getfiles(self):
        """
        Returns process's current number of open file descriptors

        Work needed here!
        """
        pass


    def getvoluntary_context_switches(self):
        """
        Returns the number of times that the process has voluntarily
        yielded control to the kernel.

        Work needed here!
        """
        pass


    def getnonvoluntary_context_switches(self):
        """
        Returns the number of times that the process has had control
        taken from it forcefully by the kernel.

        Work needed here!
        """
        pass


    def getlast_cpu(self):
        """
        Returns the last cpu that the process executed on

        Work needed here!
        """
        pass


    def getallowed_cpus(self):
        """
        Returns a list of allowed processors

        Work needed here!
        """
        pass


    def getmemory_map(self):
        """
        Returns process's memory map

        Work needed here!
        """
        pass


def main():
    """
    See comments below for expected content of this function

    Work needed here!
    """

    # Read in PID

    # Instantiate class member

    # Print out values

    pass


if __name__ == "__main__":
    main()

