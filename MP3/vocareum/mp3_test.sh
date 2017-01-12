# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                    #
# Project: CSCE-313 Machine Problem #3                                         #
#                                                                              #
# File: ./Grading                                                              #
# ---------------------------------------------------------------------------- #

echo 'VOC_NO_REPORT_OUTPUT'    >> $vocareumReportFile 

# Check to ensure the existance of all of the files that the student should 
# have turned in

part_1=false
part_2=false
part_3=false
part_4=false
part_5=false
part_6=false
part_7=false


# redirect stderr
cat mp3_part1.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_1=true"
fi

cat mp3_part2.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_2=true"
fi

cat mp3_part3.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_3=true"
fi

cat mp3_part4.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_4=true"
fi

cat mp3_part5.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_5=true"
fi

cat mp3_part6.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_6=true"
fi

cat mp3_part7.cpp > /dev/null

if [ "$?" == "0" ]; then
	let "part_7=true"
fi

# Get rid of the student's makefile and replace it with a correct one
rm makefile Makefile &> /dev/null

echo \
'# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                    #
# Project: CSCE-313 Machine Problem #3                                         #
#                                                                              #
# File: ./Makefile                                                             #
# ---------------------------------------------------------------------------- #

CC=g++
STD=-std=c++11
CFLAGS=-Ofast
T1=mp3_part1.cpp
T2=mp3_part2.cpp
T3=mp3_part3.cpp
T4=mp3_part4.cpp
T5=mp3_part5.cpp
T6=mp3_part6.cpp
T7=mp3_part7.cpp


all: $(T1) $(T2) $(T3) $(T4) $(T5) $(T6) $(T7)
	make part1
	make part2
	make part3
	make part4
	make part5
	make part6
	make part7


part1: $(T1)
	$(CC) $(STD) $(CFLAGS) -o part1 mp3_part1.cpp


part2: $(T2)
	$(CC) $(STD) $(CFLAGS) -o part2 mp3_part2.cpp


part3: $(T3)
	$(CC) $(STD) $(CFLAGS) -o part3 mp3_part3.cpp


part4: $(T4)
	$(CC) $(STD) $(CFLAGS) -o part4 mp3_part4.cpp


part5: $(T5) 
	$(CC) $(STD) $(CFLAGS) -o part5 mp3_part5.cpp


part6: $(T6)
	$(CC) $(STD) $(CFLAGS) -o part6 mp3_part6.cpp


part7: $(T7)
	$(CC) $(STD) $(CFLAGS) -o part7 mp3_part7.cpp


clean:
	rm -rf *.o *.gch part1 part2 part3 part4 part5\
		part6 part7
' > Makefile


# Point allocations for each test case
part1_points=5
part2_points=5
part3_points=5
part4_points=5
part5_points=15
part6_points=25
part7_points=40

# Test each program individually and adjust points accordingly
if [ "$part_1" == "0" ]; then
	make part1 &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part1)
		expected_output=$(ls -la)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$test_output" != "$expected_output" ]; then
			let "part1_points=0"
		fi

		# Make sure that the student is using execl properly
		PART1_REGEX="execl\(.*,.*,.*,.*\)"
		TEST=$(cat mp3_part1.cpp | grep -E "$PART1_REGEX" | grep -vE "//")

		if [ "$TEST" == "" ]; then
			echo "mp3_part1.cpp   ...  execl not used correctly" >> $vocareumReportFile
			let "part1_points=0"
		fi

		# Make sure the students aren't using the system function
		SYSTEM=$(cat mp3_part1.cpp | grep -E "system\(.*\)" | grep -vE "//")

		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part1.cpp   ...   System function not allowed" >> $vocaremReportFile
			let "part1_points=0"
		fi
	else
		echo "mp3_part1.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part1_points=0"
	fi
else
	let "part1_points=0"
fi


if [ "$part_2" == "0" ]; then
	make part2 &> /dev/null
	
	if [ "$?" == "0" ]; then
		test_output=$(./part2)
		expected_output=$(ls -la)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$test_output" != "$expected_output" ]; then
			let "part2_points=0"
		fi

		# Make sure that the student is using execlp correctly
		PART2_REGEX="execlp\(.*,.*,.*,.*\)"
		TEST=$(cat mp3_part2.cpp | grep -E "$PART2_REGEX" | grep -vE "//")		

		if [ "$TEST" == "" ]; then
			echo "mp3_part2.cpp   ...   execlp not used correctly" >> $vocareumReportFile
			let "part2_points=0"
		fi

		# Make sure the students aren't using the system() function
		SYSTEM=$(cat mp3_part2.cpp | grep -E "system\(.*\)" | grep -vE "//")

		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part2.cpp   ...   System function not allowed" >> $vocareumReportFunction
			let "part2_points=0"
		fi
	else
		echo "mp4_part2.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part2_points=0"
	fi
else
	let "part2_points=0"
fi


if [ "$part_3" == "0" ]; then
	make part3 &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part3)
		expected_output=$(ls -la)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		
		if [ "$test_output" != "$expected_output" ]; then
			let "part3_points=0"
		fi

		# Make sure that the student is using execv correctly
		PART3_REGEX="execv\(.*,.*\)"
		TEST=$(cat mp3_part3.cpp | grep -E $PART3_REGEX | grep -vE "//")

		if [ "$TEST" == "" ]; then
			echo "mp3_part3.cpp   ...   Execv not used correctly" >> $vocareumReportFile
			let "part3_points=0"
		fi

		# Make sure the students aren't using the system() function
		SYSTEM=$(cat mp3_part3.cpp | grep -E "system\(.*\)" | grep -vE "//")
		
		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part3.cpp   ...   System function not allowed" >> $vocareumReportFile
			let "part3_points=0"
		fi
		
	else
		echo "mp3_part3.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part3_points=0"
	fi
else
	let "part3_points=0"
fi


if [ "$part_4" == "0" ]; then
	make part4 &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part4)
		expected_output=$(ls -la)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$test_output" != "$expected_output" ]; then
			let "part4_points=0"
		fi

		# Make sure that the student is using execvp correctly
		PART4_REGEX="execvp\(.*,.*\)"
		TEST=$(cat mp3_part4.cpp | grep -E "$PART4_REGEX" | grep -vE "//")

		if [ "$TEST" == "" ]; then
			echo "mp3_part4.cpp   ...   Execvp not used correctly" >> $vocareumReportFile
			let "part4_points=0"
		fi

		# Make sure the students aren't using the system() function
		SYSTEM=$(cat mp3_part4.cpp | grep -E "system\(.*\)" | grep -vE "//")
		
		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part4.cpp   ...   System function not allowed" >> $vocareumReportFile
			let "part4_points=0"
		fi
	else
		echo "mp3_part4.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part4_points=0"
	fi
else
	let "part4_points=0"
fi


if [ "$part_5" == "0" ]; then
	make part5 &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part5)		
		expected_output="HelloWorld!"

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		
		# Also try and get rid of newlines just to be sure
		test_output=$(echo "$test_output" | sed ':a;N;$!ba;s/\n//g')

		if [ "$test_output" != "$expected_output" ]; then
			let "part5_points=0"
		fi

		# Make sure that the students used fork and wait properly
		TEST_1=$(cat mp3_part5.cpp | grep -E "fork()" | grep -vE "//")
		TEST_2=$(cat mp3_part5.cpp | grep -E "(wait|waitpid)\(.*\)" | grep -vE "//")

		if [ "$TEST_1" == "" ]; then
			echo "mp3_part5.cpp   ...   Fork not used correctly" >> $vocareumReportFile
			let "part5_points=0"
		fi
		
		if [ "$TEST_2" == "" ]; then
			echo "mp3_part5.cpp   ...   Wait/Waitpid not used correctly" >> $vocareumReportFile 
			let "part5_points=0"
		fi

		# Make sure the students aren't using the system() function
		SYSTEM=$(cat mp3_part5.cpp | grep -E "system\(.*\)" | grep -vE "//")

		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part5.cpp   ...   System function not allowed" >> $vocareumReportFile
			let "part5_points=0"
		fi
	else
		echo "mp3_part5.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part5_points=0"
	fi
else
	let "part5_points=0"
fi


if [ "$part_6" == "0" ]; then
	make part6 &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part6)
		expected_output=$(ls -la)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo "$test_output" | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo "$expected_output" | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$expected_output" != "$test_output" ]; then
			let "part6_points=0"
		fi

		# Make sure that the student used each required function properly
		
		# ---------------------------------------- #
		# fork()                                   #
		# ---------------------------------------- #
		TEST1=$(cat mp3_part6.cpp | grep -E "fork()" | grep -vE "//")

		if [ "$TEST1" == "" ]; then
			echo "mp3_part6.cpp   ...   Fork not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		COMMENT1=$(cat mp3_part6.cpp | grep -E '(/\*([^*]|(\*+[^*/]))*\*+/)|(//.*)')
		COMMENT1=$(echo "$COMMENT1" | grep -E "fork()" | grep -vE "//")

		if [ "$COMMENT1" == "$TEST1" ]; then
			echo "mp3_part6.cpp   ...   For not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		# ---------------------------------------- #
		# exec()                                   #
		# ---------------------------------------- #
		PART6_1_REGEX="exec(l|lp|le|v|vp|vpe)"
		TEST2=$(cat mp3_part6.cpp | grep -E "$PART6_1_REGEX" | grep -vE "//")

		# First, figure out which exec function the student used
		WHICH_EXEC_REGEX="(execl|execlp|execv|execvp|execle|execvpe)"
		EXEC_TYPE=$(cat mp3_part6.cpp | grep -E "$WHICH_EXEC_REGEX" | grep -vE "//")
		EXEC_TYPE=$(echo "$EXEC_TYPE" | sed "s/.*\(exec[lv]\{1\}[pe]\{0,1\}[e]\?\).*/\1/g" | tr --delete " \t\n")

		if [ "$EXEC_TYPE" == "" ]; then
			echo "mp3_part6.cpp   ...   Exec not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		COMMENT2=$(cat mp3_part6.cpp | grep -E '(/\*([^*]|(\*+[^*/]))*\*+/)|(//.*)')
		COMMENT2=$(echo "$COMMENT2" | grep -E "$WHICH_EXEC_REGEX" | grep -vE "//")
		COMMENT2=$(echo "$COMMENT2" | sed "s/.*\(exec[lv]\{1\}[pe]\{0,1\}[e]\?\).*/\1/g" | tr --delete " \t\n")

		if [ "$EXEC_TYPE" == "$COMMENT2" ]; then
			echo "mp3_part6.cpp   ...   Exec not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		# ---------------------------------------- #
		# read()                                   #
		# ---------------------------------------- #
		TEST3=$(cat mp3_part6.cpp | grep -E "read\(.*\)" | grep -vE "//")
		if [ "$TEST3" == "" ]; then
			echo "mp3_part6.cpp   ...   Read not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		COMMENT3=$(cat mp3_part6.cpp | grep -E '(/\*([^*]|(\*+[^*/]))*\*+/)|(//.*)')
		COMMENT3=$(echo "$COMMENT3" | grep -E "read\(.*\)" | grep -vE "//")

		if [ "$COMMENT3" == "$TEST3" ]; then
			echo "mp3_part6.cpp   ...   Read not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		# ---------------------------------------- #
		# write()                                  #
		# ---------------------------------------- #
		TEST4=$(cat mp3_part6.cpp | grep -E "write\(.*\)" | grep -vE "//")
		if [ "$TEST4" == "" ]; then
			echo "mp3_part6.cpp   ...   Write not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		COMMENT4=$(cat mp3_part6.cpp | grep -E '(/\*([^*]|(\*+[^*/]))*\*+/)|(//.*)')
		COMMENT4=$(echo "$COMMENT4" | grep -E "write\(.*\)" | grep -vE "//")

		if [ "$COMMENT4" == "$TEST4" ]; then
			echo "mp3_part6.cpp   ...   Write not used correctly" >> $vocareumReportFile
			let "part6_points=0"
		fi

		# Make sure that they didn't use the system() function
		SYSTEM=$(cat mp3_part6.cpp | grep -E "system\(.*\)" | grep -vE "//")
		
		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part6.cpp   ...   System function not allowed" >> $vocareumReportFile
			let "part6_points=0"
		fi
	else
		echo "mp3_part6.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part6_points=0"
	fi
else
	let "part6_points=0"
fi


if [ "$part_7" == "0" ]; then
	make part7 &> /dev/null	

	if [ "$?" == "0" ]; then
		test_output=$(./part7)
		expected_output=$(ls -la | tr [a-zA-Z0-9] a)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo "$test_output" | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo "$expected_output" | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$test_output" != "$expected_output" ]; then
			let "part7_points=0"
		fi

		# The students have quite a bit of freedo in writing this part of the assignment.  
		# As such, there's no pattern which I can test their assignment against.  There are
		# Functions that they have to use (specifically, fork and exec), but that's it
		# This script tests those two functions and makes sure that they aren't using 
		# system(), but does nothing else otherwise.  That should be enough to sort out
		# any potential cheaters.  

		# exec()
		TEST1=$(cat mp3_part7.cpp | grep -E "exec" | grep -vE "//")
		if [ "$TEST1" == "" ]; then
			echo "mp3_part7.cpp   ...   Exec not used correctly" >> $vocareumReportFile
			let "part7_points=0"
		fi

		# fork()
		TEST2=$(cat mp3_part7.cpp | grep -E "fork()" | grep -vE "//")
		if [ "$TEST2" == "" ]; then
			echo "mp3_part7.cpp   ...   Fork not used correctly" >> $vocareumReportFile
			let "part7_points=0"
		fi

		# Make sure the students aren't using the system() function
		SYSTEM=$(cat mp3_part7.cpp | grep -E "system\(.*\)" | grep -E "//")

		if [ "$SYSTEM" != "" ]; then
			echo "mp3_part7.cpp   ...   System function not allowed" >> $vocareumReportFile
			let "part7_points=0"
		fi
	else
		echo "mp3_part7.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part7_points=0"
	fi
else
	let "part7_points=0"
fi

# Clean up the project directory at the end
make clean &> /dev/null


# Final Point Recording

# PART_1
if [ "$part1_points" == "5" ]; then
	echo "mp3_part1.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part1,5" >> $vocareumGradeFile
else
	echo "mp3_part1.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part1,0" >> $vocareumGradeFile
fi


# PART_2
if [ "$part2_points" == "5" ]; then
	echo "mp3_part2.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part2,5" >> $vocareumGradeFile
else
	echo "mp3_part2.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part2,0" >> $vocareumGradeFile
fi


# PART_3
if [ "$part3_points" == "5" ]; then
	echo "mp3_part3.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part3,5" >> $vocareumGradeFile
else
	echo "mp3_part3.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part3,0" >> $vocareumGradeFile
fi


# PART_4
if [ "$part4_points" == "5" ]; then
	echo "mp3_part4.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part4,5" >> $vocareumGradeFile
else
	echo "mp3_part4.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part4,0" >> $vocareumGradeFile
fi


# PART_5
if [ "$part5_points" == "15" ]; then
	echo "mp3_part5.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part5,15" >> $vocareumGradeFile
else
	echo "mp3_part5.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part5,0" >> $vocareumGradeFile
fi


# PART_6
if [ "$part6_points" == "25" ]; then
	echo "mp3_part6.cpp   ...   Passed [25/25]" >> $vocareumReportFile
	echo "part6,25" >> $vocareumGradeFile
else
	echo "mp3_part6.cpp   ...   Failed [0/25]" >> $vocareumReportFile
	echo "part6,0" >> $vocareumGradeFile
fi


# PART_7
if [ "$part7_points" == "40" ]; then
	echo "mp3_part7.cpp   ...   Passed [40/40]" >> $vocareumReportFile
	echo "part7,40" >> $vocareumGradeFile
else
	echo "mp3_part7.cpp   ...   Failed [0/40]" >> $vocareumReportFile
	echo "part7,0" >> $vocareumGradeFile
fi

# Output the report file at the very end
echo "Full Grading Report: "
cat $vocareumReportFile 2> /dev/null | grep -v "VOC"

exit 0
