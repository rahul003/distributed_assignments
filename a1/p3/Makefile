all: clean p0 p1 p2

p0: p0.cpp
	g++ -w p0.cpp -o p0 -I. -lboost_thread-mt

p1: p1.cpp
	g++ -w p1.cpp -o p1 -I. -lboost_thread-mt
p2: p2.cpp
	g++ -w p2.cpp -o p2 -I. -lboost_thread-mt
clean:
	rm -f p0 p1 p2