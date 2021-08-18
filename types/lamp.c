#include "lamp.h"

/*
    Creates a lamp and allocates the necessary memory.
    Returns a lamp ponter.

    char *name: Name of the lamp;
    unsigned char initial_value: Value of the lamp, can be 1 or 0.
*/
Lamp *create_lamp(char *name, unsigned char initial_value) 
{
    if (!has_namespace(name))
        name = add_default_lamp_namespace(name);
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
    Destroys a lamp and frees the memory allocated to it.

    Lamp *lamp: Lamp to destroy;
*/
void delete_lamp(Lamp *lamp)
{
    free(lamp->name);
    free(lamp);
}