all: clean lamp

# Eventual flags for the compiler.
FLAGS = 

# Name of the output file.
OUT_NAME = lamp 

# Files to input in compilation.
FILE_LIST = ./main.c types/lamp.c types/switch.c storage/storage.c storage/namespaces.c utils/utils.c interpreter/interpreter.c

# Primary make target.
lamp:
	gcc $(FLAGS) -o $(OUT_NAME) $(FILE_LIST) 

# rm -f doesn't check for file existance, so all works everytime.
clean:
	rm -f $(OUT_NAME)