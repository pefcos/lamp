/*
    Initial concept by: Arthur Allebrandt Werlang.
    Final design and interpreter implementation: Pedro Fronchetti Costa da Silva.

    Lamp interpreter.
*/
#include <stdio.h>
#include <stdlib.h>

// Length of the hash table that stores lamps and switches. Prime number large enough to store plenty of stuff without collisions.
#define HASHTABLE_LEN 601
// Maximum name length for variable. Ex: swi
#define MAX_VARNAME_LEN 255
// Maximum name length for word. Ex. swi.on.on.off.off.on.on.off.on.off.off
#define MAX_WORD_LEN 655
// Maximum number of accesses in a word (.on and .off)
#define MAX_ACCESSES 400

#define ERROR -1
#define OFF 0
#define ON 1

/*
    Lamp struct.
    Stores the lamp name and value.
*/
struct lamp 
{
    char *name;
    unsigned char value;
};

typedef struct lamp Lamp;

/*
    Cell of the lamp hashtable.
    Hashtable conflicts are solved through simply linked list approach.
*/
struct lamp_hashcell 
{
    Lamp *content;
    struct lamp_hashcell *next;
};

typedef struct lamp_hashcell L_HC;

/*
    Switch item struct.
    Stores the directions, length of direction array and content.
    Directions are stored as an array of OFF and ON, where it contains the directions (.off.on.off, etc...).
*/
struct lswitch_item
{
    unsigned char *directions;
    int dir_arr_len; // Length of directions array.
    unsigned char value;
};

typedef struct lswitch_item LampSwitchItem;

/*
    Switch struct, represented by an array of structs.
    Stores the name, length of array of items and array of items (content).
*/
struct lswitch
{
    char *name;
    int item_arr_len; // Length of item array.
    LampSwitchItem **item_arr; // Array of pointers to items.
};

typedef struct lswitch LampSwitch;

/*
    Cell of the switch hashtable.
    Hashtable conflicts are solved through simply linked list approach.
*/
struct lswitch_hashcell 
{
    LampSwitch *content;
    struct lswitch_hashcell *next;
};

typedef struct lswitch_hashcell S_HC;

/*
    Struct that takes care of the storage of lamps and switches in hashtables.
*/
struct storage 
{
    L_HC lamps[HASHTABLE_LEN];
    S_HC switches[HASHTABLE_LEN];
};

typedef struct storage Storage;

/*
    Struct that stores circuit references in an array of fseek offsets from SEEK_SET.
*/
struct circ_refs
{
    int length;
    int *offsets;
};

typedef struct circ_refs CircRefs;

/*
    Validates a lamp/switch name.

    char *name: name to validate.
*/
int validate_name(char *name);

/*
    Creates a lamp and allocates the necessary memory.
    Returns a lamp ponter.

    char *name: Name of the lamp;
    unsigned char initial_value: Value of the lamp, can be 1 or 0.
*/
Lamp *create_lamp(char *name, unsigned char initial_value);

/*
    Calculates the hashtable address through a function that uses the name to calculate an index.

    char *name: Name of the lamp/switch.
*/
unsigned short int calc_hash(char *name);

/*
    Creates a storage and returns the pointer to it.
*/
Storage *create_storage();

/*
    Stores a lamp in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    Lamp *lamp: Lamp to be saved.
*/
int store_lamp(Storage *storage, Lamp *lamp);

/*
    Retrieves a lamp in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved.
*/
Lamp *get_lamp(Storage *storage, char *name);

/*
    Retrieves a switch in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the switch to be retrieved.
*/
LampSwitch *get_switch(Storage *storage, char *name);

/*
    Assigns value to the lamp with given name.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved;
    int value: Value to be assigned.
*/
int assign_to_lamp(Storage *storage, char *name, unsigned char value);

/*
    Gets the first word in the file.

    FILE *source: File to get the word from.
*/
char *get_word(FILE *source);

/*
    Prints a lamp's value.

    Lamp *lamp: Lamp to display.
*/
void display_lamp(Lamp *lamp);

/*
    Converts an input string of directions into a number of native direction values.

    char *directions: String containing input directions.
*/
unsigned char *convert_to_number_directions(char *directions);

/*
    Counts number of trailing ')' chars in word.

    char *word: Word to count ')' in.
*/
int count_close(char *word);

/*
    Counts number of leading '(' chars in word.

    char *word: Word to count '(' in.
*/
int count_open(char *word);

/*
    Calculates the next expected direction for switch.

    unsigned char *dir_arr: Direction array;
    int dir_arr_len: Length of direction array;
    int *new_length: Pointer to a variable to hold the length of the new array.
*/
unsigned char *next_directions(unsigned char *dir_arr, int dir_arr_len, int *new_length);

/*
    Appends a LampSwitchItem to a LampSwitch.

    LampSwitch *lswitch: LampSwitch to recieve the item;
    LampSwitchItem *lsi: Item to append.
*/
void append_to_switch(LampSwitch *lswitch, LampSwitchItem *lsi);

/*
    Makes a switch based on words from file.

    FILE *source: File to get switch from;
    char *name: Name of the switch.
*/
LampSwitch *make_switch(FILE *source, char *name);  

/*
    Checks if a switch element has specific directions as its prefix.
    Returns 1 if the prefix matches, 0 otherwise.

    LampSwitchItem *lsi: Switch element to check;
    unsigned char *prefix: Directions of the prefix;
    int pre_len: Length of the prefix array.
*/
int check_direction_prefix(LampSwitchItem *lsi, unsigned char *prefix, int pre_len);

/*
    Creates a copy of a switch element without the specified prefix.

    LampSwitchItem *lsi: Switch element to copy;
    unsigned char *prefix: Directions of the prefix;
    int pre_len: Length of the prefix array.
*/
LampSwitchItem *copy_without_prefix(LampSwitchItem *lsi, unsigned char *prefix, int pre_len);

/*
    Gets a switch or a lamp from a switch.

    LampSwitch *lswitch: Switch to get item from;
    unsigned char *directions_raw: Directions in numeric format.
*/
LampSwitch *get_switch_element(LampSwitch *lswitch, unsigned char *directions_raw); 

/*
    Displays a switch.

    LampSwitch *lswitch: Switch to display.
*/
void display_switch(LampSwitch *lswitch);

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage);