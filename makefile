all: clean lampi

# C compiler used.
CC = gcc

# Eventual flags for the compiler.
FLAGS = 

# Name of the output file.
OUT_NAME = lampi 

# Files to input in compilation.
FILE_LIST = ./main.c types/lamp.c types/switch.c storage/storage.c storage/namespaces.c utils/utils.c interpreter/interpreter.c

# Primary make target.
lampi:
	$(CC) $(FLAGS) -o ./bin/$(OUT_NAME) $(FILE_LIST) 

# rm -f doesn't check for file existance, so all works everytime.
clean:
	rm -f ./bin/$(OUT_NAME)
