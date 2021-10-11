#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "../constants/constants.h"
#endif

#ifndef UTILS_H
#define UTILS_H
#include "../utils/utils.h"
#endif

#ifndef NAMESPACES_H
#define NAMESPACES_H
#include "../storage/namespaces.h"
#endif

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
    Returns a name without directions.
    BEWARE free the return.

    char *name: Name to trim.
*/
char *switch_name(char *name);

/*
    Displays a switch.

    LampSwitch *lswitch: Switch to display.
*/
void display_switch(LampSwitch *lswitch);

/*
    Deletes a switch and frees the allocated memory.

    LampSwitch *lswitch: Switch to delete.
*/
void delete_switch(LampSwitch *lswitch);

/*
    Checks if the given switch is in fact a lamp.

    LampSwitch *lswitch: Switch to check.
*/
int is_lamp(LampSwitch *lswitch);