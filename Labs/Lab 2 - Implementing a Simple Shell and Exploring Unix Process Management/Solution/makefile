all: myshell looper

# Link the object files to create the executable
myshell: myshell.o LineParser.o LineParser.h
	gcc myshell.o LineParser.o -o myshell

# Compile myshell.c to myshell.o and specify header dependencies
myshell.o: myshell.c
	gcc -Wall -c myshell.c -o myshell.o

# Compile LineParser.c to LineParser.o and specify header dependencies
LineParser.o: LineParser.c LineParser.h
	gcc -Wall -c LineParser.c -o LineParser.o

# Compile looper.c to looper.o and specify header dependencies
looper: looper.c 
	gcc -Wall -c looper.c -o looper.o
	gcc -g -o looper looper.o

# Clean up the build dire~ctory
clean:
	rm -f myshell.o looper.o LineParser.o myshell looper