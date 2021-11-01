#include <string.h>
#include "utils.h"

/*
    Validates a lamp/switch name.

    char *name: name to validate
*/
int validate_name(char *name)
{
    register int i = 0;
    while (name[i] != '\0')
    {
        if (name[i] == '.' || name[i] == ' ')
            return 0;
        i++;
    }
    return 1;
}

/*
    Checks if word contains ().

    char *name: Word to check.
*/
int has_parentheses(char *word)
{
    register int i = 0;
    for (i = 0; i < strlen(word); i++)
        if (word[i] == '(' || word[i] == ')')
            return 1;
    return 0;
}

/*
    Removes () characters from word.

    char *word: Word to trim '()'.
*/
char *trim_parentheses(char *word)
{
    register int i = 0; 
    char copy[MAX_WORD_LEN] = "\0";
    char *final = NULL;
    while (word[i] == '(')
        i++;
    strcpy(copy,word+i);
    i = 0;
    while (copy[i] != ')' && copy[i] != '\0')
    {
        i++;
    }
    copy[i] = '\0';
    // Mallocs and returns allocated string.
    final = (char*) malloc(sizeof(char) * strlen(copy));
    for (i = 0; i <= strlen(copy); i++)
    {
        final[i] = copy[i];
    }
    return final;
}

/*
    Removes () characters and processes word value.

    char *word: Word to trim '()' from and extract value.
*/
int get_value(char *word)
{
    register int i = 0; 
    char *copy = trim_parentheses(word);
    if (!strcmp(copy,"on"))
    {
        free(copy);
        return ON;
    }
    if (!strcmp(copy,"off"))
    {
        free(copy);
        return OFF;
    }
    free(copy);
    return ERROR;
}

/*
    Calculates the position of a lamp in the hashtable, given its name.

    char *name: Name of the lamp.
*/
unsigned short int calc_hash(char *name) 
{
    register int i = 0; // Index of char in string
    register int hashindex = 0; // Final hashtable access index
    while (name[i] != '\0') 
    { 
        hashindex += ((int) name[i]) * ((int) name[i]) * (i + 1);
        i++;
    }
    return (int) (hashindex % HASHTABLE_LEN);
}

/*
    Gets the first word in the file. 
    Returns NULL if the word is too big.

    FILE *source: File to get the word from.
*/
char *get_word(FILE *source)
{
    char *word = NULL;
    register char let = ' ';
    register int counter = 0;
    // Proceeds until the end of ' ', '\n' and '\t'.
    do
    {
        let = fgetc(source);
    } while (let == ' ' || let == '\n' || let == '\t');
    // Counts number of letters in word.
    while (let != ' ' && let != '\n' && let != '\0' && let != EOF && counter < MAX_WORD_LEN)
    {
        counter++;
        let = fgetc(source);
    }
    if (counter >= MAX_WORD_LEN || counter <= 0)
        return NULL;
    // Returns to the start of the word.
    counter++; // Includes space for the '\0' character.
    if (let != EOF)
        fseek(source,counter * -1,SEEK_CUR);
    else
        fseek(source,(counter - 1) * -1,SEEK_END); // Lazy fix for EOF bug.
    word = (char*) malloc(counter * sizeof(char));
    fgets(word,counter,source);
    return word;
}

/*
    Adds the offset to the top (start) of the stack.

    long int offset: Offset to store;
    Stack *stack: Stack to push into.
*/
void stack_push(long int offset, Stack **stack)
{
    Stack *new_frame = (Stack*) malloc(sizeof(Stack));
    new_frame->offset = offset;
    new_frame->previous = *stack;
    *stack = new_frame;
}

/*
    Retrieves and removes the offset from the top (start) of the stack.

    long int offset: Offset to store;
    Stack *stack: Stack to pop from.
*/
long int stack_pop(Stack **stack)
{
    Stack *new_stack = NULL;
    long int ret = 0;
    if (stack != NULL)
    {
        new_stack = (*stack)->previous;
        ret = (*stack)->offset;
        free(*stack);
        *stack = new_stack;
        return ret;
    }
    return -1;
}

/*
    Duplicates a string, returning an allocated address.

    char *str: String to duplicate.
*/
char *duplicate_string(char *str)
{
    register int i = 0;
    int len = strlen(str);
    char *new_str = (char*) malloc(len * sizeof(char));
    for (i = 0; i <= len; i++)
        new_str[i] = str[i];
    return new_str;
}

/*
    Checks if a string has a '.'.

    char *str: String to check.
*/
int is_switch_element_accessor(char *str)
{
    register int i = 0;
    while (str[i] != '\0' && str[i] != '.')
        i++;
    return (str[i] == '.');
}
