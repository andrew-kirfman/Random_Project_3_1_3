# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                    # 
# Project: CSCE-313 Machine Problem #5                                         #
#                                                                              #
# File: ./Grading                                                              #
# ---------------------------------------------------------------------------- #

echo 'VOC_NO_REPORT_OUTPUT' >> $vocareumReportFile

# ---------------------------------------------------------------------------- #
# Check Student's Files                                                        # 
# ---------------------------------------------------------------------------- #

FILE_ERROR=false

cat shell.cpp > /dev/null

if [ "$?" != "0" ]; then
	FILE_ERROR=true
fi

cat Makefile > /dev/null

if [ "$?" != "0" ]; then
	FILE_ERROR=true
fi

if [ "$FILE_ERROR" == "true" ]; then
	echo "Unable to locate required files" >> $vocareumReportFile
	echo "SimpleCommand1   ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleCommand2   ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleArguments1 ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleArguments2 ...   Untestable [0/10]" >> $vocareumReportFile
	echo "redirection      ...   Untestable [0/15]" >> $vocareumReportFile
	echo "multipleArgs     ...   Untestable [0/10]" >> $vocareumReportFile
	echo "simplePipe1      ...   Untestable [0/10]" >> $vocareumReportFile
	echo "advancedPipe1    ...   Untestable [0/10]" >> $vocareumReportFile
	echo "advancedPipe2    ...   Untestable [0/15]" >> $vocareumReportFile

	echo "SimpleCommand1,0" >> $vocareumGradeFile
	echo "SimpleCommand2,0" >> $vocareumGradeFile
	echo "SimpleArguments1,0" >> $vocareumGradeFile
	echo "SimpleArguments2,0" >> $vocareumGradeFile
	echo "redirection,0" >> $vocareumGradeFile
	echo "multipleArgs,0" >> $vocareumGradeFile
	echo "simplePipe1,0" >> $vocareumGradeFile
	echo "advancedPipe1,0" >> $vocareumGradeFile
	echo "advancedPipe2,0" >> $vocareumGradeFile

	exit 1
fi

make > /dev/null

if [ "$?" != "0" ]; then
	echo "Unable to compile student's executable" >> $vocareumReportFile
	echo "SimpleCommand1   ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleCommand2   ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleArguments1 ...   Untestable [0/10]" >> $vocareumReportFile
	echo "SimpleArguments2 ...   Untestable [0/10]" >> $vocareumReportFile
	echo "redirection      ...   Untestable [0/15]" >> $vocareumReportFile
	echo "multipleArgs     ...   Untestable [0/10]" >> $vocareumReportFile
	echo "simplePipe1      ...   Untestable [0/10]" >> $vocareumReportFile
	echo "advancedPipe1    ...   Untestable [0/10]" >> $vocareumReportFile
	echo "advancedPipe2    ...   Untestable [0/15]" >> $vocareumReportFile

	echo "SimpleCommand1,0" >> $vocareumGradeFile
	echo "SimpleCommand2,0" >> $vocareumGradeFile
	echo "SimpleArguments1,0" >> $vocareumGradeFile
	echo "SimpleArguments2,0" >> $vocareumGradeFile
	echo "redirection,0" >> $vocareumGradeFile
	echo "multipleArgs,0" >> $vocareumGradeFile
	echo "simplePipe1,0" >> $vocareumGradeFile
	echo "advancedPipe1,0" >> $vocareumGradeFile
	echo "advancedPipe2,0" >> $vocareumGradeFile

	exit 1
fi

echo "
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

import sys
import collections
import os
from subprocess import check_output
from optparse import OptionParser


# --------------------------------------------------------------------------- #
# Global Variables                                                            #
# --------------------------------------------------------------------------- #

executable_name = \"\"


def call_shell_output():
    pass

def main(argv):
    global executable_name

    executable_name = \"shell\"

    # Test whether options.executable exists
    try:
        test_f = open(\"./%s\" % executable_name, \"r\")
        test_f.close()
    except IOError as e:
        print \"Executable Doesn't Exist\"
        return

	test_simple_command_1()
	test_simple_command_2()
	test_simple_arguments_1()
	test_simple_arguments_2()
	test_redirection()
	test_multiple_args()
	test_simple_pipe_1()
	test_advanced_pipe_1()
	test_advanced_pipe_2()
	
	return


def test_simple_command_1():
    global executable_name

    command1 = \"ls\"
    command1_shell = \"echo \\\"ls\\\" | %s -t\" % executable_name

    cmd1_test = check_output(command1, shell=True)
    cmd1_shell = check_output(command1_shell, shell=True)

	if cmd1_test == cmd1_shell:
		print \"test_simple_command_1 Succeeded\"

def test_simple_command_2():
    global executable_name

    command2 = \"pwd\"
    command2_shell = \"echo \\\"pwd\\\" | %s -t\" % executable_name

    cmd2_test = check_output(command2, shell=True)
    cmd2_shell = check_output(command2_shell, shell=True)

	if cmd2_test == cmd2_shell:
		print \"test_simple_command_2 Succeeded\"

def test_simple_arguments_1():
    global executable_name

    command3 = \"ls -la\"
    command3_shell = \"echo \\\"ls -la\\\" | %s -t\" % executable_name

    cmd3_test = check_output(command3, shell=True)
    cmd3_shell = check_output(command3_shell, shell=True)

	if cmd3_test == cmd3_shell:
		print \"test_simple_arguments_1 Succeeded\"

def test_simple_arguments_2():
    global executable_name

    command4 = \"df -h\"
    command4_shell = \"echo \\\"df -h\\\" | %s -t\" % executable_name

    cmd4_test = check_output(command4, shell=True)
    cmd4_shell = check_output(command4_shell, shell=True)

	if cmd4_test == cmd4_shell:
		print \"test_simple_arguments_2 Succeeded\"

def test_redirection():
    global executable_name

    command5_shell = \"echo \\\"echo Hello World > temp_shell\\\" | %s -t\" % executable_name

    check_output(command5_shell, shell=True)

    command6 = \"cat < temp_shell\"
    command6_shell = \"echo \\\"cat < temp_shell\\\" | %s -t\" % executable_name

    cmd6_test = check_output(command6, shell=True)
    cmd6_shell = check_output(command6_shell, shell=True)

    os.unlink(\"./temp_shell\")

	if cmd6_test == cmd6_shell:
		print \"test_redirection Succeeded\"

def test_multiple_args():
    global executable_name

    check_output(\"echo \\\"Hello World\\\" > temp_shell\", shell=True)

    command7_shell = \"echo \\\"tar -c -v -f temp.tar temp_shell\\\" | %s -t\" % executable_name

    check_output(command7_shell, shell=True)

    command8 = \"ls -la | grep temp.tar\"
    command8_shell = \"echo \\\"ls -la | grep temp.tar\\\" | %s -t\" % executable_name

    cmd8_test = check_output(command8, shell=True)
    cmd8_shell = check_output(command8_shell, shell=True)

    os.unlink(\"./temp_shell\")
    os.unlink(\"./temp.tar\")

	if cmd8_test == cmd8_shell:
		print \"test_multiple_args Succeeded\"

def test_simple_pipe_1():
    global executable_name

    command9 = \"ls | cat\"
    command9_shell = \"echo \\\"ls | cat\\\" | %s -t\" % executable_name

    cmd9_test = check_output(command9, shell=True)
    cmd9_shell = check_output(command9_shell, shell=True)

	if cmd9_test == cmd9_shell:
		print \"test_simple_pipe_1 Succeeded\"

def test_advanced_pipe_1():
    global executable_name

    command10 = \"ls | cat | cat | cat\"
    command10_shell = \"echo \\\"ls | cat | cat | cat\\\" | %s -t\" % executable_name

    cmd10_test = check_output(command10, shell=True)
    cmd10_shell = check_output(command10_shell, shell=True)

	if cmd10_test == cmd10_shell:
		print \"test_advanced_pipe_1 Succeeded\"

def test_advanced_pipe_2():
    global executable_name

    command11 = \"ls -la | cat -A | cat -A | cat -A\"
    command11_shell = \"echo \\\"ls -la | cat -A | cat -A | cat -A\\\" | %s -t\" % executable_name

    cmd11_test = check_output(command11, shell=True)
    cmd11_shell = check_output(command11_shell, shell=True)

	if cmd11_test == cmd11_shell:
		print \"test_advanced_pipe_2 Succeeded\"

if __name__ == \"__main__\":
    main(sys.argv)
" > mp5_test.py

# Make sure that the python script doesn't have any whitespace issues
sed -i 's/\t/    /g' mp5_test.py

# ---------------------------------------------------------------------------- #
# Run Python Program                                                           # 
# ---------------------------------------------------------------------------- #

shell_output="$(python mp5_test.py)"

# ---------------------------------------------------------------------------- #
# test_simple_command_1                                                        # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_simple_command_1 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "SimpleCommand1   ...   Passed [10/10]" >> $vocareumReportFile
	echo "SimpleCommand1,10" >> $vocareumGradeFile
else
	echo "SimpleCommand1   ...   Failed [0/10]" >> $vocareumReportFile
	echo "SimpleCommand1,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_simple_command_2                                                        # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_simple_command_2 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "SimpleCommand2   ...   Passed [10/10]" >> $vocareumReportFile
	echo "SimpleCommand2,10" >> $vocareumGradeFile	
else
	echo "SimpleCommand2   ...   Failed [0/10]" >> $vocareumReportFile
	echo "Simplecommand2,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_simple_arguments_1                                                      # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_simple_arguments_1 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "SimpleArguments1 ...   Passed [10/10]" >> $vocareumReportFile
	echo "SimpleArguments1,10" >> $vocareumGradeFile
else
	echo "SimpleArguments1 ...   Failed [0/10]" >> $vocareumReportFile
	echo "SimpleArguments1,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_simple_arguments_2                                                      # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_simple_arguments_2 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "SimpleArguments2 ...   Passed [10/10]" >> $vocareumReportFile
	echo "SimpleArguments2,10" >> $vocareumGradeFile
else
	echo "SimpleArguments2 ...   Failed [0/10]" >> $vocareumReportFile
	echo "SimpleArguments2,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_redirection                                                             # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_redirection Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "redirection      ...   Passed [15/15]" >> $vocareumReportFile
	echo "redirection,15" >> $vocareumGradeFile
else
	echo "redirection      ...   Failed [0/15]" >> $vocareumReportFile
	echo "redirection,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_multiple_args                                                           # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_multiple_args Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "multipleArgs     ...   Passed [10/10]" >> $vocareumReportFile
	echo "multipleArgs,10" >> $vocareumGradeFile
else
	echo "multipleArgs     ...   Failed [0/10]" >> $vocareumReportFile
	echo "multipleArgs,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_simple_pipe_1                                                           # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_simple_pipe_1 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "simplePipe1      ...   Passed [10/10]" >> $vocareumReportFile
	echo "simplePipe1,10" >> $vocareumGradeFile
else
	echo "simplePipe1      ...   Failed [0/10]" >> $vocareumReportFile
	echo "simplePipe1,10" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_advanced_pipe_1                                                         # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_advanced_pipe_1 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "advancedPipe1    ...   Passed [10/10]" >> $vocareumReportFile
	echo "advancedPipe1,10" >> $vocareumGradeFile
else
	echo "advancedPipe1    ...   Failed [0/10]" >> $vocareumReportFile
	echo "advancedPipe1,0" >> $vocareumGradeFile
fi

# ---------------------------------------------------------------------------- #
# test_advanced_pipe_2                                                         # 
# ---------------------------------------------------------------------------- #

IS_SUCCESS="$(echo $shell_output | grep 'test_advanced_pipe_2 Succeeded')"

if [ "$IS_SUCCESS" != "" ]; then
	echo "advancedPipe2    ...   Passed [15/15]" >> $vocareumReportFile
	echo "advancedPipe2,15" >> $vocareumGradeFile
else
	echo "advancedPipe2    ...   Failed [0/15]" >> $vocareumReportFile
	echo "advancedPipe2,0" >> $vocareumGradeFile
fi

echo "Full grading report: "
cat $vocareumReportFile 2> /dev/null | grep -v "VOC"

exit 0
