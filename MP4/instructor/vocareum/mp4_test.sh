# ---------------------------------------------------------------------------- #
# Select Language                                                              # 
# ---------------------------------------------------------------------------- #

# Note: Students shoudl have the opportunity to write this assignment in a number
# of languages.  So far, C++, Python, and BASH (Bourne Again Shell) are supported.  
# This script autodetects the file that the students provided and continues from there.  

CHOSEN_LANGUAGE=""

cat proctest.sh &> /dev/null

if [ "$?" == "0" ]; then
	CHOSEN_LANGUAGE="BASH"
fi

cat proctest.cpp &> /dev/null

if [ "$?" == "0" ]; then
	CHOSEN_LANGUAGE="C++"
fi

cat proctest.py &> /dev/null

if [ "$?" == "0" ]; then
	CHOSEN_LANGHAGE="Python"
fi

if [ "$CHOSEN_LANGUAGE" == "" ]; then
	echo "Program file not found" >> $vocareumReportFile
    echo "getpid        ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getppid       ...   Untestable [0/5]" >> $vocareumReportFile
    echo "geteuid       ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getegid       ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getruid       ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getrgid       ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getfsuid      ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getfsgid      ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getstate      ...   Untestable [0/5]" >> $vocareumReportFile
	echo "getthread     ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getpriority   ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getniceness   ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getstime      ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getutime      ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getcstime     ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getcutime     ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getstartcode  ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getendcode    ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getesp        ...   Untestable [0/5]" >> $vocareumReportFile
    echo "geteip        ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getfiles      ...   Untestable [0/20]" >> $vocareumReportFile
	echo "getvoluntary  ...   Untestable [0/5]" >> $vocareumReportFile
	echo "getnonvol     ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getcontext_sw ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getallowed_cp ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getlast_cpu   ...   Untestable [0/5]" >> $vocareumReportFile
    echo "getmemory_map ...   Untestable [0/20]" >> $vocareumReportFile
    
    echo "getpid,0" >> $vocareumGradeFile
    echo "getppid,0" >> $vocareumGradeFile
    echo "geteuid,0" >> $vocareumGradeFile
    echo "getegid,0" >> $vocareumGradeFile
    echo "getruid,0" >> $vocareumGradeFile
    echo "getrgid,0" >> $vocareumGradeFile
    echo "getfsuid,0" >> $vocareumGradeFile
    echo "getfsgid,0" >> $vocareumGradeFile
    echo "getstate,0" >> $vocareumGradeFile
	echo "getthread_count,0" >> $vocareumGradeFile
    echo "getpriority,0" >> $vocareumGradeFile
    echo "getniceness,0" >> $vocareumGradeFile
    echo "getstime,0" >> $vocareumGradeFile
    echo "getutime,0" >> $vocareumGradeFile
    echo "getcstime,0" >> $vocareumGradeFile
    echo "getcutime,0" >> $vocareumGradeFile
    echo "getstartcode,0" >> $vocareumGradeFile
    echo "getendcode,0" >> $vocareumGradeFile
    echo "getesp,0" >> $vocareumGradeFile
    echo "geteip,0" >> $vocareumGradeFile
    echo "getfiles,0" >> $vocareumGradeFile
	echo "getvoluntary,0" >> $vocareumGradeFile
	echo "getnonvol,0" >> $vocareumGradeFile
    echo "getcontext_switches,0" >> $vocareumGradeFile
    echo "getallowed_cpus,0" >> $vocareumGradeFile
    echo "getlast_cpu,0" >> $vocareumGradeFile
    echo "getmemory_map,0" >> $vocareumGradeFile

	exit 1
fi

# ---------------------------------------------------------------------------- #
# BASH                                                                         # 
# ---------------------------------------------------------------------------- #

if [ "$CHOSEN_LANGUAGE" == "BASH" ]; then

	# ---------------------------------------------------------------------------- #
	# Source Student's Executable                                                  # 
	# ---------------------------------------------------------------------------- #

	source ./proctest.sh testing

	if [ "$?" != "0" ]; then

		echo "Can't Source Student Executable" >> $vocareumReportFile
    	echo "getpid        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getppid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "geteuid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getegid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getruid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getrgid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfsuid      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfsgid      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstate      ...   Untestable [0/5]" >> $vocareumReportFile
		echo "getthread     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getpriority   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getniceness   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstime      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getutime      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcstime     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcutime     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstartcode  ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getendcode    ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getesp        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "geteip        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfiles      ...   Untestable [0/20]" >> $vocareumReportFile
		echo "getvoluntary  ...   Untestable [0/5]" >> $vocareumReportFile
		echo "getnonvol     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcontext_sw ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getallowed_cp ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getlast_cpu   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getmemory_map ...   Untestable [0/20]" >> $vocareumReportFile
    
   		echo "getpid,0" >> $vocareumGradeFile
    	echo "getppid,0" >> $vocareumGradeFile
    	echo "geteuid,0" >> $vocareumGradeFile
    	echo "getegid,0" >> $vocareumGradeFile
    	echo "getruid,0" >> $vocareumGradeFile
    	echo "getrgid,0" >> $vocareumGradeFile
    	echo "getfsuid,0" >> $vocareumGradeFile
    	echo "getfsgid,0" >> $vocareumGradeFile
    	echo "getstate,0" >> $vocareumGradeFile
		echo "getthread_count,0" >> $vocareumGradeFile
    	echo "getpriority,0" >> $vocareumGradeFile
    	echo "getniceness,0" >> $vocareumGradeFile
    	echo "getstime,0" >> $vocareumGradeFile
    	echo "getutime,0" >> $vocareumGradeFile
    	echo "getcstime,0" >> $vocareumGradeFile
    	echo "getcutime,0" >> $vocareumGradeFile
    	echo "getstartcode,0" >> $vocareumGradeFile
    	echo "getendcode,0" >> $vocareumGradeFile
    	echo "getesp,0" >> $vocareumGradeFile
    	echo "geteip,0" >> $vocareumGradeFile
    	echo "getfiles,0" >> $vocareumGradeFile
		echo "getvoluntary,0" >> $vocareumGradeFile
		echo "getnonvol,0" >> $vocareumGradeFile
    	echo "getcontext_switches,0" >> $vocareumGradeFile
    	echo "getallowed_cpus,0" >> $vocareumGradeFile
    	echo "getlast_cpu,0" >> $vocareumGradeFile
    	echo "getmemory_map,0" >> $vocareumGradeFile

		exit 1
	fi

	# ---------------------------------------------------------------------------- #
	# Start a Process                                                              # 
	# ---------------------------------------------------------------------------- #

	# Start a dummy process
	sleep 100 &
	
	# The pid of the last command is stored in the shell variable $!
	PID=$!

	# ---------------------------------------------------------------------------- #
	# Get Process Information                                                      # 
	# ---------------------------------------------------------------------------- #

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
            FILES=$(($FILES - 3))
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

	# ---------------------------------------------------------------------------- #
	# getpid                                                                       # 
	# ---------------------------------------------------------------------------- #

	STUDENT_PID="$(getpid)"
	
	if [ "$PID" == "$STUDENT_PID" ]; then
    	echo "getpid        ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getpid,5" >> $vocareumGradeFile
	else
		echo "getpid        ...   Failed [0/5]" >> $vocareumReportFile
		echo "getpid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getppid                                                                      # 
	# ---------------------------------------------------------------------------- #

	STUDENT_PPID="$(getppid)"

	if [ "$TEST_PPID" == "$STUDENT_PPID" ]; then
    	echo "getppid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "getppid,5" >> $vocareumGradeFile
	else
		echo "getppid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getppid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# geteuid                                                                      # 
	# ---------------------------------------------------------------------------- #	

	STUDENT_EUID="$(geteuid)"

	if [ "$TEST_EUID" == "$STUDENT_EUID" ]; then
		echo "geteuid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "geteuid,5" >> $vocareumGradeFile
	else
		echo "geteuid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "geteuid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getegid                                                                      # 
	# ---------------------------------------------------------------------------- #

	STUDENT_EGID="$(getegid)"

	if [ "$EGID" == "$STUDENT_EGID" ]; then
		echo "getegid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "getegid,5" >> $vocareumGradeFile
	else
		echo "getegid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getegid,0" >> $vocareumGradeFile

	fi

	# ---------------------------------------------------------------------------- #
	# getruid                                                                      # 
	# ---------------------------------------------------------------------------- #

	STUDENT_RUID="$(getruid)"

	if [ "$RUID" == "$STUDENT_RUID" ]; then
		echo "getruid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "getruid,5" >> $vocareumGradeFile
	else
		echo "getruid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getruid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getrgid                                                                      # 
	# ---------------------------------------------------------------------------- #

	STUDENT_RGID="$(getrgid)"

	if [ "$RGID" == "$STUDENT_RGID" ]; then
		echo "getrgid       ...   Passed [5/5]" >> $vocareumReportFile   
		echo "getrgid,5" >> $vocareumGradeFile
	else
		echo "getrgid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getrgid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getfsuid                                                                     # 
	# ---------------------------------------------------------------------------- #

	STUDENT_FSUID="$(getfsuid)"

	if [ "$FSUID" == "$STUDENT_FSUID" ]; then
		echo "getfsuid      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getfsuid,5" >> $vocareumGradeFile
	else
		echo "getfsuid      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getfsuid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getfsgid                                                                     # 
	# ---------------------------------------------------------------------------- #

	STUDENT_FSGID="$(getfsgid)"
	
	if [ "$FSGID" == "$STUDENT_FSGID" ]; then
		echo "getfsgid      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getfsgid,5" >> $vocareumGradeFile
	else
		echo "getfsgid      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getfsgid,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getstate                                                                     # 
	# ---------------------------------------------------------------------------- #

	STUDENT_STATE="$(getstate)"
	
	if [ "$STATE" == "$STUDENT_STATE" ]; then
		echo "getstate      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getstate,5" >> $vocareumGradeFile
	else
		echo "getstate      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getstate,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getthread_count                                                              # 
	# ---------------------------------------------------------------------------- #
	
	STUDENT_THREAD_COUNT="$(getthread_count)"

	if [ "$THREAD_COUNT" == "$STUDENT_THREAD_COUNT" ]; then
        echo "getthread     ...   Passed [5/5]" >> $vocareumReportFile
        echo "getthread_count,5" >> $vocareumGradeFile
	else
		echo "getthread     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getthread_count,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getpriority                                                                  # 
	# ---------------------------------------------------------------------------- #

	STUDENT_PRIORITY="$(getpriority)"

	if [ "$PRIORITY" == "$STUDENT_PRIORITY" ]; then
		echo "getpriority   ...   Passed [5/5]" >> $vocareumReportFile
		echo "getpriority,5" >> $vocareumGradeFile
	else
		echo "getpriority   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getpriority,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getniceness                                                                  # 
	# ---------------------------------------------------------------------------- #

	STUDENT_NICENESS="$(getniceness)"

	if [ "$NICENESS" == "$STUDENT_NICENESS" ]; then
		echo "getniceness   ...   Passed [5/5]" >> $vocareumReportFile
		echo "getniceness,5" >> $vocareumGradeFile
	else
		echo "getniceness   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getniceness,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getstime                                                                     # 
	# ---------------------------------------------------------------------------- #

	STUDENT_STIME="$(getstime)"

	if [ "$STIME" == "$STUDENT_STIME" ]; then
		echo "getstime      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getstime,5" >> $vocareumGradeFile
	else
		echo "getstime      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getstime,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getutime                                                                     # 
	# ---------------------------------------------------------------------------- #

	STUDENT_UTIME="$(getutime)"

	if [ "$UTIME" == "$STUDENT_UTIME" ]; then
		echo "getutime      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getutime,5" >> $vocareumGradeFile
	else
		echo "getutime      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getutime,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getcstime                                                                    # 
	# ---------------------------------------------------------------------------- #

	STUDENT_CSTIME="$(getcstime)"

	if [ "$CSTIME" == "$STUDENT_CSTIME" ]; then
		echo "getcstime     ...   Passed [5/5]" >> $vocareumReportFile
		echo "getcstime,5" >> $vocareumGradeFile
	else
		echo "getcstime     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getcstime,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getcutime                                                                    # 
	# ---------------------------------------------------------------------------- #

	STUDENT_CUTIME="$(getcutime)"

	if [ "$CUTIME" == "$STUDENT_CUTIME" ]; then
		echo "getcutime     ...   Passed [5/5]" >> $vocareumReportFile
		echo "getcutime,5" >> $vocareumGradeFile
	else
		echo "getcutime     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getcutime,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getstartcode                                                                 # 
	# ---------------------------------------------------------------------------- #

	STUDENT_STARTCODE="$(getstartcode)"

	if [ "$STARTCODE" == "$STUDENT_STARTCODE" ]; then
		echo "getstartcode  ...   Passed [5/5]" >> $vocareumReportFile
		echo "getstartcode,5" >> $vocareumGradeFile
	else
		echo "getstartcode  ...   Failed [0/5]" >> $vocareumReportFile
		echo "getstartcode,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getendcode                                                                   # 
	# ---------------------------------------------------------------------------- #

	STUDENT_ENDCODE="$(getendcode)"

	if [ "$ENDCODE" == "$STUDENT_ENDCODE" ]; then
		echo "getendcode    ...   Passed [5/5]" >> $vocareumReportFile
		echo "getendcode,5" >> $vocareumGradeFile
	else
		echo "getendcode    ...   Failed [0/5]" >> $vocareumReportFile
		echo "getendcode,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getesp                                                                       # 
	# ---------------------------------------------------------------------------- #

	STUDENT_ESP="$(getesp)"

	if [ "$ESP" == "$STUDENT_ESP" ]; then
		echo "getesp        ...   Passed [5/5]" >> $vocareumReportFile
		echo "getesp,5" >> $vocareumGradeFile
	else
		echo "getesp        ...   Failed [0/5]" >> $vocareumReportFile
		echo "getesp,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# geteip                                                                       # 
	# ---------------------------------------------------------------------------- #

	STUDENT_EIP="$(geteip)"

	if [ "$EIP" == "$STUDENT_EIP" ]; then
		echo "geteip        ...   Passed [5/5]" >> $vocareumReportFile
		echo "geteip,5" >> $vocareumGradeFile
	else
		echo "geteip        ...   Failed [0/5]" >> $vocareumReportFile
		echo "geteip,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getfiles                                                                     # 
	# ---------------------------------------------------------------------------- #	

	STUDENT_FILES="$(getfiles)"

	if [ "$FILES" == "$STUDENT_FILES" ]; then
		echo "getfiles      ...   Passed [5/5]" >> $vocareumReportFile
		echo "getfiles,20" >> $vocareumGradeFile
	else
		echo "getfiles      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getfiles,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getvoluntary_context_switches                                                # 
	# ---------------------------------------------------------------------------- #

	STUDENT_VOLUNTARY="$(getvoluntary_context_switches)"

	if [ "$VOLUNTARY" == "$STUDENT_VOLUNTARY" ]; then
		echo "getvoluntary  ...   Passed [5/5]" >> $vocareumReportFile
		echo "getvoluntary,5" >> $vocareumGradeFile
	else
        echo "getvoluntary  ...   Failed [0/5]" >> $vocareumReportFile
        echo "getvoluntary,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getnonvoluntary_context_switches                                             # 
	# ---------------------------------------------------------------------------- #

	STUDENT_NONVOLUNTARY="$(getnonvoluntary_context_switches)"
	
	if [ "$NONVOLUNTARY" == "$STUDENT_NONVOLUNTARY" ]; then
		echo "getnonvol     ...   Passed [5/5]" >> $vocareumReportFile
		echo "getnonvol,5" >> $vocareumGradeFile
	else
		echo "getnonvol     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getnonvol,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getlast_cpu                                                                  # 
	# ---------------------------------------------------------------------------- #

	STUDENT_LAST_CPU="$(getlast_cpu)"

	if [ "$LAST_CPU" == "$STUDENT_LAST_CPU" ]; then
		echo "getlast_cpu   ...   Passed [5/5]" >> $vocareumReportFile
		echo "getlast_cpu,5" >> $vocareumGradeFile
	else
		echo "getlast_cpu   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getlast_cpu,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getallowed_cpus                                                              # 
	# ---------------------------------------------------------------------------- #

	STUDENT_ALLOWED_CPUS="$(getallowed_cpus)"

	if [ "$ALLOWED_CPUS" == "$STUDENT_ALLOWED_CPUS" ]; then
		echo "getallowed    ...   Passed [5/5]" >> $vocareumReportFile
		echo "getallowed_cpus,5" >> $vocareumGradeFile
	else
		echo "getallowed    ...   Failed [0/5]" >> $vocareumReportFile
		echo "getallowed_cpus,0" >> $vocareumGradeFile
	fi

	# ---------------------------------------------------------------------------- #
	# getmemory_map                                                                # 
	# ---------------------------------------------------------------------------- #

	# Note: 
	# This portion of the assignment is rather difficult to grade.  
	# It may be necessary to grade the student's memory map function manually.  
	# Come back to this when the assignment is actually issued to students.  

	SUCCESS=true
    MEMORY_STRING="$(getmemory_map)"

    for item in $MEM_ARRAY
    do
        if [[ ${MEMORY_STRING} != *"$item"* ]]
        then
            # Variable scope problem here, need to rework so there is a way to pass values
            # back to the primary shell.  
            SUCCESS=false
        fi
    done
	
    if [ "$SUCCESS" == "true" ]; then
		echo "getmemory     ...   Passed [20/20]" >> $vocareumReportFile
		echo "getmemory_map,20" >> $vocareumGradeFile
	else
		echo "getmemory     ...   Failed [0/20]" >> $vocareumReportFile
		echo "getmemory_map,0" >> $vocareumGradeFile
    fi

	exit 0
fi

# ---------------------------------------------------------------------------- #
# C++                                                                          # 
# ---------------------------------------------------------------------------- #

if [ "$CHOSEN_LANGUAGE" == "C++" ]; then
	echo "Flag2"

	# ---------------------------------------------------------------------------- #
	# Check Student's Source Files                                                 # 
	# ---------------------------------------------------------------------------- #	

	FILE_NOT_FOUND=false

	cat proctest.cpp &> /dev/null

	if [ "$?" != "0" ]; then
		FILE_NOT_FOUND=true
	fi

	cat proctest.h &> /dev/null

	if [ "$?" != "0" ]; then
		FILE_NOT_FOUND=true
	fi

	if [ "$FILE_NOT_FOUND" == "true" ]; then
		echo "Program File(s) Not Found" >> $vocareumReportFile
    	echo "getpid        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getppid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "geteuid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getegid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getruid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getrgid       ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfsuid      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfsgid      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstate      ...   Untestable [0/5]" >> $vocareumReportFile
		echo "getthread     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getpriority   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getniceness   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstime      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getutime      ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcstime     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcutime     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getstartcode  ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getendcode    ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getesp        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "geteip        ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getfiles      ...   Untestable [0/20]" >> $vocareumReportFile
		echo "getvoluntary  ...   Untestable [0/5]" >> $vocareumReportFile
		echo "getnonvol     ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getcontext_sw ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getallowed_cp ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getlast_cpu   ...   Untestable [0/5]" >> $vocareumReportFile
    	echo "getmemory_map ...   Untestable [0/20]" >> $vocareumReportFile
    
    	echo "getpid,0" >> $vocareumGradeFile
    	echo "getppid,0" >> $vocareumGradeFile
    	echo "geteuid,0" >> $vocareumGradeFile
    	echo "getegid,0" >> $vocareumGradeFile
    	echo "getruid,0" >> $vocareumGradeFile
    	echo "getrgid,0" >> $vocareumGradeFile
    	echo "getfsuid,0" >> $vocareumGradeFile
    	echo "getfsgid,0" >> $vocareumGradeFile
    	echo "getstate,0" >> $vocareumGradeFile
		echo "getthread_count,0" >> $vocareumGradeFile
    	echo "getpriority,0" >> $vocareumGradeFile
    	echo "getniceness,0" >> $vocareumGradeFile
    	echo "getstime,0" >> $vocareumGradeFile
    	echo "getutime,0" >> $vocareumGradeFile
    	echo "getcstime,0" >> $vocareumGradeFile
    	echo "getcutime,0" >> $vocareumGradeFile
    	echo "getstartcode,0" >> $vocareumGradeFile
    	echo "getendcode,0" >> $vocareumGradeFile
    	echo "getesp,0" >> $vocareumGradeFile
    	echo "geteip,0" >> $vocareumGradeFile
    	echo "getfiles,0" >> $vocareumGradeFile
		echo "getvoluntary,0" >> $vocareumGradeFile
		echo "getnonvol,0" >> $vocareumGradeFile
    	echo "getcontext_switches,0" >> $vocareumGradeFile
    	echo "getallowed_cpus,0" >> $vocareumGradeFile
    	echo "getlast_cpu,0" >> $vocareumGradeFile
    	echo "getmemory_map,0" >> $vocareumGradeFile

		exit 1
	fi

	# First, move their old makefile so that it can be replaced
	# Their makefile could either be named makefile or Makefile
	# If they didn't inclue a makefile, then that's fine too.  
	# I'm going to overwrite it anyway
	MAKEFILE_NAME=""

	cat makefile &> /dev/null

	if [ "$?" == "0" ]; then
		MAKEFILE_NAME="makefile"
	fi

	cat Makefile &> /dev/null

	if [ "$?" == "0" ]; then
		MAKEFILE_NAME="Makefile"
	fi

	if [ "$MAKEFILE_NAME" != "" ]; then
		mv $MAKEFILE_NAME student_makefile
	fi

	# ---------------------------------------------------------------------------- #
	# C++ Testing Program                                                          # 
	# ---------------------------------------------------------------------------- #	

	echo "
	/* ------------------------------------------------------------------------- */
	/* Developer: Andrew Kirfman                                                 */
	/* Project: CSCE-313 Machine Problem 4                                       */
	/*                                                                           */
	/* File: ./instructor/mp4_test.sh                                            */
	/* ------------------------------------------------------------------------- */

	/* ------------------------------------------------------------------------- */
	/* Standard Library Includes                                                 */
	/* ------------------------------------------------------------------------- */

	#include<iostream>
	#include<vector>
	#include<fcntl.h>
	#include<sys/types.h>
	#include<sys/stat.h>
	#include<signal.h>

	// Attempt to include student's executable
	#include \"./proctest.h\"

	/* ------------------------------------------------------------------------- */
	/* User Defined Includes                                                     */
	/* ------------------------------------------------------------------------- */
	
	// Add include for proctest here!!!
	
	class MP4_Test
	{
	public:
    	/* Constructor */
    	MP4_Test();

    	/* Destructor */
    	~MP4_Test();

    	/* Data Getter Functions */
    	int getprocess_id();
    	int getsuccessful_tests();
    	int getfailed_tests();
    	int gettotal_tests();
	
	    /* Data Setter Functions */
	    void setprocess_id(int new_process_id);
	    void setsuccessful_tests(int new_successful_tests);
	    void setfailed_tests(int new_failed_tests);
	    void settotal_tests(int new_total_tests);

	    /* Proctest Testing Functions  */
	    /* Identifiers */
	    bool testpid();
	    bool testppid();
	    bool testeuid();
	    bool testegid();
	    bool testruid();
	    bool testrgid();
	    bool testfsuid();
    	bool testfsgid();

    	/* State */
    	bool teststate();
    
    	/* Thread Information */
    	bool testthread_count();

    	/* Priority */
    	bool testpriority();
    	bool testniceness();

    	/* Time Information */
    	bool teststime();
    	bool testutime();
    	bool testcstime();
    	bool testcutime();

    	/* Address Space */
    	bool teststartcode();
    	bool testendcode();
    	bool testesp();
    	bool testeip();

    	/* Resources */
    	bool testfiles();
    	bool testvoluntary_context_switches();
    	bool testnonvoluntary_context_switches();

    	/* Processors */
    	bool testlast_cpu();
    	bool testallowed_cpus();

    	/* Memory Map */
    	bool testmemory_map();

    	/* Primary Testing Function */
    	void testall();

	private:
    	/* Corect Process Data  */
    	int m_process_id;
    	char *m_read_buffer;
    	std::string m_num_fds;
    	std::vector<std::string> m_stat_array;
    	std::vector<std::vector<std::string> > m_status_array;
    	std::vector<std::string> m_mem_array;

    	std::string stringify_buffer(char *buffer);
    	std::vector<std::string> split_on_spaces(std::string input);
	
    	/* Student's Executable Data */
    	Proctest *student_executable;


    	/* Testing Data */
    	int m_successful_tests;
    	int m_failed_tests;
    	int m_total_tests;
	};


	/* Supporting Functions */
	std::string MP4_Test::stringify_buffer(char *buffer)
	{
	    std::string buffer_string = \"\";
	    int pos = 0;
	
	    while(true)
	    {
	        if(buffer[pos] == '\0')
	        {
	            break;
	        }
	        else
	        {
	            buffer_string += buffer[pos];
	            pos++;
	        }
	    }

	    return buffer_string;
	}

    std::vector<std::string> MP4_Test::split_on_spaces(std::string input)
    {
        std::vector<std::string> components;
        std::string token = \"\";
    
        for(unsigned short int i=0; i<input.length(); i++)
        {
            if(input[i] != ' ')
            {
                token += input[i];
            }
            else
            {
                components.push_back(token);
                token = \"\";
            }
        }
    
        return components;
    }
    
    
    /* Constructor */
    MP4_Test::MP4_Test()
    {
        m_successful_tests = 0;
        m_failed_tests = 0;
        m_total_tests = 26;
        
        pid_t process_id = fork();
        
        if(process_id == 0)
        {
            execlp(\"sleep\", \"sleep\", \"1000\", NULL);
        }
        else
        {
            // Parent doesn't do anything
        }
        
    
        /* Record process ID */
        m_process_id = process_id;
    
    
        /* Initialize Student Data */
        student_executable = new Proctest(m_process_id);
    
        /* Initialize the read buffer */
        const int BUFFER_SIZE = 16394;
        m_read_buffer = new char[BUFFER_SIZE];
    
        /* Read from the stat file */
        int stat_pipe[2];
        pipe(stat_pipe);
    
        pid_t stat_pid = fork();
    
        if(stat_pid == 0)
        {
            close(stat_pipe[0]);
            close(STDOUT_FILENO);
            dup(stat_pipe[1]);
    
            std::string fname = \"/proc/\" + std::to_string(m_process_id) + \"/stat\";
            execlp(\"cat\", \"cat\", fname.c_str(), NULL);
        }
        else
        {
            close(stat_pipe[1]);
            waitpid(stat_pid, NULL, 0);   
        }
    
        int read_size = read(stat_pipe[0], m_read_buffer, BUFFER_SIZE);
    
        if(read_size == -1)
        {
            std::cout << \"[ERROR]: Unable to read stat file.\" << std::endl;
            _exit(1);
        }
    
        std::string string_stat = stringify_buffer(m_read_buffer);
     
        /* Clean the read buffer for use again later */
        memset(m_read_buffer, '\0', BUFFER_SIZE);
    
        /* Assign result to stat_array member function */
        m_stat_array = split_on_spaces(string_stat);
    
        /* Read from the status file */
        std::string fname = \"/proc/\" + std::to_string(m_process_id) + \"/status\";
    
        int status_pipe[2];
        pipe(status_pipe);
    
        pid_t status_pid = fork();
    
        if(status_pid == 0)
        {
            close(status_pipe[0]);
            close(STDOUT_FILENO);
            dup(status_pipe[1]);
    
            execlp(\"cat\", \"cat\", fname.c_str(), NULL);
        }
        else
        {
            close(status_pipe[1]);
            waitpid(status_pid, NULL, 0);
        }
    
        read_size = read(status_pipe[0], m_read_buffer, BUFFER_SIZE);
        if(read_size == -1)
        {
            std::cout << \"[ERROR]: Unable to read status file.\" << std::endl;
            _exit(1);
        }
    
        std::string string_status = stringify_buffer(m_read_buffer);
    
        /* Clean Read Buffer */
        memset(m_read_buffer, '\0', BUFFER_SIZE);
    
        /* Turn status array into a vector<vector<string> > */
        std::vector<std::vector<std::string> > status_data;
        std::string token1 = \"\";
        std::string token2 = \"\";
        for(unsigned short int i=0; i<string_status.length(); i++)
        {
            if(string_status[i] == '\n')
            {
                status_data.push_back(std::vector<std::string>());
    
                /* Now separate the string on tabs */
                for(unsigned short int j=0; j<token1.length(); j++)
                {
                    if(token1[j] == '\t')
                    {
                        status_data[status_data.size() - 1].push_back(token2);
    
                        token2 = \"\";
                    }
                    else if(j == token1.length() - 1)
                    {
                        token2 += token1[j];
    
                        status_data[status_data.size() - 1].push_back(token2);
    
                        token2 = \"\";
                    }
                    else
                    {
                        token2 += token1[j];
                    }
                }
                token1 = \"\";
            }
            else
            {
                token1 += string_status[i];
            }
        }
    
        /* Assign the found data into the status array */
        m_status_array = status_data;
    
        /* Find the number of open file descriptors  */
        int fd_pipe_1[2];
        pipe(fd_pipe_1);
    
        pid_t fd_pid_1 = fork();
    
        if(fd_pid_1 == 0)
        {
            close(fd_pipe_1[0]);
            close(STDOUT_FILENO);
            dup(fd_pipe_1[1]);
            close(STDERR_FILENO);
            dup(fd_pipe_1[1]);
    
            std::string fd_file = \"/proc/\" + std::to_string(m_process_id) + \"/fd\";
            execlp(\"ls\", \"ls\", \"-la\", fd_file.c_str(), NULL);
        }
        else
        {
            close(fd_pipe_1[1]);
            waitpid(fd_pid_1, NULL, 0);
        }
    
        read(fd_pipe_1[0], m_read_buffer, BUFFER_SIZE);
        std::string fd_data = stringify_buffer(m_read_buffer);
    
        close(fd_pipe_1[0]);
    
        int failure_test = fd_data.find(\"Permission denied\");
        if(failure_test != -1)
        {
            m_num_fds = \"permission denied\";
        }
        else
        {
            int fd_pipe_2[2];
            pipe(fd_pipe_2);
    
            int fd_pipe_3[2];
            pipe(fd_pipe_3);
            
            write(fd_pipe_3[1], m_read_buffer, BUFFER_SIZE);
    
            pid_t fd_pid_2 = fork();
    
            if(fd_pid_2 == 0)
            {
                close(fd_pipe_2[0]);
                
                close(STDOUT_FILENO);
                dup(fd_pipe_2[1]);
    
                close(fd_pipe_3[1]);
    
                close(STDIN_FILENO);
                dup(fd_pipe_3[0]);
    
                execlp(\"wc\", \"wc\", \"-l\", NULL);
            }
            else
            {
                close(fd_pipe_2[1]);
                close(fd_pipe_3[1]);
                waitpid(fd_pid_2, NULL, 0);
            }
            read(fd_pipe_2[0], m_read_buffer, 16392);
            fd_data = stringify_buffer(m_read_buffer);
            
            close(fd_pipe_2[0]);
            
            m_num_fds = std::to_string(atoi(fd_data.c_str()) - 3);
        }
    
        /* Clean read buffer */
        memset(m_read_buffer, '\0', BUFFER_SIZE);
    
        /* Populate Memory Map */
        std::string mem_file = \"/proc/\" + std::to_string(process_id) + \"/smaps\";
        int mem_fd = open(mem_file.c_str(), O_RDONLY);
        std::string mem_data = \"\";
    
        while(read(mem_fd, m_read_buffer, BUFFER_SIZE) > 0)
        {
            mem_data += stringify_buffer(m_read_buffer);
            memset(m_read_buffer, '\0', BUFFER_SIZE);
        }
    
        if(mem_data == \"\")
        {
            m_mem_array.push_back(\"permission denied\");
        }
        else
        {
            std::vector<std::string> mem_lines;
            std::string mem_line = \"\";
            for(unsigned int i=0; i<mem_data.length(); i++)
            {
                if(mem_data[i] == '\n' || i == mem_data.length() - 1)
                {
                    mem_lines.push_back(mem_line);
    
                    mem_line = \"\";
                }
                else
                {
                    mem_line += mem_data[i];
                }
            }
    
            for(unsigned short int i=0; i<mem_lines.size(); i++)
            {
                if(!isupper(mem_lines[i][0]))
                {
                    m_mem_array.push_back(mem_lines[i]);
                }
            }
        }
    }
    
    MP4_Test::~MP4_Test()
    {
        /* Wipe out the process that was created */
        kill(m_process_id, SIGKILL);    
    
        /* Clear all other data */
        delete m_read_buffer;
        m_stat_array.clear();
        m_status_array.clear();
        m_mem_array.clear();
        delete student_executable;
    }
    
    
    /* Data Getter Functions */
    int MP4_Test::getprocess_id()
    {
        return m_process_id;
    }
    
    int MP4_Test::getsuccessful_tests()
    {
        return m_successful_tests;
    }
    
    int MP4_Test::getfailed_tests()
    {
        return m_failed_tests;
    }
    
    int MP4_Test::gettotal_tests()
    {
        return m_total_tests;
    }
    
    
    /* Data Setter Functions */
    void MP4_Test::setprocess_id(int new_process_id)
    {
        m_process_id = new_process_id;
    
        return;
    }
    
    void MP4_Test::setsuccessful_tests(int new_successful_tests)
    {
        m_successful_tests = new_successful_tests;
    
        return;
    }
    
    void MP4_Test::setfailed_tests(int new_failed_tests)
    {
        m_failed_tests = new_failed_tests;
    
        return;
    }
    
    void MP4_Test::settotal_tests(int new_total_tests)
    {
        m_total_tests = new_total_tests;
    
        return;
    }
    
    
    /* Proctest Testing Functions */
    /* Identifiers */
    bool MP4_Test::testpid()
    {
        std::string student_pid = student_executable->getpid();
        std::string test_pid = m_stat_array[0];
        
        return (student_pid == test_pid);
    }
    
    bool MP4_Test::testppid()
    {
        std::string student_ppid = student_executable->getppid();
        std::string test_ppid = m_stat_array[3];
    
        return (student_ppid == test_ppid);
    }
    
    bool MP4_Test::testeuid()
    {
        std::string student_euid = student_executable->geteuid();
        std::string test_euid = \"\";
        
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Uid:\");
            if(location != -1)
            {
                if(m_status_array[i][2] != \"\")
                {
                    test_euid = m_status_array[i][2];
                }
            }
        }
    
        return (student_euid == test_euid);
    }
    
    bool MP4_Test::testegid()
    {
        std::string student_egid = student_executable->getegid();
        std::string test_egid = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Gid:\");
            if(location != -1)
            {
                if(m_status_array[i][2] != \"\")
                {
                    test_egid = m_status_array[i][2];
                }
            }
        }
    
        return (student_egid == test_egid);
    }
    
    bool MP4_Test::testruid()
    {
        std::string student_ruid = student_executable->getruid();
        std::string test_ruid = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Uid:\");
            if(location != -1)
            {
                if(m_status_array[i][1] != \"\")
                {
                    test_ruid = m_status_array[i][1];
                }
            }
        }
    
    
        return (student_ruid == test_ruid);
    }
    
    bool MP4_Test::testrgid()
    {
        std::string student_rgid = student_executable->getrgid();
        std::string test_rgid = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Gid:\");
            if(location != -1)
            {
                if(m_status_array[i][1] != \"\")
                {
                    test_rgid = m_status_array[i][1];
                }
            }
        }
    
        return (student_rgid == test_rgid);
    }
    
    bool MP4_Test::testfsuid()
    {
        std::string student_fsuid = student_executable->getfsuid();
        std::string test_fsuid = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Uid:\");
            if(location != -1)
            {
                if(m_status_array[i][4] != \"\")
                {
                    test_fsuid = m_status_array[i][4];
                }
            }
        }
    
        return (student_fsuid == test_fsuid);
    }
    
    bool MP4_Test::testfsgid()
    {
        std::string student_fsgid = student_executable->getfsgid();
        std::string test_fsgid = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Gid:\");
            if(location != -1)
            {
                if(m_status_array[i][4] != \"\")
                {
                    test_fsgid = m_status_array[i][4];
                }
            }
        }
    
        return (student_fsgid == test_fsgid);
    }
    
    
    /* State */
    bool MP4_Test::teststate()
    {
        std::string student_state = student_executable->getstate();
        std::string test_state = m_stat_array[2];
    
        return (student_state == test_state);
    }
    
    
    /* Thread Information */
    bool MP4_Test::testthread_count()
    {
        std::string student_thread_count = student_executable->getthread_count();
        std::string test_thread_count = m_stat_array[19];
    
        return (student_thread_count == test_thread_count);
    }
    
    
    /* Priority */
    bool MP4_Test::testpriority()
    {
        std::string student_priority = student_executable->getpriority();
        std::string test_priority = m_stat_array[17];
    
        return (student_priority == test_priority);
    }
    
    bool MP4_Test::testniceness()
    {
        std::string student_niceness = student_executable->getniceness();
        std::string test_niceness = m_stat_array[18];
    
        return (student_niceness == test_niceness);
    }
    
    
    /* Time Information */
    bool MP4_Test::teststime()
    {
        std::string student_stime = student_executable->getstime();
        std::string test_stime = m_stat_array[14];
    
        return (student_stime == test_stime);
    }
    
    bool MP4_Test::testutime()
    {
        std::string student_utime = student_executable->getutime();
        std::string test_utime = m_stat_array[13];
    
        return (student_utime == test_utime); 
    }
    
    bool MP4_Test::testcstime()
    {
        std::string student_cstime = student_executable->getcstime();
        std::string test_cstime = m_stat_array[16];
    
        return (student_cstime == test_cstime);
    }
    
    bool MP4_Test::testcutime()
    {
        std::string student_cutime = student_executable->getcutime();
        std::string test_cutime = m_stat_array[15];
    
        return (student_cutime == test_cutime);
    }
    
    
    /* Address Space */
    bool MP4_Test::teststartcode()
    {
        std::string student_startcode = student_executable->getstartcode();
        std::string test_startcode = m_stat_array[25];
    
        return (student_startcode == test_startcode);
    }
    
    bool MP4_Test::testendcode()
    {
        std::string student_endcode = student_executable->getendcode();
        std::string test_endcode = m_stat_array[26];
    
        return (student_endcode == test_endcode);
    }
    
    bool MP4_Test::testesp()
    {
        std::string student_esp = student_executable->getesp();
        std::string test_esp = m_stat_array[28];
    
        return (student_esp == test_esp);
    }
    
    bool MP4_Test::testeip()
    {
        std::string student_eip = student_executable->geteip();
        std::string test_eip = m_stat_array[29];
    
        return (student_eip == test_eip);
    }
    
    
    /* Resources */
    bool MP4_Test::testfiles()
    {
        std::string student_files = student_executable->getfiles();
        std::string test_files = m_num_fds;
    
        return (student_files == test_files);
    }
    
    bool MP4_Test::testvoluntary_context_switches()
    {
        std::string student_voluntary_context_switches =
            student_executable->getnonvoluntary_context_switches();
        std::string test_voluntary_context_switches = \"\";
    
        int location1 = -1;
        int location2 = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location1 = m_status_array[i][0].find(\"voluntary\");
            location2 = m_status_array[i][0].find(\"nonvoluntary\");
            if(location1 != -1 && location2 == -1)
            {
                test_voluntary_context_switches = m_status_array[i][1];
            }
        }
    
        return (student_voluntary_context_switches == test_voluntary_context_switches);
    }
    
    bool MP4_Test::testnonvoluntary_context_switches()
    {
        std::string student_nonvoluntary_context_switches = 
            student_executable->getnonvoluntary_context_switches();
        std::string test_nonvoluntary_context_switches = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"nonvoluntary\");
            if(location != -1)
            {
                test_nonvoluntary_context_switches = m_status_array[i][1];
            }
        }
    
        return (student_nonvoluntary_context_switches == test_nonvoluntary_context_switches);
    }
    
    
    
    /* Processors */
    bool MP4_Test::testlast_cpu()
    {
        std::string student_last_cpu = student_executable->getlast_cpu();
        std::string test_last_cpu = m_stat_array[38];
    
        return (student_last_cpu == test_last_cpu);
    }
    
    bool MP4_Test::testallowed_cpus()
    {
        std::string student_allowed_cpus = student_executable->getallowed_cpus();
        std::string test_allowed_cpus = \"\";
    
        int location = -1;
        for(unsigned short int i=0; i<m_status_array.size(); i++)
        {
            location = m_status_array[i][0].find(\"Cpus_allowed_list\");
            if(location != -1)
            {
                test_allowed_cpus = m_status_array[i][1];   
            }
        }
    
        return (student_allowed_cpus == test_allowed_cpus);
    }
    
    
    /* Memory Map */
    bool MP4_Test::testmemory_map()
    {
        std::vector<std::string> student_memory_map = student_executable->getmemory_map();
        std::vector<std::string> test_memory_map = m_mem_array;
    
        bool is_equal = false;
        for(unsigned int i=0; i<test_memory_map.size(); i++)
        {
            for(unsigned int j=0; j<student_memory_map.size(); j++)
            {
                if(test_memory_map[i] == student_memory_map[j])
                {
                    is_equal = true;
                }
            }
    
            if(is_equal == false)
            {
                return false;
            }
        }
    
        return true;
    }

    /* Primary Testing Function */
    void MP4_Test::testall()
    {
        /* getpid */
        if(testpid())
        {
			std::cout << \"getpid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getpid Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getppid */
        if(testppid())
        {
			std::cout << \"getppid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
            std::cout << \"getppid Failed\" << std::endl;
            m_failed_tests += 1;
        }
        
        /* geteuid */
        if(testeuid())
        {
			std::cout << \"geteuid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"geteuid Failed\" << std::endl;
        	m_failed_tests += 1;
        }
    
        /* getegid */
        if(testegid())
        {
			std::cout << \"getegid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getegid Failed\" << std::endl;
        	m_failed_tests += 1;
        }
    
        /* getruid */
        if(testruid())
        {
			std::cout << \"getruid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getruid Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getrgid */
        if(testrgid())
        {
			std::cout << \"getrgid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getrgid Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getfsuid */
        if(testfsuid())
        {
            std::cout << \"getfsuid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getfsuid Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getfsgid */
        if(testfsgid())
        {
			std::cout << \"getfsgid Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getfsgid Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getstate */
        if(teststate())
        {
			std::cout << \"getstate Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getstate Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getthread_count */
        if(testthread_count())
        {
			std::cout << \"getthread_count Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getthread_count Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getpriority */
        if(testpriority())
        {
			std::cout << \"getpriority Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getpriority Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getniceness */
        if(testniceness())
        {
			std::cout << \"getniceness Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getniceness Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getstime */
        if(teststime())
        {
			std::cout << \"getstime Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getstime Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getutime */
        if(testutime())
        {
			std::cout << \"getutime Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getutime Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getcstime */
        if(testcstime())
        {
			std::cout << \"getcstime Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getcstime Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getcutime */
        if(testcutime())
        {
            std::cout << \"getcutime Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getcutime Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getstartcode */
        if(teststartcode())
        {
			std::cout << \"getstartcode Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getstartcode Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getendcode */
        if(testendcode())
        {
			std::cout << \"getendcode Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getendcode Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getesp */
        if(testesp())
        {
			std::cout << \"getesp Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getesp Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* geteip */
        if(testeip())
        {
			std::cout << \"geteip Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"geteip Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getfiles */
        if(testfiles())
        {
			std::cout << \"getfiles Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getfiles Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getvoluntary_context_switches */
        if(testvoluntary_context_switches())
        {
			std::cout << \"getvoluntary Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getvoluntary Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getnonvoluntary_context_switches */
        if(testnonvoluntary_context_switches())
        {
			std::cout << \"getnonvoluntary Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getnonvoluntary Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getlast_cpu */
        if(testlast_cpu())
        {
            std::cout << \"getlast_cpu Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
            std::cout << \"getlast_cpu Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getallowed_cpus */
        if(testallowed_cpus())
        {
            std::cout << \"getallowed_cpus Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getallowed_cpus Failed\" << std::endl;
            m_failed_tests += 1;
        }
    
        /* getmemory_map */
        if(testmemory_map())
        {
			std::cout << \"getmemory_map Succeeded\" << std::endl;
            m_successful_tests += 1;
        }
        else
        {
			std::cout << \"getmemory_map Failed\" << std::endl;
            m_failed_tests += 1;
        }
    }
                                      
                                      
    int main()
    {
    	MP4_Test *test_unit = new MP4_Test;
        
        test_unit->testall();                           
                    
        delete test_unit;
                                      
        return 0;
    }" > main.cpp
                                      
	echo "
# --------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                   #
# Project: CSCE-313 Episode #4                                                #
#                                                                             #
# File: ./Makefile                                                            #
# --------------------------------------------------------------------------- #
    
    
CC=g++
STD=-std=c++11
CFLAGS=-O3 -Wall -g
    
all: main.o proctest.o
	\$(CC) \$(STD) \$(CFLAGS) -o proctest main.o proctest.o 
                                      
main.o: main.cpp
	\$(CC) \$(STD) \$(CFLAGS) -c main.cpp
                                      
proctest.o: proctest.cpp proctest.h
	\$(CC) \$(STD) \$(CFLAGS) -c proctest.cpp proctest.h
    
clean:
	rm -rf proctest *.o *.gch
" > Makefile
                                      
    make
                                      
    if [ "$?" != "0" ]; then
		echo "Problem Compiling Test File"
    fi                                  
             
	# ---------------------------------------------------------------------------- #
	# Run Proctest Program                                                         # 
	# ---------------------------------------------------------------------------- #                                    
           
	protest_output="$(./proctest)"
    
	# ---------------------------------------------------------------------------- #
	# getpid                                                                       # 
	# ---------------------------------------------------------------------------- #        
            
    IS_GETPID="$(echo $proctest_output | grep 'getpid Succeeded')"
    
    if [ "$IS_GETPID" != "" ]; then
    	echo "getpid        ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getpid,5" >> $vocareumGradeFile
	else
		echo "getpid        ...   Failed [0/5]" >> $vocareumReportFile
		echo "getpid,0" >> $vocareumGradeFile    
	fi
                              
	# ---------------------------------------------------------------------------- #
	# getppid                                                                      # 
	# ---------------------------------------------------------------------------- #                                      
	
	IS_GETPPID="$(echo $proctest_output | grep 'getppid Succeeded')"
	
	if [ "$IS_GETPPID" != "" ]; then
    	echo "getppid       ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getppid,5" >> $vocareumGradeFile	
	else
		echo "getppid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getppid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# geteuid                                                                      # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETEUID="$(echo $proctest_output | grep 'geteuid Succeeded')"
	
	if [ "$IS_GETEUID" != "" ]; then
    	echo "geteuid       ...   Passed [5/5]" >> $vocareumReportFile
   		echo "geteuid,5" >> $vocareumGradeFile
	else
		echo "geteuid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "geteuid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getegid                                                                      # 
	# ---------------------------------------------------------------------------- # 
	
	IS_GETEGID="$(echo $proctest_output | grep 'getegid Succeeded')"
	
	if [ "$IS_GETEGID" != "" ]; then
    	echo "getegid       ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getegid,5" >> $vocareumGradeFile	
	else
		echo "getegid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getegid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getruid                                                                      # 
	# ---------------------------------------------------------------------------- # 
	
	IS_GETRUID="$(echo $proctest_output | grep 'getruid Succeeded')"
	
	if [ "$IS_GETRUID" != "" ]; then
    	echo "getruid       ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getruid,5" >> $vocareumGradeFile
	else
		echo "getruid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getruid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getrgid                                                                      # 
	# ---------------------------------------------------------------------------- # 
	
	IS_GETRGID="$(echo $proctest_output | grep 'getrgid Succeeded')"
	
	if [ "$IS_GETRGID" != "" ]; then
		echo "getrgid       ...   Passed [5/5]" >> $vocareumReportFile
		echo "getrgid,5" >> $vocareumGradeFile
	else
		echo "getrgid       ...   Failed [0/5]" >> $vocareumReportFile
		echo "getrgid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getfsuid                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETFSUID="$(echo $proctest_output | grep 'getfsuid Succeeded')"
	
	if [ "$IS_GETFSUID" != "" ]; then
    	echo "getfsuid      ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getfsuid,5" >> $vocareumGradeFile
	else
		echo "getfsuid      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getfsuid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getfsgid                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETFSGID="$(echo $proctest_output | grep 'getfsgid Succeeded')"
	
	if [ "$IS_GETFSGID" != "" ]; then
    	echo "getfsgid      ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getfsgid,5" >> $vocareumGradeFile
	else
		echo "getfsgid      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getfsgid,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getstate                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETSTATE="$(echo $proctest_output | grep 'getstate Succeeded')"
	
	if [ "$IS_GETSTATE" != "" ]; then
    	echo "getstate      ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getstate,5" >> $vocareumGradeFile
	else
		echo "getstate      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getstate,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getthread_count                                                              # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETTHREAD_COUNT="$(echo $proctest_output | grep 'getthread_count Succeeded')"
	
	if [ "$IS_GETTHREAD_COUNT" != "" ]; then
    	echo "getthread     ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getthread_count,5" >> $vocareumGradeFile	
	else
		echo "getthread     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getthread_count,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getpriority                                                                  # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETPRIORITY="$(echo $proctest_output | grep 'getpriority Succeeded')"
	
	if [ "$IS_GETPRIORITY" != "" ]; then
    	echo "getpriority   ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getpriority,5" >> $vocareumGradeFile
	else
		echo "getpriority   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getpriority,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getniceness                                                                  # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETNICENESS="$(echo $proctest_output | grep 'getniceness Succeeded')"
	
	if [ "$IS_GETNICENESS" != "" ]; then
    	echo "getniceness   ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getniceness,5" >> $vocareumGradeFile
	else
		echo "getniceness   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getniceness,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getstime                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETSTIME="$(echo $proctest_output | grep 'getstime Succeeded')"
	
	if [ "$IS_GETSTIME" != "" ]; then
    	echo "getstime      ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getstime,5" >> $vocareumGradeFile
	else
		echo "getstime      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getstime,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getutime                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETUTIME="$(echo $proctest_output | grep 'getutime Succeeded')"
	
	if [ "$IS_GETUTIME" != "" ]; then
    	echo "getutime      ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getutime,5" >> $vocareumGradeFile
	else
		echo "getutime      ...   Failed [0/5]" >> $vocareumReportFile
		echo "getutime,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getcstime                                                                    # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETCSTIME="$(echo $proctest_output | grep 'getcstime Succeeded')"
	
	if [ "$IS_GETCSTIME" != "" ]; then
    	echo "getcstime     ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getcstime,5" >> $vocareumGradeFile
	else
		echo "getcstime     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getcstime,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getcutime                                                                    # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETCUTIME="$(echo $proctest_output | grep 'getcutime Succeeded')"
	
	if [ "$IS_GETCUTIME" != "" ]; then
    	echo "getcutime     ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getcutime,5" >> $vocareumGradeFile
	else
		echo "getcutime     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getcutime,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getstartcode                                                                 # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETSTARTCODE="$(echo $proctest_output | grep 'getstartcode Succeeded')"
	
	if [ "$IS_GETSTARTCODE" != "" ]; then
    	echo "getstartcode  ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getstartcode,5" >> $vocareumGradeFile
	else
		echo "getstartcode  ...   Failed [0/5]" >> $vocareumReportFile
		echo "getetartcode,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getendcode                                                                   # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETENDCODE="$(echo $proctest_output | grep 'getendcode Succeeded')"
	
	if [ "$IS_GETENDCODE" != "" ]; then
    	echo "getendcode    ...   Passed [5/5]" >> $vocareumReportFile
   		echo "geteuid,5" >> $vocareumGradeFile
	else
		echo "getendcode    ...   Failed [0/5]" >> $vocareumReportFile
		echo "getendcode,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getesp                                                                       # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETESP="$(echo $proctest_output | grep 'getesp Succeeded')"
	
	if [ "$IS_GETESP" != "" ]; then
    	echo "getesp        ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getesp,5" >> $vocareumGradeFile
	else
		echo "getesp        ...   Failed [0/5]" >> $vocareumReportFile
		echo "getesp,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# geteip                                                                       # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETEIP="$(echo $proctest_output | grep 'geteip Succeeded')"
	
	if [ "$IS_GETEIP" != "" ]; then
    	echo "geteip        ...   Passed [5/5]" >> $vocareumReportFile
   		echo "geteip,5" >> $vocareumGradeFile
	else
		echo "geteip        ...   Failed [0/5]" >> $vocareumReportFile
		echo "geteip,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getfiles                                                                     # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETFILES="$(echo $proctest_output | grep 'getfiles Succeeded')"
	
	if [ "$IS_GETFILES" != "" ]; then
    	echo "getfiles      ...   Passed [20/20]" >> $vocareumReportFile
   		echo "getfiles,20" >> $vocareumGradeFile
	else
		echo "getfiles      ...   Failed [0/20]" >> $vocareumReportFile
		echo "getfiles,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getvoluntary_context_switches                                                # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETVOLUNTARY="$(echo $proctest_output | grep 'getvoluntary Succeeded')"
	
	if [ "$IS_GETVOLUNTARY" != "" ]; then
    	echo "getvoluntary  ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getvoluntary,5" >> $vocareumGradeFile
	else
		echo "getvoluntary  ...   Failed [0/5]" >> $vocareumReportFile
		echo "getvoluntary,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getnonvoluntary_context_switches                                             # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETNONVOL="$(echo $proctest_output | grep 'getnonvol Succeeded')"
	
	if [ "$IS_GETNONVOL" != "" ]; then
    	echo "getnonvol     ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getnonvol,5" >> $vocareumGradeFile
	else
		echo "getnonvol     ...   Failed [0/5]" >> $vocareumReportFile
		echo "getnonvol,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getlast_cpu                                                                  # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETLAST_CPU="$(echo $proctest_output | grep 'getlast_cpu Succeeded')"
	
	if [ "$IS_GETLAST_CPU" != "" ]; then
    	echo "getlast_cpu   ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getlast_cpu,5" >> $vocareumGradeFile
	else
		echo "getlast_cpu   ...   Failed [0/5]" >> $vocareumReportFile
		echo "getlast_cpu,0" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getallowed_cpus                                                              # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETALLOWED_CPUS="$(echo $proctest_output | grep 'getallowed_cpus')"
	
	if [ "$IS_GETALLOWED_CPUS" != "" ]; then
    	echo "getallowed_cp ...   Passed [5/5]" >> $vocareumReportFile
   		echo "getallowed_cpus,5" >> $vocareumGradeFile
	else
		echo "getallowed_cp ...   Failed [0/5]" >> $vocareumReportFile
		echo "getallowed_cpus,5" >> $vocareumGradeFile
	fi
	
	# ---------------------------------------------------------------------------- #
	# getmemory_map                                                                # 
	# ---------------------------------------------------------------------------- # 	
	
	IS_GETMEMORY_MAP="$(echo $proctest_output | grep 'getmemory_map')"
	
	if [ "$IS_GETMEMORY_MAP" != "" ]; then
    	echo "getmemory_map ...   Passed [20/20]" >> $vocareumReportFile
   		echo "getmemory_map,20" >> $vocareumGradeFile
	else
		echo "getmemory_map ...   Failed [0/20]" >> $vocareumReportFile
		echo "getmemory_map,0" >> $vocareumGradeFile
	fi
	
	if [ "$MAKEFILE_NAME" != "" ]; then
		mv student_makefile Makefile
	fi

	exit 0
fi
