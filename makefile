all: clean lampi

# Current version of lampi being compiled.
VERSION = 1.0.1

# Eventual flags for the compiler.
FLAGS = 

# Name of the output file.
OUT_NAME = lampi 

# Files to input in compilation.
FILE_LIST = ./main.c types/lamp.c types/switch.c storage/storage.c storage/namespaces.c utils/utils.c interpreter/interpreter.c

# Primary make target.
lampi:
	gcc $(FLAGS) -o ./bin/v$(VERSION)/$(OUT_NAME) $(FILE_LIST) 

# rm -f doesn't check for file existance, so all works everytime.
clean:
	rm -f ./bin/v$(VERSION)/$(OUT_NAME)
