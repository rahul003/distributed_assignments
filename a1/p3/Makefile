all: p0 p1 p2 p3 p4

p0: p0.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g p0.cpp helper.cpp process.cpp thread_helper.cpp -o bin/p0 -I. -lboost_thread-mt
p1: p1.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g p1.cpp helper.cpp process.cpp thread_helper.cpp -o bin/p1 -I. -lboost_thread-mt
p2: p2.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g p2.cpp helper.cpp process.cpp thread_helper.cpp -o bin/p2 -I. -lboost_thread-mt
p3: p3.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g p3.cpp helper.cpp process.cpp thread_helper.cpp -o bin/p3 -I. -lboost_thread-mt
p4: p4.cpp include/process.h include/helper.h include/thread_helper.h include/main_header.h
	g++ -w -g p4.cpp helper.cpp process.cpp thread_helper.cpp -o bin/p4 -I. -lboost_thread-mt

clean:
	rm -f bin/*