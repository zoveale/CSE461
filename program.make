Program: Sdisk.o Filesys.o Shell.o Table.o main.o
	g++ Sdisk.o Filesys.o Shell.o Table.o main.o -o Program

Table.o: Table.cpp
	g++ -c Table.cpp

Shell.o: Shell.cpp
	g++ -c Shell.cpp
	
Filesys.o: Filesys.cpp
	g++ -c Filesys.cpp

Sdisk.o: Sdisk.cpp
	g++ -c Sdisk.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o
