#ifndef STORAGE_H
#define STORAGE_H

#include "../constants/constants.h"
#include "../types/lamp.h"
#include "../types/switch.h"
#include "../utils/utils.h"

#define NO_TYPE_CHECK 0 // No type check was found.
#define LAMP_TYPE_CHECK 1 // "lamp?" type check was found.
#define SWITCH_TYPE_CHECK 2 // "switch?" type check was found.

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

/*
    Sets file cursor to a specific circuit.

    Storage *storage: Storage to retrieve the reference from;
    FILE *source: File to search for circuit reference;
    char *name: Name of the circuit.
*/
int call_circuit(Storage *storage, FILE *source, char *name);

/*
    Gets the lamp/switch by its name or processes a type_check.
    Returns 1 if there was a type check, returns 0 otherwise.

    Storage *storage: Storage to get from;
    char *word: Name to search by or check token; 
    Lamp **lamp: Pointer to assign lamp to;
    LampSwitch **lswitch: Pointer to assign switch to;
    FILE *source: Source to retrieve next word for if word is a type check.
*/
int get_var_or_type_check(Storage *storage, char *word, Lamp **lamp, LampSwitch **lswitch, FILE *source);

/*
    Gets the lamp/switch by its name. Useful to get value.

    Storage *storage: Storage to get from;
    char *var_name: Name to search by; 
    Lamp **lamp: Pointer to assign lamp to;
    LampSwitch **lswitch: Pointer to assign switch to.
*/
void get_var_by_name(Storage *storage, char *var_name, Lamp **lamp, LampSwitch **lswitch);

#endif