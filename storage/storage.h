#include "../constants/constants.h"
#include "../types/lamp.h"
#include "../types/switch.h"

/*
    Struct that takes care of the storage of lamps and switches in hashtables.
*/
struct storage 
{
    L_HC lamps[HASHTABLE_LEN];
    S_HC switches[HASHTABLE_LEN];
};

typedef struct storage Storage;

/*
    Struct that stores circuit references in an array of fseek offsets from SEEK_SET.
*/
struct circ_refs
{
    int length;
    int *offsets;
};

typedef struct circ_refs CircRefs;

/*
    Creates a storage and returns the pointer to it.
*/
Storage *create_storage();

/*
    Stores a lamp in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    Lamp *lamp: Lamp to be saved.
*/
int store_lamp(Storage *storage, Lamp *lamp);

/*
    Retrieves a lamp in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved.
*/
Lamp *get_lamp(Storage *storage, char *name);

/*
    Retrieves a switch in the given storage.
    Returns NULL if not found.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the switch to be retrieved.
*/
LampSwitch *get_switch(Storage *storage, char *name);

/*
    Assigns value to the lamp with given name.

    Storage *storage: Storage to retrieve from;
    char *name: Name of the lamp to be retrieved;
    int value: Value to be assigned.
*/
int assign_to_lamp(Storage *storage, char *name, unsigned char value);