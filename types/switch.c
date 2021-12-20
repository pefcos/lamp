#include "switch.h"

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
    // Fills dir_arr;
    pointer = strtok(NULL,".");
    while (pointer != NULL)
    {
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
    Concatenates two direction arrays.
    Does NOT free any of the arguments!
*/
unsigned char *concat_directions(unsigned char *dir_arr1, int dir_arr1_len, unsigned char *dir_arr2, int dir_arr2_len)
{
    register int i = 0;
    unsigned char *result = malloc((dir_arr1_len + dir_arr2_len) * sizeof(unsigned char));
    for (i = 0; i < dir_arr1_len; i++)
        result[i] = dir_arr1[i];
    for (i = 0; i < dir_arr2_len; i++)
        result[dir_arr1_len + i] = dir_arr2[i];
    return result;
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
    {
        if (dir_arr[i] == OFF)
            last_off_index = i;
    }
    if (last_off_index >= 0)
    {
        result = malloc((last_off_index + 1) * sizeof(unsigned char));
        for (i = 0; i < last_off_index; i++)
            result[i] = dir_arr[i];
        result[last_off_index] = ON;
    }
    if (dir_arr == NULL)
    {
        result = malloc(sizeof(unsigned char));
        result[0] = OFF;
        last_off_index = 0;
    }
    if (new_length != NULL)
        *new_length = last_off_index + 1;
    return result; // Returns NULL if finished.
}

/*
    Creates a switch with no items.

    char *name: Name of the new switch.
*/
LampSwitch *new_switch(char *name)
{
    LampSwitch *result = (LampSwitch*) malloc(sizeof(LampSwitch));
    result->item_arr = NULL;
    result->item_arr_len = 0;
    result->name = duplicate_string(name);
    return result;
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
    Copies items from a switch to another switch, adding a specified prefix.
*/
void copy_items_with_prefix(LampSwitch *destination, LampSwitch *origin, unsigned char *prefix, int prefix_len)
{
    register int i = 0;
    unsigned char *directions = NULL;
    LampSwitchItem *item = NULL;
    LampSwitchItem *to_add = NULL;

    for (i = 0; i < origin->item_arr_len; i++)
    {
        item = (origin->item_arr)[i];
        directions = concat_directions(
            prefix,
            prefix_len,
            item->directions,
            item->dir_arr_len);
        to_add = new_switch_item(directions,
            prefix_len + item->dir_arr_len,
            item->value);
        append_to_switch(destination,to_add);
    }
}

/*
    Checks if a switch element has specific directions as its prefix.
    Returns 1 if the prefix matches, 0 otherwise.

    LampSwitchItem *lsi: Switch element to check;
    unsigned char *prefix: Directions of the prefix;
    int pre_len: Length of the prefix array.
*/
int check_direction_prefix(LampSwitchItem *lsi, unsigned char *prefix, int pre_len)
{
    register int i = 0;
    for (i = 0; i < pre_len; i++)
        if ((lsi->directions)[i] != prefix[i])
            return 0;
    return 1;
}

/*
    Creates a copy of a switch element without the specified prefix.

    LampSwitchItem *lsi: Switch element to copy;
    unsigned char *prefix: Directions of the prefix;
    int pre_len: Length of the prefix array.
*/
LampSwitchItem *copy_without_prefix(LampSwitchItem *lsi, unsigned char *prefix, int pre_len)
{
    register int i = 0;
    LampSwitchItem *result = malloc(sizeof(LampSwitchItem));
    result->value = lsi->value;
    result->dir_arr_len = (lsi->dir_arr_len) - pre_len;
    result->directions = malloc((result->dir_arr_len) * sizeof(unsigned char));
    for (i = pre_len; i < (lsi->dir_arr_len); i++)
        (result->directions)[i-pre_len] = (lsi->directions)[i];
    return result;
}

/*
    Gets a switch or a lamp from a switch.

    LampSwitch *lswitch: Switch to get item from;
    unsigned char *directions_raw: Directions in numeric format, with length as first item.
*/
LampSwitch *get_switch_element(LampSwitch *lswitch, unsigned char *directions_raw)
{
    register int i = 0;
    LampSwitchItem *current = NULL;
    LampSwitch *result = malloc(sizeof(LampSwitch));
    int dir_len = directions_raw[0];
    unsigned char *directions = directions_raw + 1;
    result->item_arr_len = 0;
    result->name = NULL;
    result->item_arr = NULL;
    for (i = 0; i < lswitch->item_arr_len; i++)
    {
        current = (lswitch->item_arr)[i];
        // Case if the directions are not exact.
        if (dir_len < current->dir_arr_len)
        {
            if (check_direction_prefix(current,directions,dir_len))
            {
                append_to_switch(result,copy_without_prefix(current,directions,dir_len));
            }
        }
        // Case if the directions are exact.
        else if (current->dir_arr_len == dir_len)
        {
            if (check_direction_prefix(current,directions,dir_len))
            {
                append_to_switch(result,current);
                return result;
            }
        }
        // Case if directions are greater than current (ignore).
        // else { }
    }   
    return result;
}

/*
    Returns a name without directions.

    char *name: Name to trim.
*/
char *switch_name(char *name)
{
    register int length = 0;
    char *temp_name = NULL;
    temp_name = duplicate_string(name);
    *(strchr(temp_name,'.')) = '\0';
    return temp_name;
}

/*
    Displays a switch.

    LampSwitch *lswitch: Switch to display;
    unsigned char block: If printing as block.
*/
void display_switch(LampSwitch *lswitch, unsigned char block)
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
                if (!block)
                    printf("(");
        if (current->value == OFF)
            if (block)
                printf(" ");
            else
                printf("off");
        else
            if (block)
                printf("█");
            else
                printf("on");
        if (last_direction == ON)
            for (j = 0; j < amount; j++)
                if (!block)
                    printf(")");
        if (!block)
            printf(" ");
    }
    printf("\n");
}

/*
    Deletes a switch and frees the allocated memory.

    LampSwitch *lswitch: Switch to delete.
*/
void delete_switch(LampSwitch *lswitch)
{
    register int i = 0;
    LampSwitchItem *current = NULL;
    free(lswitch->name);
    for (i = 0; i < (lswitch->item_arr_len); i++)
    {
        current = (lswitch->item_arr)[i];
        free(current->directions);
        free(current);
    }
    free(lswitch);
}

/*
    Inverts on and off values in the switch.

    LampSwitch *lswitch: Switch to invert.
*/
void invert_switch(LampSwitch *lswitch)
{
    register int i = 0;
    for (i = 0; i < lswitch->item_arr_len; i++)
    {
        lswitch->item_arr[i]->value = !(lswitch->item_arr[i]->value);
    }
}

/*
    Checks if the given switch is in fact a lamp.

    LampSwitch *lswitch: Switch to check.
*/
int is_lamp(LampSwitch *lswitch)
{
    if (lswitch == NULL)
        return 0;
    return (lswitch->item_arr_len == 1);
}

/*
    Returns a pointer to a LampSwitch with the same content as a given argument LampSwitch.

    LampSwitch *lswitch: Switch to duplicate;
    char *name: Name of the copy.
*/
LampSwitch *duplicate_switch(LampSwitch *lswitch, char *name)
{
    register int i = 0;
    register int j = 0;
    LampSwitch *result = (LampSwitch*) malloc(sizeof(LampSwitch));
    result->name = name;
    for (i = 0; i < lswitch->item_arr_len; i++)
        append_to_switch(result,copy_without_prefix((lswitch->item_arr)[i],NULL,0));
    return result;
}

/*
    Creates a new LampSwitchItem.

    unsigned char *directions: Directions of the element;
    int dir_len: Length of the direction array;
    unsigned char value: Value of the item.
*/
LampSwitchItem *new_switch_item(unsigned char *directions, int dir_len, unsigned char value)
{
    LampSwitchItem *item;
    item = (LampSwitchItem*) malloc(sizeof(LampSwitchItem));
    item->dir_arr_len = dir_len;
    item->directions = directions;
    item->value = value;
    return item;
}

/*
    Adds an off suffix to the direction.
    Example:
    on.off -> on.on.off
    off -> on.off
    on -> on.off

    unsigned char *directions: Directions to append the off suffix to
*/
unsigned char *add_direction_off_suffix(unsigned char *directions, int *dir_len)
{
    register int i = 0;
    directions = malloc(((*dir_len) + 1) * sizeof(unsigned char));
    for(i = 0; i < (*dir_len); i++)
        directions[i] = ON;
    directions[(*dir_len)] = OFF;
    (*dir_len)++;
    return directions;
}
