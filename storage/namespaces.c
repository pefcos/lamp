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

    char *name: Name of lamp to add namespace to. It will be added directly to this variable.
*/
char *add_default_lamp_namespace(char *name)
{
    register int i = 0;
    char prefix[6] = "lamp:";
    char *aux_pointer = NULL;
    char *new_name = malloc((strlen(prefix) + strlen(name)) * sizeof(char));
    strcpy(new_name,prefix);
    aux_pointer = new_name + strlen(prefix);
    strcpy(aux_pointer,name);
    free(name);
    name = new_name;
    return name;
}