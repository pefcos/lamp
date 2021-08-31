#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "../constants/constants.h"
#endif
#ifndef TYPE_LAMP_H
#define TYPE_LAMP_H
#include "../types/lamp.h"
#endif
#ifndef TYPE_SWITCH_H
#define TYPE_SWITCH_H
#include "../types/switch.h"
#endif
#ifndef UTILS_H
#define UTILS_H
#include "../utils/utils.h"
#endif

/*
    Struct that takes care of the storage of lamps and switches in hashtables.
*/
struct storage 
{
    L_HC *lamps[HASHTABLE_LEN];
    S_HC *switches[HASHTABLE_LEN];
    CircRefs *circ_refs;
};

typedef struct storage Storage;

/*
    Struct that stores circuit references in an array of fseek offsets from SEEK_SET.
*/
struct circ_refs
{
    int length;
    long int *offsets;
    char **names;
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
    Stores a switch in the  given storage.
    Returns 1 if is able to store, return 0 if error.

    Storage *storage: Storage to save in;
    LampSwitch *lswitch: Lamp to be saved.
*/
int store_switch(Storage *storage, LampSwitch *lswitch);

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

/*
    Removes a lamp with given name from the given storage.

    Storage *storage: Storage to remove the lamp from;
    char *name: Name of the lamp to remove.
*/
int remove_storage_lamp(Storage *storage, char *name);

/*
    Removes a switch with given name from the given storage.

    Storage *storage: Storage to remove the switch from;
    char *name: Name of the switch to remove.
*/
int remove_storage_switch(Storage *storage, char *name);

/*
    Gets all circuit references in source file.

    Storage *storage: Storage to store the references in;
    FILE *source: File to search for circuit references.
*/
void get_circ_refs(Storage *storage, FILE *source);