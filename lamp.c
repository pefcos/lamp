/*
    Pedro Fronchetti Costa da Silva, 08/2021
    Lamp interpreter.
*/

#include "lamp.h"

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
    // Proceeds until the end of ' ' and '\n'.
    do
    {
        let = fgetc(source);
    } while (let == ' ' || let == '\n');
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
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage)
{
    char sname[35] = "roger";
    char dirs[35] = "roger.on.on.on.on";
    LampSwitch *lswitch = make_switch(source,sname);  
    display_switch(get_switch_element(lswitch,convert_to_number_directions(dirs)));
    return 1;
}

int main(int argc, char *argv[])
{
    Storage *storage = create_storage();
    FILE *source = NULL;
    switch(argc)
    {
    case 1:
        printf("No file to interpret. To specify a file, call the interpreter using \'lamp <filename.lamp>\'.\n");
        break;
    
    case 2:
        source = fopen(argv[1],"r");
        if (source == NULL)
        {
            printf("Error opening file %s. Please specify the full filename with the extension and remember that uppercase and lowercase letters are different.\n",argv[1]);
            break;
        }
        if (interpret(source, storage))
            printf("Program finished successfully.\n");
        else
            printf("Program execution interrupted.\n");
        break;
    
    default:
        printf("Too many arguments! Please execute the program using \'lamp <filename.lamp>\'.\n");
    }
    
    return 0;
}