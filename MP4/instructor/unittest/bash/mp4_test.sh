#!/bin/bash

# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Machine Problem #4                                        #
#                                                                             #
# File: ./mp4_test.sh                                                         #
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Normal Color Definitions                                                    #
# --------------------------------------------------------------------------- #

BLACK=$(tput setaf 0)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 4)
LIME_YELLOW=$(tput setaf 190)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
WHITE=$(tput setaf 7)
BOLD=$(tput bold)
NORMAL=$(tput sgr0)
BLINK=$(tput blink)
REVERSE=$(tput smso)
UNDERLINE=$(tput smul)

# --------------------------------------------------------------------------- #
# Bold Color Definitions                                                      #
# --------------------------------------------------------------------------- #

BOLDBLACK=${BOLD}${BLACK}
BOLDRED=${BOLD}${RED}
BOLDGREEN=${BOLD}${GREEN}
BOLDYELLOW=${BOLD}${YELLOW}
BOLDLIME_YELLOW=${BOLD}${LIME_YELLOW}
BOLDBLUE=${BOLD}${BLUE}
BOLDMAGENTA=${BOLD}${MAGENTA}
BOLDCYAN=${BOLD}${CYAN}
BOLDWHITE=${BOLD}${WHITE}

# --------------------------------------------------------------------------- #
# Program Set Up                                                              #
# --------------------------------------------------------------------------- #

# Attempt to source the student's script so that his functions can be called
source ./proctest.sh testing

# If something went wrong, terminate with an error
if [ $? -ne 0 ]
then
    echo "[ERROR]: Student script not found!"
    exit 1
fi

# Enter a pid
printf "Enter a pid to test: "
read PID

# pid must be an integer
while ! [ "$PID" -eq "$PID" ]
do
    printf "Pid must be an integer, try again: "
    read PID
done

# Search the system to make sure that the process exists.  
ls "/proc/$PID" > /dev/null
if [ $? == 1 ]
then
    echo "[ERROR]: A Process with the given ID does not exist on this system! - Exiting!"
    exit 1
fi

# Find values for all fields
PID="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[0]' "/proc/$PID/stat")"
# In some bash shells, variables $PPID and $EUID are special, different names 
# chosen here so that nothing will go wrong.  
TEST_PPID="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[3]' "/proc/$PID/stat")"
TEST_EUID="$(grep 'Uid' /proc/$PID/status | awk '{print $3}')"
EGID="$(grep 'Gid' /proc/$PID/status | awk '{print $3}')"
RUID="$(grep 'Uid' /proc/$PID/status | awk '{print $2}')"
RGID="$(grep 'Gid' /proc/$PID/status | awk '{print $2}')"
FSUID="$(grep 'Uid' /proc/$PID/status | awk '{print $5}')"
FSGID="$(grep 'Gid' /proc/$PID/status | awk '{print $5}')"
STATE="$(cat "/proc/$PID/status" | grep "State" | sed 's/State:\t//')\n"
STATE="$(echo $STATE | sed 's/(.*)//g')"
STATE="$(echo $STATE | sed 's/[ \\n]//g')"
THREAD_COUNT="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[19]' "/proc/$PID/stat")"
THREAD_COUNT="$(echo $THREAD_COUNT | sed 's/[ \\n]//g')"
PRIORITY="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[17]' "/proc/$PID/stat")"
PRIORITY="$(echo $PRIORITY | sed 's/[ \\n]//g')"
NICENESS="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[18]' "/proc/$PID/stat")"
NICENESS="$(echo $NICENESS | sed 's/[ \\n]//g')"
STIME="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[14]' "/proc/$PID/stat")"
STIME="$(echo $STIME | sed 's/[ \\n]//g')"
UTIME="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[13]' "/proc/$PID/stat")"
UTIME="$(echo $UTIME | sed 's/[ \\n]//g')"
CSTIME="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[16]' "/proc/$PID/stat")"
CSTIME="$(echo $CSTIME | sed 's/[ \\n]//g')"
CUTIME="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[15]' "/proc/$PID/stat")"
#CUTIME="$(echo $CUTIME | sed 's/[ \\n]//g')"
STARTCODE="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[25]' "/proc/$PID/stat")"
STARTCODE="$(echo $STARTCODE | sed 's/[ \\n]//g')"
ENDCODE="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[25]' "/proc/$PID/stat")"
ENDCODE="$(echo $ENDCODE | sed 's/[ \\n]//g')"
ESP="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[28]' "/proc/$PID/stat")"
ESP="$(echo $ESP | sed 's/[ \\n]//g')"
EIP="$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[29]' "/proc/$PID/stat")"
EIP="$(echo $EIP | sed 's/[ \\n]//g')"
FILES=""
ls -la "/proc/$PID/fd" &> /dev/null
if [ $? -eq 2 ]
then
    FILES="permission denied"
else
    FILES=$(ls -la "/proc/$PID/fd" | wc -l)
    if [ $? -eq 0 ]
    then
        FILES=$(($OUTPUT - 3))
        printf "$OUTPUT"
    else
        printf "ERROR"
    fi
fi
VOLUNTARY=$(grep "voluntary" "/proc/$PID/status" | grep -v "non" | sed 's/voluntary_ctxt_switches:\t//')
NONVOLUNTARY=$(grep "nonvoluntary" "/proc/$PID/status" | sed 's/nonvoluntary_ctxt_switches:\t//')
LAST_CPU=$(perl -l -0777 -ne '@f = /\(.*\)|\S+/g; print $f[38]' "/proc/$PID/stat")
ALLOWED_CPUS=$(grep "Cpus_allowed_list" "/proc/$PID/status" | sed 's/Cpus_allowed_list:\t//')
MEM_ARRAY=""
cat /proc/$PID/smaps &> /dev/null
if [ $? -ne 0 ]
then
    MEM_ARRAY="permission denied"
else
    MEM_ARRAY=$(cat /proc/$PID/smaps  | grep -v '^[A-Z[:punct:]]')
fi

echo ""
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
printf "${BOLDCYAN}# Data Values For Process %-52d#\n${NORMAL}" "$PID"
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo ""
echo "${BOLDGREEN}1. Identifiers${NORMAL}"
echo "   PID: $PID"
echo "   PPID: $TEST_PPID"
echo "   EUID: $TEST_EUID"
echo "   EGID: $EGID"
echo "   RUID: $RUID"
echo "   RGID: $RGID"
echo "   FSUID: $FSUID"
echo "   FSGID: $FSGID"
echo ""
echo "${BOLDGREEN}2. State${NORMAL}"
echo "   State: $STATE"
echo ""
echo "${BOLDGREEN}3. Thread Information${NORMAL}"
echo "   Thread Count: $THREAD_COUNT"
echo ""
echo "${BOLDGREEN}4. Priority${NORMAL}"
echo "   Priority Number: $PRIORITY"
echo "   Niceness Value: $NICENESS"
echo ""
echo "${BOLDGREEN}5. Time Information${NORMAL}"
echo "   stime: $STIME"
echo "   utime: $UTIME"
echo "   cstime: $CSTIME"
echo "   cutime: $CUTIME"
echo ""
echo "${BOLDGREEN}6. Address Space${NORMAL}"
echo "   Startcode: $STARTCODE"
echo "   Endcode: $ENDCODE"
echo "   Stack Pointer: $ESP"
echo "   Instruction Pointer: $EIP"
echo ""
echo "${BOLDGREEN}7. Resources${NORMAL}"
echo "   File Handles: $FILES"
echo "   Voluntary Context Switches: $VOLUNTARY"
echo "   Nonvoluntary Context Switches: $NONVOLUNTARY"
echo ""
echo "${BOLDGREEN}8. Processors${NORMAL}"
echo "   Last Processor: $LAST_CPU"
echo "   Allowed Cores: $ALLOWED_CPUS"
echo ""
echo "${BOLDGREEN}9. Memory Map${NORMAL}"
while read -r line
do
    echo "   $line"
done <<< "$MEM_ARRAY"

# --------------------------------------------------------------------------- #
# Tests for program integrity                                                 #
# --------------------------------------------------------------------------- #

echo ""
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo "${BOLDCYAN}# Tests For Program Integrity                                                 #${NORMAL}"
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo ""

# Initialize counting variables
declare -i SUCCESSFUL_INTEGRITY_TESTS=0
declare -i FAILED_INTEGRITY_TESTS=0
declare -i TOTAL_INTEGRITY_TESTS=26

# 1. getpid
echo "${BOLDGREEN}1.  Testing getpid...${NORMAL}"
getpid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getpid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getpid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 2. getppid
echo ""
echo "${BOLDGREEN}2.  Testing getppid...${NORMAL}"
getppid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getppid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getppid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 3. geteuid
echo ""
echo "${BOLDGREEN}3.  Testing geteuid...${NORMAL}"
geteuid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}geteuid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}geteuid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 4. getegid
echo ""
echo "${BOLDGREEN}4.  Testing getegid...${NORMAL}"
getegid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getegid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getegid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 5. getruid
echo ""
echo "${BOLDGREEN}5.  Testing getruid...${NORMAL}"
getruid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getruid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getruid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 6. getrgid
echo ""
echo "${BOLDGREEN}6.  Testing getrgid...${NORMAL}"
getrgid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getrgid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getrgid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 7. getfsuid
echo ""
echo "${BOLDGREEN}7.  Testing getfsuid...${NORMAL}"
getfsuid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getfsuid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getfsuid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 8. getfsgid
echo ""
echo "${BOLDGREEN}8.  Testing getfsgid...${NORMAL}"
getfsgid &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getfsgid${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getfsgid${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 9. getstate
echo ""
echo "${BOLDGREEN}9.  Testing getstate...${NORMAL}"
getstate &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getstate${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getstate existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 10. getthread_count
echo ""
echo "${BOLDGREEN}10. Testing getthread_count...${NORMAL}"
getthread_count &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getthread_count${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getthread_count${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 11. getpriority
echo ""
echo "${BOLDGREEN}11. Testing getpriority...${NORMAL}"
getpriority &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getpriority${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getpriority${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 12. getniceness
echo ""
echo "${BOLDGREEN}12. Testing getniceness...${NORMAL}"
getniceness &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getniceness${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getniceness${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 13. getstime
echo ""
echo "${BOLDGREEN}13. Testing getstime...${NORMAL}"
getstime &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getstime${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getstime${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 14. getutime
echo ""
echo "${BOLDGREEN}14. Testing getutime...${NORMAL}"
getutime &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getutime${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getutime${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 15. getcstime
echo ""
echo "${BOLDGREEN}15. Testing getcstime...${NORMAL}"
getcstime &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getcstime${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getcstime${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 16. getcutime
echo ""
echo "${BOLDGREEN}16. Testing getcutime...${NORMAL}"
getcutime &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getcutime${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getcutime${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 17. getstartcode
echo ""
echo "${BOLDGREEN}17. Testing getstartcode...${NORMAL}"
getstartcode &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getstartcode${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getstartcode${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 18. getendcode
echo ""
echo "${BOLDGREEN}18. Testing getendcode...${NORMAL}"
getendcode &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getendcode${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getendcode${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 19. getesp
echo ""
echo "${BOLDGREEN}19. Testing getesp...${NORMAL}"
getesp &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getesp${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getesp${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 20. geteip
echo ""
echo "${BOLDGREEN}20. Testing geteip...${NORMAL}"
geteip &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}geteip${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}geteip${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 21. getfiles
echo ""
echo "${BOLDGREEN}21. Testing getfiles...${NORMAL}"
getfiles &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getfiles${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getfiles${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 22. getvoluntary_context_switches
echo ""
echo "${BOLDGREEN}22. Testing getvoluntary_context_switches...${NORMAL}"
getvoluntary_context_switches &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getvoluntary_context_switches${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getvoluntary_context_switches${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 23. getnonvoluntary_context_switches
echo ""
echo "${BOLDGREEN}23. Testing getnonvoluntary_context_switches...${NORMAL}"
getnonvoluntary_context_switches &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getnonvoluntary_context_switches${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getnonvoluntary_context_switches${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 24. getlast_cpu
echo ""
echo "${BOLDGREEN}24. Testing getlast_cpu...${NORMAL}"
getlast_cpu &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getlast_cpu${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getlast_cpu${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 25. getallowed_cpus
echo ""
echo "${BOLDGREEN}25. Testing getallowed_cpus...${NORMAL}"
getallowed_cpus &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getallowed_cpus${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getallowed_cpus${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# 26. getmemory_map
echo ""
echo "${BOLDGREEN}26. Testing getmemory_map...${NORMAL}"
getmemory_map &> /dev/null
if [ $? -ne 127 ]
then
    echo "    ${BOLDCYAN}getmemory_map${NORMAL} existence test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_INTEGRITY_TESTS=$SUCCESSFUL_INTEGRITY_TESTS+1
else
    echo "    ${BOLDCYAN}getmemory_map${NORMAL} existence test ${RED}failed!${NORMAL}"
    let FAILED_INTEGRITY_TESTS=$FAILED_INTEGRITY_TESTS+1
fi

# --------------------------------------------------------------------------- #
# Tests for program correctness                                               #
# --------------------------------------------------------------------------- #

echo ""
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo "${BOLDCYAN}# Tests For Program Correctness                                               #${NORMAL}"
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo ""

# Initialize counting variables
declare -i SUCCESSFUL_CORRECTNESS_TESTS=0
declare -i FAILED_CORRECTNESS_TESTS=0
declare -i TOTAL_CORRECTNESS_TESTS=26

# 1. PID
echo "${BOLDGREEN}1.  Testing PID...${NORMAL}"
if [ "$(getpid)" == "$PID" ]
then
    echo "    ${BOLDCYAN}PID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCESSFUL_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}PID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 2. TEST_PPID
echo ""
echo "${BOLDGREEN}2.  Testing PPID...${NORMAL}"
if [ "$(getppid)" == "$TEST_PPID" ]
then
    echo "    ${BOLDCYAN}PPID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}PPID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
fi

# 3. EUID
echo ""
echo "${BOLDGREEN}3.  Testing EUID...${NORMAL}"
if [ "$(geteuid)" == "$TEST_EUID" ]
then
    echo "    ${BOLDCYAN}EUID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}EUID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 4. EGID
echo ""
echo "${BOLDGREEN}4.  Testing EGID...${NORMAL}"
if [ "$(getegid)" == "$EGID" ]
then
    echo "    ${BOLDCYAN}EGID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCESSFUL_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}EGID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 5. RUID
echo ""
echo "${BOLDGREEN}5.  Testing RUID...${NORMAL}"
if [ "$(getruid)" == "$RUID" ]
then
    echo "    ${BOLDCYAN}RUID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}RUID${NORMAL} test ${WHITE}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 6. RGID
echo ""
echo "${BOLDGREEN}6.  Testing RGID...${NORMAL}"
if [ "$(getrgid)" == "$RGID" ]
then
    echo "    ${BOLDCYAN}RGID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}RGID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 7. FSUID
echo ""
echo "${BOLDGREEN}7.  Testing FSUID...${NORMAL}"
if [ "$(getfsuid)" == "$FSUID" ]
then
    echo "    ${BOLDCYAN}FSUID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}FSUID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 8. FSGID
echo ""
echo "${BOLDGREEN}8.  Testing FSGID...${NORMAL}"
if [ "$(getfsgid)" == "$FSGID" ]
then
    echo "    ${BOLDCYAN}FSGID${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCESSFUL_TESTS+1
else
    echo "    ${BOLDCYAN}FSGID${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_Tests+1
fi

# 9. State
echo ""
echo "${BOLDGREEN}9.  Testing State...${NORMAL}"
if [ "$(getstate)" == "$STATE" ]
then
    echo "    ${BOLDCYAN}State${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}State${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 10. Thread Count
echo ""
echo "${BOLDGREEN}10. Testing Thread_count...${NORMAL}"
if [ "$(getthread_count)" == "$THREAD_COUNT" ]
then
    echo "    ${BOLDCYAN}Thread_count${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Thread_count${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 11. Priority
echo ""
echo "${BOLDGREEN}11. Testing Priority...${NORMAL}"
if [ "$(getpriority)" == "$PRIORITY" ]
then
    echo "    ${BOLDCYAN}Priority${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Priority${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 12. Niceness
echo ""
echo "${BOLDGREEN}12. Testing Niceness...${NORMAL}"
if [ "$(getniceness)" == "$NICENESS" ]
then
    echo "    ${BOLDCYAN}Niceness${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Niceness${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 13. stime
echo ""
echo "${BOLDGREEN}13. Testing stime...${NORMAL}"
if [ "$(getstime)" == "$STIME" ]
then
    echo "    ${BOLDCYAN}stime${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}stime${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 14. utime
echo ""
echo "${BOLDGREEN}14. Testing utime...${NORMAL}"
if [ "$(getutime)" == "$UTIME" ]
then
    echo "    ${BOLDCYAN}utime${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}utime${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 15. cstime
echo ""
echo "${BOLDGREEN}15. Testing cstime...${NORMAL}"
if [ "$(getcstime)" == "$CSTIME" ]
then
    echo "    ${BOLDCYAN}cstime${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}cstime${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 16. cutime
echo ""
echo "${BOLDGREEN}16. Testing cutime...${NORMAL}"
if [ "$(getcutime)" == "$CUTIME" ]
then
    echo "    ${BOLDCYAN}cutime${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCESSFUL_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}cutime${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 17. Startcode
echo ""
echo "${BOLDGREEN}17. Testing Startcode...${NORMAL}"
if [ "$(getstartcode)" == "$STARTCODE" ]
then
    echo "    ${BOLDCYAN}Startcode${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Startcode${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 18. Endcode
echo ""
echo "${BOLDGREEN}18. Testing Endcode...${NORMAL}"
if [ "$(getendcode)" == "$ENDCODE" ]
then
    echo "    ${BOLDCYAN}Endcode${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Endcode${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 19. ESP
echo ""
echo "${BOLDGREEN}19. Testing ESP...${NORMAL}"
if [ "$(getesp)" == "$ESP" ]
then
    echo "    ${BOLDCYAN}ESP${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}ESP${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 20. EIP
echo ""
echo "${BOLDGREEN}20. Testing EIP...${NORMAL}"
if [ "$(geteip)" == "$EIP" ]
then
    echo "    ${BOLDCYAN}EIP${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}EIP${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 21. Files
echo ""
echo "${BOLDGREEN}21. Testing Files...${NORMAL}"
if [ "$FILES" == "permission denied" ]
then
    echo "    ${BOLDCYAN}File${NORMAL} test ${MAGENTA}permission denied!${NORMAL}"
    let TOTAL_CORRECTNESS_TESTS=$TOTAL_CORRECTNESS_TESTS-1
else
    if [ "$(getfiles)" == "Files" ]
    then
        echo "    ${BOLDCYAN}File${NORMAL} test ${WHITE}succeeded!${NORMAL}"
        let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
    else
        echo "    ${BOLDCYAN}File${NORMAL} test ${RED}failed!${NORMAL}"
        let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
    fi
fi

# 22. Voluntary Context Switches
echo ""
echo "${BOLDGREEN}22. Testing Voluntary Context Switches...${NORMAL}"
if [ "$(getvoluntary_context_switches)" == "$VOLUNTARY" ]
then
    echo "    ${BOLDCYAN}Voluntary context switch${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCESSFUL_TESTS+1
else
    echo "    ${BOLDCYAN}Voluntary context switch${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 23. Nonvoluntary Context Switches
echo ""
echo "${BOLDGREEN}23. Testing Nonvoluntary Context Switches...${NORMAL}"
if [ "$(getnonvoluntary_context_switches)" == "$NONVOLUNTARY" ]
then
    echo "    ${BOLDCYAN}Nonvoluntary context switch${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Nonvoluntary context switch${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 24. Last CPUs
echo ""
echo "${BOLDGREEN}24. Testing Last CPU...${NORMAL}"
if [ "$(getlast_cpu)" == "$LAST_CPU" ]
then
    echo "    ${BOLDCYAN}Last CPU${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Last CPU${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 25. Allowed CPUs
echo ""
echo "${BOLDGREEN}25. Testing Allowed CPUs...${NORMAL}"
if [ "$(getallowed_cpus)" == "$ALLOWED_CPUS" ]
then
    echo "    ${BOLDCYAN}Allowed CPUs${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    let SUCCESSFUL_CORRECTNESS_TESTS=$SUCCESSFUL_CORRECTNESS_TESTS+1
else
    echo "    ${BOLDCYAN}Allowed CPUs${NORMAL} test ${RED}failed!${NORMAL}"
    let FAILED_CORRECTNESS_TESTS=$FAILED_CORRECTNESS_TESTS+1
fi

# 26. Memory Map
echo ""
echo "${BOLDGREEN}26. Testing Memory Map...${NORMAL}"
if [ "${MEM_ARRAY[0]}" == "permission denied" ]
then
    echo "    ${BOLDCYAN}Memory Map${NORMAL} test ${MAGENTA}permission denied!${NORMAL}"
else
    SUCCESS=true
    #MEMORY_STRING="$(getmemory_map)"
    MEMORY_STRING=""
    for item in $MEM_ARRAY
    do
        echo "   $item"
        if [[ ${MEMORY_STRING} != *"$item"* ]]
        then
            echo "THINGTHINGTHINGTHINGTHINGTHINGTHING"
            # Variable scope problem here, need to rework so there is a way to pass values
            # back to the primary shell.  
            SUCCESS=false
        fi
    done
    if [ $SUCCESS ]
    then
        echo "    ${BOLDCYAN}Memory Map${NORMAL} test ${WHITE}succeeded!${NORMAL}"
    else
        echo "    ${BOLDCYAN}Memory Map${NORMAL} test ${RED}failed!${NORMAL}"
    fi
fi

echo ""
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo "${BOLDCYAN}# Final Statistics                                                            #${NORMAL}"
echo "${BOLDCYAN}# --------------------------------------------------------------------------- #${NORMAL}"
echo ""
echo "${BOLDGREEN}Integrity Statistics${NORMAL}"
printf "  - Passed Tests: %2d / %2d\n" "$SUCCESSFUL_INTEGRITY_TESTS" "$TOTAL_INTEGRITY_TESTS"
printf "  - Failed Tests: %2d / %2d\n" "$FAILED_INTEGRITY_TESTS" "$TOTAL_INTEGRITY_TESTS"
echo ""
echo "${BOLDGREEN}Correctness Statistics${NORMAL}"
echo "  - Passed Tests: $SUCCESSFUL_CORRECTNESS_TESTS / $TOTAL_CORRECTNESS_TESTS"
echo "  - Failed Tests: $FAILED_CORRECTNESS_TESTS / $TOTAL_CORRECTNESS_TESTS"
TOTAL_PASSED_TESTS=$(($SUCCESSFUL_INTEGRITY_TESTS+$SUCCESSFUL_CORRECTNESS_TESTS))
TOTAL_FAILED_TESTS=$(($FAILED_INTEGRITY_TESTS+$FAILED_CORRECTNESS_TESTS))
TOTAL_TESTS=$(($TOTAL_INTEGRITY_TESTS+$TOTAL_CORRECTNESS_TESTS))
echo ""
echo "${BOLDGREEN}Overall Statistics: ${NORMAL}"
echo "  - Passed Tests: $TOTAL_PASSED_TESTS / $TOTAL_TESTS"
echo "  - Failed Tests: $TOTAL_FAILED_TESTS / $TOTAL_TESTS"
echo ""

exit 0
