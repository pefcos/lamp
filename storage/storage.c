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
        (storage->lamps)[0].content = NULL;
        (storage->lamps)[0].next = NULL;
    }
    for (i = 0; i < HASHTABLE_LEN; i++)
    {
        (storage->switches)[0].content = NULL;
        (storage->switches)[0].next = NULL;
    }
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
    L_HC *cell = &((storage->lamps)[calc_hash(lamp->name)]);
    L_HC *new_end = NULL;
    while (cell->content != NULL)
    {
        if (!strcmp(cell->content->name,lamp->name))
            return 0; // Error for already existing name within storage.
        if (cell->next != NULL)
            cell = cell->next;
        else
        {
            // Really bad code, fast bugfix, to fix requires changing storage struct definition to array of pointers.
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
    Retrieves a lamp in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved.
*/
Lamp *get_lamp(Storage *storage, char *name)
{
    L_HC *cell = &((storage->lamps)[calc_hash(name)]);
    while (cell->content != NULL)
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
    S_HC *cell = &((storage->switches)[calc_hash(name)]);
    while (cell->content != NULL)
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