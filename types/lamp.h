#include "../constants/constants.h"

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

    Lamp *lamp: Lamp to display.
*/
void display_lamp(Lamp *lamp);

