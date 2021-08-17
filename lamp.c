/*
    Pedro Fronchetti Costa da Silva, 08/2021
    Lamp interpreter.
*/
#include <math.h>
#include <string.h>

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
    Creates a lamp and allocates the necessary memory.
    Returns a lamp ponter.

    char *name: Name of the lamp;
    unsigned char initial_value: Value of the lamp, can be 1 or 0.
*/
Lamp *create_lamp(char *name, unsigned char initial_value) 
{
    if (name != NULL && !validate_name(name))
        return NULL;
    Lamp *pointer = (Lamp*) malloc(sizeof(Lamp));
    pointer->name = (char*) malloc(sizeof(*name));
    if (name != NULL)
        strcpy(pointer->name,name);
    else
        pointer->name = NULL;
    pointer->value = initial_value;
    return pointer;
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
    Creates a storage and returns the pointer to it.
*/
Storage *create_storage()
{
    Storage *storage = (Storage*) malloc(sizeof(Storage));
    register int i = 0;
    for (i = 0; i < HASHTABLE_LEN; i++)
    {
        (storage->lamps)[0].content = NULL;
        (storage->lamps)[0].next = NULL;
    }
    for (i = 0; i < HASHTABLE_LEN; i++)
    {
        (storage->switches)[0].content = NULL;
        (storage->switches)[0].next = NULL;
    }
    return storage;
}

/*
    Stores a lamp in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    Lamp *lamp: Lamp to be saved.
*/
int store_lamp(Storage *storage, Lamp *lamp)
{
    L_HC *cell = &((storage->lamps)[calc_hash(lamp->name)]);
    L_HC *new_end = NULL;
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,lamp->name))
            return 0; // Error for already existing name within storage.
        if (cell->next != NULL)
            cell = cell->next;
        else
        {
            // Really bad code, fast bugfix, to fix requires changing storage struct definition to array of pointers.
            new_end = (L_HC*) malloc(sizeof(L_HC));
            new_end->content = lamp;
            new_end->next = NULL;
            cell->next = new_end;
            return 1;
        }
    }
    cell->content = lamp;
    cell->next = NULL; 
    return 1;
}

/*
    Retrieves a lamp in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved.
*/
Lamp *get_lamp(Storage *storage, char *name)
{
    L_HC *cell = &((storage->lamps)[calc_hash(name)]);
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,name))
            return cell->content;
        if (cell->next != NULL)
            cell = cell->next;
        else
            return NULL;
    }
    return NULL;
}

/*
    Retrieves a switch in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the switch to be retrieved.
*/
LampSwitch *get_switch(Storage *storage, char *name)
{
    S_HC *cell = &((storage->switches)[calc_hash(name)]);
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,name))
            return cell->content;
        if (cell->next != NULL)
            cell = cell->next;
        else
            return NULL;
    }
    return NULL;
}

/*
    Assigns value to the lamp with given name.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved;
    int value: Value to be assigned.
*/
int assign_to_lamp(Storage *storage, char *name, unsigned char value)
{
    Lamp *lamp = get_lamp(storage,name);
    if (lamp == NULL)
        return 0;
    lamp->value = value;
    return 1;
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
    printf("get_word COUNTER -> %d\n",counter);
    fgets(word,counter,source);
    return word;
}

/*
    Prints a lamp's value.

    Lamp *lamp: Lamp to display.
*/
void display_lamp(Lamp *lamp)
{
    if (lamp->value)
        printf("on\n");
    else
        printf("off\n");
}

/*
    Converts an input string of directions into a number of native direction values.
    The first element of the returned array is length. NOT A DIRECTION.

    char *directions: String containing input directions.
*/
unsigned char *convert_to_number_directions(char *directions)
{
    register int i = 0;
    register int index = 0; // Counts how many directions were stored in dir_arr.
    unsigned char dir_arr[MAX_ACCESSES] = {0}; // Stores directions in the stack. Later, these will be stored in heap.
    unsigned char *result = NULL; // Final result pointer.
    char *pointer = NULL; // Pointer for strtok reference.
    pointer = strtok(directions,"."); // Gets switch name, not important.
    printf("starting in switch %s\n",pointer);
    // Fills dir_arr;
    pointer = strtok(NULL,".");
    while (pointer != NULL)
    {
        printf("start...  %s\n",pointer);
        if (!strcmp(pointer,"on"))
        {
            dir_arr[index] = ON;
            index++;
        }
        else if (!strcmp(pointer,"off"))
        {
            dir_arr[index] = OFF;
            index++;
        }
        else
            return NULL; // Error, invalid value.
        pointer = strtok(NULL,".");
    }
    // First element of array is length. NOT A DIRECTION.
    result = (unsigned char*) malloc(1 + index * sizeof(unsigned char));
    result[0] = (unsigned char) index;
    for(i = 1; i <= index; i++)
    {
        result[i] = dir_arr[i-1];
        printf("resulti = %d\n",(int) result[i]);
    }
    return result;
}

/*
    Counts number of trailing ')' chars in word.

    char *word: Word to count ')' in.
*/
int count_close(char *word)
{
    register int i = 0;
    char *intermediary = NULL;
    while (word[i] != '\0')
        i++;
    intermediary = word + i;
    i = -1;
    while (intermediary[i] == ')')
        i--;
    return (i+1) * -1;
}

/*
    Counts number of leading '(' chars in word.

    char *word: Word to count '(' in.
*/
int count_open(char *word)
{
    register int i = 0;
    while (word[i] == '(')
        i++;
    return i;
}

/*
    Removes () characters and processes word value.

    char *word: Word to trim '()' from and extract value.
*/
int get_value(char *word)
{
    register int i = 0; 
    char copy[MAX_WORD_LEN] = "\0";
    while (word[i] == '(')
        i++;
    strcpy(copy,word+i);
    i = 0;
    while (copy[i] != ')' && copy[i] != '\0')
    {
        i++;
    }
    copy[i] = '\0';
    if (!strcmp(copy,"on"))
        return ON;
    if (!strcmp(copy,"off"))
        return OFF;
    return ERROR;
}

/*
    Calculates the next expected direction for switch.

    unsigned char *dir_arr: Direction array;
    int dir_arr_len: Length of direction array;
    int *new_length: Pointer to a variable to hold the length of the new array.
*/
unsigned char *next_directions(unsigned char *dir_arr, int dir_arr_len, int *new_length)
{
    register int i = 0;
    int last_off_index = -1; // Last off index stores the offset (array position) of the last OFF direction.
    unsigned char *result = NULL;
    for (i = 0; i < dir_arr_len; i++)
        if (dir_arr[i] == OFF)
            last_off_index = i;
    if (last_off_index >= 0)
    {
        result = malloc((last_off_index + 1) * sizeof(unsigned char));
        for (i = 0; i < last_off_index; i++)
            result[i] = dir_arr[i];
        result[last_off_index] = ON;
    }
    *new_length = last_off_index + 1;
    return result; // Returns NULL if finished.
}

/*
    Appends a LampSwitchItem to a LampSwitch.

    LampSwitch *lswitch: LampSwitch to recieve the item;
    LampSwitchItem *lsi: Item to append.
*/
void append_to_switch(LampSwitch *lswitch, LampSwitchItem *lsi)
{
    register int i = 0;
    LampSwitchItem **new_arr = malloc((lswitch->item_arr_len + 1) * sizeof(LampSwitchItem*));
    for (i = 0; i < lswitch->item_arr_len; i++)
        new_arr[i] = (lswitch->item_arr)[i];
    new_arr[lswitch->item_arr_len] = lsi;
    (lswitch->item_arr_len)++;
    (lswitch->item_arr) = new_arr;
}

/*
    Makes a switch based on words from file.
    The order of inserting is: recalculate directions, append to array in dir position (IF DIR != NULL).

    FILE *source: File to get switch from;
    char *name: Name of the switch.
*/
LampSwitch *make_switch(FILE *source, char *name)
{
    register int i = 0;
    register int j = 0;
    char *word = NULL;
    int open = 0;
    int close = 0;
    int cdir_len = 0; // Length of current directions array.
    int aux_len = 0; // Used in next_directions function as length of aux_directions.
    unsigned char *current_directions = NULL; //Stores current directions. This works as a cursor to situate the next information that should be recieved.
    unsigned char *aux_directions = NULL; // Helps expanding the original array.
    LampSwitch *result = NULL; // No lamp switch to start.
    LampSwitchItem *to_add = NULL; // Item to add to switch.
    // Initializes variables
    result = (LampSwitch*) malloc(sizeof(LampSwitch));
    result->item_arr = NULL;
    result->item_arr_len = 0;
    result->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(result->name,name);
    do
    {
        if (aux_directions != NULL) 
            free(aux_directions); // Frees aux_directions (end loop condition).
        aux_directions = NULL;
        if (word != NULL)
            free(word); // Frees previous iteration word.
        word = get_word(source);
        if (word == NULL)
            return NULL; // Error for invalid switch due to never reaching the NULL condition on current_directions.
        open = count_open(word);
        close = count_close(word);
        if (open > 0 && close > 0)
            return NULL;  // Error for invalid switch item, opening and closing in the same item.
        for (i = 0; i < open; i++)
        {
            aux_directions = malloc((cdir_len + 1) * sizeof(unsigned char));
            for (j = 0; j < cdir_len; j++)
                aux_directions[j] = current_directions[j]; // Copies array elements to new array.
            free(current_directions);
            current_directions = aux_directions;
            aux_directions = NULL;
            // Insert element at end of direction array. cdir_len has not been incremented yet.
            current_directions[cdir_len] = OFF;
            cdir_len++;
        }
        to_add = (LampSwitchItem*) malloc(sizeof(LampSwitchItem));
        to_add->dir_arr_len = cdir_len;
        to_add->directions = current_directions;
        to_add->value = get_value(word);   
        append_to_switch(result,to_add);                           
        //Test end condition and update directions.
        aux_directions = next_directions(current_directions,cdir_len,&aux_len);
        free(current_directions);
        current_directions = aux_directions;
        aux_directions = NULL;
        cdir_len = aux_len;         
    } while (cdir_len != 0);
    return result;
}

/*
    Makes a switch based on words from file.

    FILE *source: File to get switch from;
    unsigned char *directions: Directions in numeric format.
*/
LampSwitch *get_switch_element(LampSwitch *lswitch, unsigned char *directions)
{
    
}

/*
    Displays a switch.

    LampSwitch *lswitch: Switch to display.
*/
void display_switch(LampSwitch *lswitch)
{
    register int i = 0;
    register int j = 0;
    int amount = 0;
    unsigned char last_direction = 0;
    LampSwitchItem *current = NULL;
    for (i = 0; i < lswitch->item_arr_len; i++)
    {
        amount = 1;
        current = (lswitch->item_arr)[i];
        // Gets the last direction of the directions array;
        last_direction = (current->directions)[(current->dir_arr_len) - 1];
        j = current->dir_arr_len - 2;
        while (current->directions[j] == last_direction && j >= 0)
        {
            amount++;
            j--;
        }
        if (last_direction == OFF)
            for (j = 0; j < amount; j++)
                printf("(");
        if (current->value == OFF)
            printf("off");
        else
            printf("on");
        if (last_direction == ON)
            for (j = 0; j < amount; j++)
                printf(")");
        printf(" ");
    }
    printf("\n");
}

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage)
{
    char sname[35] = "roger";
    LampSwitch *lswitch = make_switch(source,sname);
    display_switch(lswitch);
    return 1;
}

int main(int argc, char *argv[])
{
    Storage *storage = create_storage();
    FILE *source = NULL;
    char fn[10] = "test.lamp";
    /* switch(argc)
    {
    case 1:
        printf("No file to interpret. To specify a file, call the interpreter using \'lamp <filename.lamp>\'.\n");
        break;
    
    case 2: */
        source = fopen(fn,"r");
        if (source == NULL)
        {
            printf("Error opening file %s. Please specify the full filename with the extension and remember that uppercase and lowercase letters are different.\n",argv[1]);
            //break;
        }
        if (interpret(source, storage))
            printf("Program finished successfully.\n");
        else
            printf("Program execution interrupted.\n");
        //break;
    
    /* default:
        printf("Too many arguments! Please execute the program using \'lamp <filename.lamp>\'.\n");
    } */
    
    return 0;
}