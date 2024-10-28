target: sudoku

sudoku: main.o sudoku.o
	g++ -Wall main.o sudoku.o -o sudoku

main.o: main.cpp
	g++ -Wall -c main.cpp -o main.o

sudoku.o: sudoku.cpp
	g++ -Wall -c sudoku.cpp -o sudoku.o

clean:
	rm *.o
