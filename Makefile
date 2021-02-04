# -*- MakeFile -*-

my_ls: main.o my_ls.o my_ls_smalls.o
	gcc main.o my_ls.o my_ls_smalls.o -fsanitize=address -o my_ls

main.o: main.c my_ls.h
	gcc -c -g3 -Wall -Werror -Wextra main.c

my_ls.o: my_ls.c my_ls.h
	gcc -c -g3 -Wall -Werror -Wextra my_ls.c

my_ls_smalls.o: my_ls_smalls.c my_ls.h
	gcc -c -g3 -Wall -Werror -Wextra my_ls_smalls.c