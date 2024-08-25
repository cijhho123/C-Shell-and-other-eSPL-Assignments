all : *.c


addresses : addresses.c
	# compile
	gcc -g -m32 -Wall -c -o addresses.o addresses.c

	#link
	gcc -m32 -g -o addresses addresses.o

base : base.c
	# compile
	gcc -g -m32 -Wall -c -o base.o base.c

	#link
	gcc -m32 -g -o base base.o

menu_map : menu_map.c
	# compile
	gcc -g -m32 -Wall -c -o menu_map.o menu_map.c

	#link
	gcc -m32 -g -o menu_map menu_map.o

	

clean :
	rm -f *.o
	rm -f addresses
	rm -f base
	rm -f menu_map