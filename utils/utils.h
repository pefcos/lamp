#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "../constants/constants.h"
#endif

/*
    Validates a lamp/switch name.

    char *name: name to validate.
*/
int validate_name(char *name);

/*
    Removes () characters and processes word value.

    char *word: Word to trim '()' from and extract value.
*/
int get_value(char *word);

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