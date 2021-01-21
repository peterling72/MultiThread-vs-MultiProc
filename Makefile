all:
	echo "Error. Please make with either 'proc' or 'thread'"
proc:
	gcc -Wall -Werror -std=c99 -g -o searchtest searchtest.c multitest_proc.c -lm
thread:
	gcc -Wall -Werror -std=c99 -g -o searchtest searchtest.c multitest_thread.c -lm -lpthread
clean:
	rm -f searchtest
