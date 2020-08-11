assembler : collect_input.o main.o asembler.o validation.o
	gcc -g -ansi -Wall collect_input.o main.o asembler.o validation.o -lm -pedantic -o assembler

collect_input.o : collect_input.c asembler.h
	gcc -c -g -ansi collect_input.c -lm -Wall -pedantic -o collect_input.o 

validation.o : validation.c asembler.h
	gcc -c  -g -ansi validation.c -lm -Wall -pedantic -o validation.o 

asembler.o : asembler.c asembler.h
	gcc -c -g -ansi asembler.c -lm -Wall -pedantic -o asembler.o 

main.o : main.c asembler.h
	gcc -c  -g -ansi main.c -lm -Wall -pedantic -o main.o 
