all: main

main: main.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g main.cpp helper.cpp process.cpp thread_helper.cpp -o bin/main -I. -lboost_thread-mt

clean:
	rm -f bin/*