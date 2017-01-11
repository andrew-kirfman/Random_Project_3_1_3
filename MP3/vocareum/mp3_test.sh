# ---------------------------------------------------------------------------- #
# Developer: Andrew Kirfman                                                    #
# Project: CSCE-313 Machine Problem #3                                         #
#                                                                              #
# File: ./Grading                                                              #
# ---------------------------------------------------------------------------- #

# Check to ensure the existance of all of the files that the student should 
# have turned in

part1=false
part2=false
part3=false
part4=false
part5=false
part6=false
part7=false


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

# If the makefile isn't there, then just replace it.  
# Also, check to see if the student tampered with the built in Makefile
# If the did, replace it as well.  If the Makefile doesn't fit the
# expected pattern, it will definitely fail.  
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
' > makefile_test

diff makefile_test Makefile

if [ "$?" != "0" ]; then
	echo "Makefile Replaced!"
	mv makefile_test Makefile
fi

rm makefile_test


# Point allocations for each test case
part1_points=5
part2_points=5
part3_points=5
part4_points=5
part5_points=15
part6_points=25
part7_points=40


# Test each program individually and adjust points accordingly
if [ "$part_1" == "true" ]; then
	make part1  # &> /dev/null

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
		PART1_REGEX="execl\(.*,.*,.*,[ ]{0,1}[nN][uU][lL][lL].*\)"
		TEST=$(cat mp3_part1.cpp | grep -E "$PART1_REGEX" | grep -vE "//")

		if [ "$TEST" == "" ]; then
			echo "mp3_part1.cpp   ...  execl not used correctly" >> $vocareumReportFile
			let "part1_points=0"
		fi
	else
		echo "mp3_part1.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part1_points=0"
	fi
fi

if [ "$part_2" == "true" ]; then
	make part2 # &> /dev/null
	
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
		PART2_REGEX="execlp\(.*,.*,.*,[ ]{0,1}[nN][uU][lL][lL].*\)"
		TEST=$(cat mp3_part1.cpp | grep -E "$PART2_REGEX" | grep -vE "//")		

		if [ "$TEST" == "" ]; then
			echo "mp3_part2.cpp   ...  execlp not used correctly" >> $vocareumReportFile
			let "part2_points=0"
		fi
	else
		echo "mp4_part2.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part2_points=0"
	fi
fi

if [ "$part_3" == "true" ]; then
	make part3 # &> /dev/null

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
	else
		echo "mp3_part3.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part3_points=0"
	fi


fi

if [ "$part_4" == "true" ]; then
	make part4 # &> /dev/null

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
	else
		echo "mp3_part4.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part4_points=0"
	fi
fi

if [ "$part_5" == "true" ]; then
	make part5 # &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part5)		
		expected_output="HelloWorld!"

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		
		if [ "$test_output" != "$expected_output" ];
			let "part5_points=0"
		fi

		# Make sure that the students used fork and wait properly
		TEST_1=$(cat mp3_part5.cpp | grep -E "fork();" | grep -vE "//")
		TEST_2=$(cat mp3_part5.cpp | grep -E "(wait|waitpid)\(.*\)" | grep -vE "//")

		if [ "$TEST_1" == "" ]; then
			echo "mp3_part5.cpp   ...   Fork not used correctly" >> $vocareumReportFile
			let "part5_points=0"
		fi
		
		if [ "$TEST_2" == "" ]]; then
			echo "mp3_part5.cpp   ...   Wait/Waitpid not used correctly" >> $vocareumReportFile 
			let "part5_points=0"
		fi
	else
		echo "mp3_part5.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part5_points=0"
	fi
fi

if [ "$part_6" == "true" ]; then
	make part6 # &> /dev/null

	if [ "$?" == "0" ]; then
		test_output=$(./part6)
		expected_output=$(ps -elf)

		# Strip leading and trailing spaces/tabs
		test_output=$(echo $test_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')
		expected_output=$(echo $expected_output | sed 's/^[ \t]*//g' | sed 's/[ \t]*$//g')

		if [ "$expected_output" != "$test_output" ]; then
			let "part6_points=0"
		fi

		# Tests to make sure that the student isn't cheating

	else
		echo "mp3_part6.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part6_points=0"
	fi
fi

if [ "$part_7" == "true" ]; then
	make part7 # &> /dev/null	

	if [ "$?" == "0" ]; then
		test_output=$(./part7)
		expected_output=$(ls -la | tr [a-zA-Z0-9] a)
		
		if [ "$test_output" != "$expected_output" ]; then
			let "part7_points=0"
		fi

		# Tests to ensure that the student isn't cheating

	else
		echo "mp3_part7.cpp   ...   Compilation Failed!" >> $vocareumReportFile
		let "part7_points=0"
	fi
fi

# Clean up the project directory at the end
make clean


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
	echo "part2,5" >> $vocareumGradeFile
fi


# PART_3
if [ "$part3_points" == "5" ]; then
	echo "mp3_part3.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part3,5" >> $vocareumGradeFile
else
	echo "mp3_part3.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part3,5" >> $vocareumGradeFile
fi


# PART_4
if [ "$part4_points" == "5" ]; then
	echo "mp3_part4.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part4,5" >> $vocareumGradeFile
else
	echo "mp3_part4.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part4,5" >> $vocareumGradeFile
fi


# PART_5
if [ "$part5_points" == "15" ]; then
	echo "mp3_part5.cpp   ...   Passed [5/5]" >> $vocareumReportFile
	echo "part5,15" >> $vocareumGradeFile
else
	echo "mp3_part5.cpp   ...   Failed [0/5]" >> $vocareumReportFile
	echo "part5,15" >> $vocareumGradeFile
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

exit 0
