all: p0 p1 p2

p0: p0.cpp headers/process.h headers/helper.h
	g++ -w -g p0.cpp -o bin/p0 -I. -lboost_thread-mt

p1: p1.cpp headers/process.h headers/helper.h
	g++ -w -g p1.cpp -o bin/p1 -I. -lboost_thread-mt

p2: p2.cpp headers/process.h headers/helper.h
	g++ -w -g p2.cpp -o bin/p2 -I. -lboost_thread-mt
clean:
	rm -f bin/*