target: sudoku

sudoku: main.o sudoku.o
	g++ main.o sudoku.o -o sudoku

main.o: main.cpp
	g++ -c main.cpp -o main.o

sudoku.o: sudoku.cpp
	g++ -c sudoku.cpp -o sudoku.o

clean:
	rm *.o
