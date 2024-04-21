all: exec

exec: base.cpp
	g++ -o exec base.cpp

clean:
	rm -rf *.o exec