all: myshell looper mypipeline

# Link the object files to create the executable
myshell: myshell.o LineParser.o LineParser.h
	gcc myshell.o LineParser.o -o myshell

# Compile myshell.c to myshell.o and specify header dependencies
myshell.o: myshell.c
	gcc -Wall -g -c myshell.c -o myshell.o

# Link the object files to create the executable
mypipeline: mypipeline.o LineParser.o LineParser.h
	gcc mypipeline.o LineParser.o -o mypipeline

# Compile mypipeline.c to mypipeline.o and specify header dependencies
mypipeline.o: mypipeline.c
	gcc -Wall -c mypipeline.c -o mypipeline.o


# Compile LineParser.c to LineParser.o and specify header dependencies
LineParser.o: LineParser.c LineParser.h
	gcc -Wall -c LineParser.c -o LineParser.o

# Compile looper.c to looper.o and specify header dependencies
looper: looper.c 
	gcc -Wall -c looper.c -o looper.o
	gcc -g -o looper looper.o

# Clean up the build dire~ctory
clean:
	rm -f *.o myshell looper mypipeline