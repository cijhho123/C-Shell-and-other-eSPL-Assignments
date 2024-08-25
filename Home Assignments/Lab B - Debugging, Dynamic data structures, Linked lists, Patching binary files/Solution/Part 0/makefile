all: bubblesort hexaPrint


# bubblesort
bubblesort: bubblesort.c
	gcc -Wall -c bubblesort.c -g -o bubblesort.o
	gcc bubblesort.o -o bubblesort

# hexaPrint
hexaPrint: hexaPrint.c
	gcc -Wall -c hexaPrint.c -g -o hexaPrint.o
	gcc hexaPrint.o -o hexaPrint


# Clean 
clean:
	rm -f ./*.o  bubblesort hexaPrint