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


void print_dirs(unsigned char *directions, int dir_len);

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
    Concatenates two direction arrays.
    Does NOT free any of the arguments!
*/
unsigned char *concat_directions(unsigned char *dir_arr1, int dir_arr1_len, unsigned char *dir_arr2, int dir_arr2_len);

/*
    Calculates the next expected direction for switch.

    unsigned char *dir_arr: Direction array;
    int dir_arr_len: Length of direction array;
    int *new_length: Pointer to a variable to hold the length of the new array.
*/
unsigned char *next_directions(unsigned char *dir_arr, int dir_arr_len, int *new_length);

/*
    Creates a switch with no items.

    char *name: Name of the new switch.
*/
LampSwitch *new_switch(char *name);

/*
    Appends a LampSwitchItem to a LampSwitch.

    LampSwitch *lswitch: LampSwitch to recieve the item;
    LampSwitchItem *lsi: Item to append.
*/
void append_to_switch(LampSwitch *lswitch, LampSwitchItem *lsi);

/*
    Copies items from a switch to another switch, adding a specified prefix.
*/
void copy_items_with_prefix(LampSwitch *destination, LampSwitch *origin, unsigned char *prefix, int prefix_len);

/*
    make_switch definition was moved to interpreter.h/interpreter.c due to dependency issues.
*/

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

    LampSwitch *lswitch: Switch to display;
    unsigned char block: If printing as block.
*/
void display_switch(LampSwitch *lswitch, unsigned char block);

/*
    Deletes a switch and frees the allocated memory.

    LampSwitch *lswitch: Switch to delete.
*/
void delete_switch(LampSwitch *lswitch);

/*
    Inverts on and off values in the switch.

    LampSwitch *lswitch: Switch to invert.
*/
void invert_switch(LampSwitch *lswitch);

/*
    Checks if the given switch is in fact a lamp.

    LampSwitch *lswitch: Switch to check.
*/
int is_lamp(LampSwitch *lswitch);

/*
    Returns a pointer to a LampSwitch with the same content as a given argument LampSwitch.

    LampSwitch *lswitch: Switch to duplicate;
    char *name: Name of the copy.
*/
LampSwitch *duplicate_switch(LampSwitch *lswitch, char *name);

/*
    Creates a new LampSwitchItem.

    unsigned char *directions: Directions of the element;
    int dir_len: Length of the direction array;
    unsigned char value: Value of the item.
*/
LampSwitchItem *new_switch_item(unsigned char *directions, int dir_len, unsigned char value);

/*
    Adds an off suffix to the direction.
    Example:
    on.off -> on.on.off
    off -> on.off
    on -> ERROR!

    unsigned char *directions: Directions to append the off suffix to
*/
unsigned char *add_direction_off_suffix(unsigned char *directions, int *dir_len);
