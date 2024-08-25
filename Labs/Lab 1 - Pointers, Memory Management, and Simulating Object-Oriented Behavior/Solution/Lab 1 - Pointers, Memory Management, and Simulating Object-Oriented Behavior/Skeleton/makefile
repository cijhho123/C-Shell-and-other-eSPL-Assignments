addresses : addresses.c
	# compile
	gcc -g -m32 -Wall -c -o addresses.o addresses.c

	#link
	gcc -m32 -g -o addresses addresses.o

test : test.c
	# compile
	gcc -g -m32 -Wall -c -o test.o test.c

	#link
	gcc -m32 -g -o test test.o
	

clean :
	rm *.o
	rm addresses test
