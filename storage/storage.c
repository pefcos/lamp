#include "storage.h"

/*
    Creates a storage and returns the pointer to it.
*/
Storage *create_storage()
{
    Storage *storage = (Storage*) malloc(sizeof(Storage));
    register int i = 0;
    for (i = 0; i < HASHTABLE_LEN; i++)
    {
        (storage->lamps)[i] = NULL;
        (storage->lamps)[i] = NULL;
    }
    for (i = 0; i < HASHTABLE_LEN; i++)
    {
        (storage->switches)[i] = NULL;
        (storage->switches)[i] = NULL;
    }
    storage->circ_refs = (CircRefs*) malloc(sizeof(CircRefs));
    storage->circ_refs->length = 0;
    storage->circ_refs->names = NULL;
    storage->circ_refs->offsets = NULL;
    return storage;
}

/*
    Stores a lamp in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    Lamp *lamp: Lamp to be saved.
*/
int store_lamp(Storage *storage, Lamp *lamp)
{
    L_HC *cell = (storage->lamps)[calc_hash(lamp->name)];
    L_HC *new_end = NULL;
    if (cell == NULL)
    {
        // Bad code, too lazy to fix.
        new_end = (L_HC*) malloc(sizeof(L_HC));
        new_end->content = lamp;
        new_end->next = NULL;
        (storage->lamps)[calc_hash(lamp->name)] = new_end;
        return 1;
    }
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,lamp->name))
            return 0; // Error for already existing name within storage.
        if (cell->next != NULL)
            cell = cell->next;
        else
        {
            // Bad code, too lazy to fix.
            new_end = (L_HC*) malloc(sizeof(L_HC));
            new_end->content = lamp;
            new_end->next = NULL;
            cell->next = new_end;
            return 1;
        }
    }
    cell->content = lamp;
    cell->next = NULL; 
    return 1;
}

/*
    Stores a switch in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    LampSwitch *lswitch: Lamp to be saved.
*/
int store_switch(Storage *storage, LampSwitch *lswitch)
{
    S_HC *cell = (storage->switches)[calc_hash(lswitch->name)];
    S_HC *new_end = NULL;
    if (cell == NULL)
    {
        // Bad code, too lazy to fix.
        new_end = (S_HC*) malloc(sizeof(S_HC));
        new_end->content = lswitch;
        new_end->next = NULL;
        (storage->switches)[calc_hash(lswitch->name)] = new_end;
        return 1;
    }
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,lswitch->name))
            return 0; // Error for already existing name within storage.
        if (cell->next != NULL)
            cell = cell->next;
        else
        {
            // Bad code, too lazy to fix.
            new_end = (S_HC*) malloc(sizeof(S_HC));
            new_end->content = lswitch;
            new_end->next = NULL;
            cell->next = new_end;
            return 1;
        }
    }
    cell->content = lswitch;
    cell->next = NULL; 
    return 1;
}

/*
    Retrieves a lamp in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved.
*/
Lamp *get_lamp(Storage *storage, char *name)
{
    L_HC *cell = (storage->lamps)[calc_hash(name)];
    while (cell != NULL && cell->content != NULL)
    {
        if (!strcmp(cell->content->name,name))
            return cell->content;
        if (cell->next != NULL)
            cell = cell->next;
        else
            return NULL;
    }
    return NULL;
}

/*
    Retrieves a switch in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the switch to be retrieved.
*/
LampSwitch *get_switch(Storage *storage, char *name)
{
    S_HC *cell = (storage->switches)[calc_hash(name)];
    while (cell != NULL && cell->content != NULL)
    {
        if (!strcmp(cell->content->name,name))
            return cell->content;
        if (cell->next != NULL)
            cell = cell->next;
        else
            return NULL;
    }
    return NULL;
}

/*
    Assigns value to the lamp with given name.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved;
    int value: Value to be assigned.
*/
int assign_to_lamp(Storage *storage, char *name, unsigned char value)
{
    Lamp *lamp = get_lamp(storage,name);
    if (lamp == NULL)
        return 0;
    lamp->value = value;
    return 1;
}

/*
    Removes a lamp with given name from the given storage.

    Storage *storage: Storage to remove the lamp from;
    char *name: Name of the lamp to remove.
*/
int remove_storage_lamp(Storage *storage, char *name)
{
    register int deleted = 0;
    L_HC *cell = (storage->lamps)[calc_hash(name)];
    if (cell == NULL)
        return 0; // Not found
    if (!strcmp(cell->content->name,name))
    {
        delete_lamp(cell->content);
        cell->content = NULL;
        cell->next = NULL;
        deleted = 1;
    }
    if (cell != NULL)
    {    
        while (cell->next != NULL)
        {
            if (!strcmp(cell->next->content->name,name))
            {
                delete_lamp(cell->next->content);
                cell->next = cell->next->next;
                deleted = 1;
            }
            cell = cell->next;
        }
    }
    return deleted; // Not found if deleted == 0.
}

/*
    Removes a switch with given name from the given storage.

    Storage *storage: Storage to remove the switch from;
    char *name: Name of the switch to remove.
*/
int remove_storage_switch(Storage *storage, char *name)
{
    register int deleted = 0;
    S_HC *cell = (storage->switches)[calc_hash(name)];
    if (cell == NULL)
        return 0; // Not found
    if (!strcmp(cell->content->name,name))
    {
        delete_switch(cell->content);
        cell->content = NULL;
        cell->next = NULL;
        deleted = 1;
    }
    if (cell != NULL)
    {    
        while (cell->next != NULL)
        {
            if (!strcmp(cell->next->content->name,name))
            {
                delete_switch(cell->next->content);
                cell->next = cell->next->next;
                deleted = 1;
            }
            cell = cell->next;
        }
    }
    return deleted; // Not found if deleted == 0.
}

/*
    Gets all circuit references in source file.

    Storage *storage: Storage to store the references in;
    FILE *source: File to search for circuit references.
*/
void get_circ_refs(Storage *storage, FILE *source)
{
    register int i = 0;
    char *word = NULL;
    long int *new_offsets = NULL;
    char **new_names = NULL;
    while (1)
    {
        // Loops until it finds a circuit reference.
        do {
            if (word != NULL)
                free(word);
            word = get_word(source);
            if (word == NULL)
                return; // Breaks the loop on EOF.
        } while (strcmp(word,"circuit") != 0);
        // Initializes new arays.
        new_offsets = malloc((storage->circ_refs->length + 1) * sizeof(long int));
        new_names = malloc((storage->circ_refs->length + 1) * sizeof(char*));
        // Copy contents to new array.
        for (i = 0; i < storage->circ_refs->length; i++)
        {
            new_offsets[i] = (storage->circ_refs->offsets)[i];
            new_names[i] = (storage->circ_refs->names)[i];
        }
        word = get_word(source);
        new_names[i] = word;
        word = NULL;
        new_offsets[i] = ftell(source);
        if ((storage->circ_refs->offsets) != NULL)
            free(storage->circ_refs->offsets);
        if ((storage->circ_refs->names) != NULL)
            free(storage->circ_refs->names);
        (storage->circ_refs->length)++;
        storage->circ_refs->offsets = new_offsets;
        storage->circ_refs->names = new_names;
    }
}

/*
    Sets file cursor to a specific circuit.

    Storage *storage: Storage to retrieve the reference from;
    FILE *source: File to search for circuit reference;
    char *name: Name of the circuit.
*/
int call_circuit(Storage *storage, FILE *source, char *name)
{
    long int circ_offset = 0;
    register int i = 0;
    while( i < (storage->circ_refs->length) && strcmp((storage->circ_refs->names)[i],name) != 0)
        i++;
    if (i != (storage->circ_refs->length))
    {
        circ_offset = (storage->circ_refs->offsets)[i];
        fseek(source,circ_offset,SEEK_SET);
        return 1;
    }
    return 0;
}

/*
    Gets the lamp/switch by its name. Useful to get value.
    If both lamp and lswitch are NULL, var does not exist.

    Storage *storage: Storage to get from;
    char *name: Name to search by; 
    Lamp **lamp: Pointer to assign lamp to;
    LampSwitch **lswitch: Pointer to assign switch to.
*/
void get_var_by_name(Storage *storage, char *name, Lamp **lamp, LampSwitch **lswitch)
{
    char *new_name = NULL;
    Lamp *temp_lamp = NULL;
    LampSwitch *temp_lswitch = NULL;
    *lamp = NULL;
    *lswitch = NULL;
    new_name = duplicate_string(name);
    if (!has_namespace(new_name))
        new_name = add_default_lamp_namespace(new_name);
    temp_lamp = get_lamp(storage,new_name);
    if (temp_lamp != NULL)
        *lamp = temp_lamp;
    else
    {
        free(new_name);
        new_name = duplicate_string(name);
        if (!has_namespace(new_name))
            new_name = add_default_switch_namespace(new_name);
        temp_lswitch = get_switch(storage,new_name);
        if (temp_lswitch != NULL)
            *lswitch = temp_lswitch;
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