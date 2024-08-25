connect : src/main.c src/numbers.c src/add.s
	# compile
	nasm -g -f elf -w+all -o bin/add.o src/add.s
	gcc -g -m32 -Wall -c -o bin/numbers.o src/numbers.c
	gcc -g -m32 -Wall -c -o bin/main.o src/main.c

	#link
	gcc -m32 -g -o results/output bin/add.o bin/numbers.o bin/main.o
	

my_echo : src/my_echo.c
	gcc -g -m32 -Wall -c -o bin/my_echo.o src/my_echo.c
	gcc -m32 -g -o results/my_echo bin/my_echo.o

clean :
	rm bin/*.o
	rm results/*

