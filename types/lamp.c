#include "lamp.h"

/*
    Creates a lamp and allocates the necessary memory.
    Returns a lamp ponter.

    char *name: Name of the lamp;
    unsigned char initial_value: Value of the lamp, can be 1 or 0.
*/
Lamp *create_lamp(char *name, unsigned char initial_value)
{
    name = duplicate_string(name);
    if (!has_namespace(name))
        name = add_default_lamp_namespace(name);
    if (name != NULL && !validate_name(name))
    {
        free(name);
        return NULL;
    }
    Lamp *pointer = (Lamp*) malloc(sizeof(Lamp));
    pointer->name = (char*) malloc(sizeof(*name));
    if (name != NULL)
        strcpy(pointer->name,name);
    else
        pointer->name = NULL;
    pointer->value = initial_value;
    free(name);
    return pointer;
}

/*
    Prints a lamp's value.

    Lamp *lamp: Lamp to display;
    unsigned char block: If printing as block.
*/
void display_lamp(Lamp *lamp, unsigned char block)
{
    if (!block)
        if (lamp->value)
            printf("on\n");
        else
            printf("off\n");
    else
        if (lamp->value)
            printf("█\n");
        else
            printf(" \n");
}

/*
    Destroys a lamp and frees the memory allocated to it.

    Lamp *lamp: Lamp to destroy;
*/
void delete_lamp(Lamp *lamp)
{
    free(lamp->name);
    free(lamp);
}