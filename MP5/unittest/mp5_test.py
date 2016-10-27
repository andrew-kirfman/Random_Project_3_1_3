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
import collections
import os
from subprocess import check_output
from optparse import OptionParser


# --------------------------------------------------------------------------- #
# Global Variables                                                            #
# --------------------------------------------------------------------------- #

executable_name = ""


def call_shell_output():
    pass

def main(argv):
    global executable_name
    
    sys.stdout.write("Enter the name of the student's executable to test: ")
    executable_name = sys.stdin.readline().strip()

    if executable_name == "":
        print "[ERROR]: No executable to check was provided."
        return

    # Test whether options.executable exists
    try:
        test_f = open("./%s" % executable_name, "r")
        test_f.close()
    except IOError as e:
        print "[ERROR]: Entered executable doesn't exist."
        return

    unittest.main()

class TestHandlers(unittest.TestCase):
    def setUp(self):
        pass

    def test_simple_command_1(self):
        global executable_name

        command1 = "ls"
        command1_shell = "echo \"ls\" | %s -t" % executable_name
    
        cmd1_test = check_output(command1, shell=True)
        cmd1_shell = check_output(command1_shell, shell=True)

        self.assertEqual(cmd1_test, cmd1_shell)

    def test_simple_command_2(self):
        global executable_name

        command2 = "pwd"
        command2_shell = "echo \"pwd\" | %s -t" % executable_name

        cmd2_test = check_output(command2, shell=True)
        cmd2_shell = check_output(command2_shell, shell=True)

        self.assertEqual(cmd2_test, cmd2_shell)

    def test_simple_arguments_1(self):
        global executable_name

        command3 = "ls -la"
        command3_shell = "echo \"ls -la\" | %s -t" % executable_name

        cmd3_test = check_output(command3, shell=True)
        cmd3_shell = check_output(command3_shell, shell=True)
        
        self.assertEqual(cmd3_test, cmd3_shell)

    def test_simple_arguments_2(self):
        global executable_name

        command4 = "du -h"
        command4_shell = "echo \"du -h\" | %s -t" % executable_name

        cmd4_test = check_output(command4, shell=True)
        cmd4_shell = check_output(command4_shell, shell=True)

        self.assertEqual(cmd4_test, cmd4_shell)

    def test_redirection(self):
        global executable_name

        command5_shell = "echo \"echo Hello World > temp_shell\" | %s -t" % executable_name

        check_output(command5_shell, shell=True)

        command6 = "cat < temp_shell"
        command6_shell = "echo \"cat < temp_shell\" | %s -t" % executable_name

        cmd6_test = check_output(command6, shell=True)
        cmd6_shell = check_output(command6_shell, shell=True)

        os.unlink("./temp_shell")

        self.assertEqual(cmd6_test, cmd6_shell)

    def test_multiple_args(self):
        global executable_name

        check_output("echo \"Hello World\" > temp_shell", shell=True)
        
        command7_shell = "echo \"tar -c -v -f temp.tar temp_shell\" | %s -t" % executable_name

        check_output(command7_shell, shell=True)

        command8 = "ls -la | grep temp.tar"
        command8_shell = "echo \"ls -la | grep temp.tar\" | %s -t" % executable_name

        cmd8_test = check_output(command8, shell=True)
        cmd8_shell = check_output(command8_shell, shell=True)

        os.unlink("./temp_shell")
        os.unlink("./temp.tar")

        self.assertEqual(cmd8_test, cmd8_shell)

    def test_simple_pipe_1(self):
        global executable_name

        command9 = "ls | cat"
        command9_shell = "echo \"ls | cat\" | %s -t" % executable_name
        
        cmd9_test = check_output(command9, shell=True)
        cmd9_shell = check_output(command9_shell, shell=True)

        self.assertEqual(cmd9_test, cmd9_shell)

    def test_advanced_pipe_1(self):
        global executable_name

        command10 = "ls | cat | cat | cat"
        command10_shell = "echo \"ls | cat | cat | cat\" | %s -t" % executable_name

        cmd10_test = check_output(command10, shell=True)
        cmd10_shell = check_output(command10_shell, shell=True)

        self.assertEqual(cmd10_test, cmd10_shell)

    def test_advanced_pipe_2(self):
        global executable_name

        command11 = "ls -la | cat -A | cat -A | cat -A"
        command11_shell = "echo \"ls -la | cat -A | cat -A | cat -A\" | %s -t" % executable_name

        cmd11_test = check_output(command11, shell=True)
        cmd11_shell = check_output(command11_shell, shell=True)

        self.assertEqual(cmd11_test, cmd11_shell)


if __name__ == "__main__":
    main(sys.argv)
