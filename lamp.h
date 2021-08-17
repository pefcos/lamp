/*
    Initial concept by: Arthur Allebrandt Werlang.
    Final design and interpreter implementation: Pedro Fronchetti Costa da Silva.

    Lamp interpreter.
*/

#include "types/lamp.h" // Includes constants.h
#include "types/switch.h"
#include "storage/storage.h"

/*
    Validates a lamp/switch name.

    char *name: name to validate.
*/
int validate_name(char *name);

/*
    Calculates the hashtable address through a function that uses the name to calculate an index.

    char *name: Name of the lamp/switch.
*/
unsigned short int calc_hash(char *name);

/*
    Gets the first word in the file.

    FILE *source: File to get the word from.
*/
char *get_word(FILE *source);

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage);