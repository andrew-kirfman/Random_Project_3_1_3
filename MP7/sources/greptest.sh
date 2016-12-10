nm -ACl --radix=d "$1" | grep -v "(.*)" | grep -v " [TtN\?-] " | grep -v "threadsafe_console_output" | grep "\.cpp:[0-9]*$" > /dev/null
if [ $? -eq 0 ]; then
	echo "$1 contains global variables"
else
	echo "$1 does not contain global variables"
fi
