#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "../constants/constants.h"
#endif

/*
    Struct that stores offsets before function calls.
    Stack implemented as linked list.
*/
struct stackframe
{
    long int offset;
    struct stackframe *previous;
};

typedef struct stackframe Stack;

/*
    Validates a lamp/switch name.

    char *name: name to validate.
*/
int validate_name(char *name);

/*
    Checks if word contains ().

    char *name: Word to check.
*/
int has_parentheses(char *word);

/*
    Removes () characters from word.

    char *word: Word to trim '()'.
*/
char *trim_parentheses(char *word);

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
    Recounts lines from the start of the file. Useful to update line count after circuit calls.
    Returns current line number on file cursor.

    FILE *source: File to find cursor line position.
*/
int which_line(FILE *source);

/*
    Gets the first word in the file.

    FILE *source: File to get the word from.
*/
char *get_word(FILE *source);

/*
    Gets the first word in the file without advancing the cursor in the file.

    FILE *source: File to peek the word from.
*/
char *peek_word(FILE *source);

/*
    Adds the offset to the top (start) of the stack.

    long int offset: Offset to store;
    Stack *stack: Stack to push into.
*/
void stack_push(long int offset, Stack **stack);

/*
    Retrieves and removes the offset from the top (start) of the stack.

    long int offset: Offset to store;
    Stack *stack: Stack to pop from.
*/
long int stack_pop(Stack **stack);

/*
    Duplicates a string, returning an allocated address.

    char *str: String to duplicate.
*/
char *duplicate_string(char *str);

/*
    Checks if a string has a '.'.

    char *str: String to check.
*/
int is_switch_element_accessor(char *str);

/*
    Checks if string starts with comment indicator.
*/
int starts_with_comment(char *str);

/*
    Checks if string ends with comment indicator.
*/
int ends_with_comment(char *str);

/*
    Prints end message and gives more information on errors.
*/
void print_end_message(int execution_end, char *word, int line);
