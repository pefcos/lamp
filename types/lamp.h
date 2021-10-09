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
    Creates a lamp and allocates the necessary memory.
    Returns a lamp ponter.

    char *name: Name of the lamp;
    unsigned char initial_value: Value of the lamp, can be 1 or 0.
*/
Lamp *create_lamp(char *name, unsigned char initial_value);

/*
    Prints a lamp's value.

    Lamp *lamp: Lamp to display;
    unsigned char block: If printing as block.
*/
void display_lamp(Lamp *lamp, unsigned char block);

/*
    Destroys a lamp and frees the memory allocated to it.

    Lamp *lamp: Lamp to destroy;
*/
void delete_lamp(Lamp *lamp);