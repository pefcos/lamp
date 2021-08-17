FLAGS = # Flags for the compiler.

OUT_NAME = lamp # Name of the output file.

FILE_LIST = lamp.c # Files to input in compilation.

# Primary make target.
lamp:
	gcc $(FLAGS) -o $(OUT_NAME) $(FILE_LIST)