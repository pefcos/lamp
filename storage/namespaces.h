#include "../constants/constants.h"

/*
    Checks if the naming string has any namespace indicator.

    char *name: Name string to check.
*/
int has_namespace(char *name);

/*
    Adds "lamp:" in front of lamp name.

    char *name: Name of lamp to add namespace to. It will be added directly to this variable.
*/
char *add_default_lamp_namespace(char *name);

/*
    Adds "switch:" in front of lamp name.

    char *name: Name of switch to add namespace to. It will be added directly to this variable.
*/
char *add_default_switch_namespace(char *name);