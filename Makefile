all: exec

exec: main.o cpu.o file.o hd.o process.o
	g++ -o exec main.o cpu.o file.o hd.o process.o

main.o: main.cpp
	g++ -c main.o main.cpp

cpu.o: ./source/cpu.cpp ./header/cpu.h
	g++ -c cpu.o ./source/cpu.cpp

file.o: ./source/file.cpp ./header/file.h
	g++ -c file.o ./source/file.cpp

hd.o: ./source/hd.cpp ./header/hd.h
	g++ -c hd.o ./source/hd.cpp

process.o: ./source/process.cpp ./header/process.h
	g++ -c process.o ./source/process.cpp

clean:
	rm -rf *.o exec