#make clean all
gdb -q client_solution -x MP7_grading_python_source.py 2>&1 | tee grading_output.txt 
