#include "namespaces.h"

/*
    Checks if the naming string has any namespace indicator.

    char *name: Name string to check.
*/
int has_namespace(char *name)
{
    register int i = 0;
    while (name[i] != '\0' && name[i] != EOF && i < MAX_VARNAME_LEN)
    {
        if (name[i] == ':')
            return 1;
        i++;
    }
    return 0;
}

/*
    Adds "lamp:" in front of lamp name.

    char *name: Name of lamp to add namespace to. It will be added directly to this variable;
    char *state_lamp_namespace: Default namespace to add to lamp.
*/
char *add_default_lamp_namespace(char *name, char *state_lamp_namespace)
{
    register int i = 0;
    char *aux_pointer = NULL;
    char *new_name = malloc((strlen(state_lamp_namespace) + strlen(name)) * sizeof(char));
    strcpy(new_name,state_lamp_namespace);
    aux_pointer = new_name + strlen(state_lamp_namespace);
    strcpy(aux_pointer,name);
    free(name);
    name = new_name;
    return name;
}

/*
    Adds the switch namespace in front of lamp name.

    char *name: Name of switch to add namespace to. It will be added directly to this variable;
    char *state_switch_namespace: Default namespace to add to lamp.
*/
char *add_default_switch_namespace(char *name, char *state_switch_namespace)
{
    register int i = 0;
    char *aux_pointer = NULL;
    char *new_name = malloc((strlen(state_switch_namespace) + strlen(name)) * sizeof(char));
    strcpy(new_name,state_switch_namespace);
    aux_pointer = new_name + strlen(state_switch_namespace);
    strcpy(aux_pointer,name);
    free(name);
    name = new_name;
    return name;
}