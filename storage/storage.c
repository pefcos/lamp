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
    Gets the lamp/switch by its name or processes a type_check.
    Returns 1 if there was a type check, returns 0 otherwise.

    Storage *storage: Storage to get from;
    char *word: Name to search by or check token; 
    Lamp **lamp: Pointer to assign lamp to;
    LampSwitch **lswitch: Pointer to assign switch to;
    FILE *source: Source to retrieve next word for if word is a type check.
*/
int get_var_or_type_check(Storage *storage, char *word, Lamp **lamp, LampSwitch **lswitch, FILE *source, char *lamp_namespace, char* lswitch_namespace)
{
    char *negativeless = NULL; // Trims the '-' in the start of a string if it starts with that.
    int has_negative = 0; // Uses to duplicate the right thing to store in variable negativeless.
    unsigned char return_value = OFF; // Value of the returning lamp if it is a type check.
    unsigned char type_check = NO_TYPE_CHECK; // Set on strcmp.

    if (word[0] == '-') 
        has_negative = 1;
    negativeless = duplicate_string(&(word[has_negative]));
    if(!strcmp(negativeless,"lamp?"))
        type_check = LAMP_TYPE_CHECK;
    else if(!strcmp(negativeless,"switch?"))
        type_check = SWITCH_TYPE_CHECK;
    
    free(negativeless);
    negativeless = NULL;
    if (type_check == LAMP_TYPE_CHECK || type_check == SWITCH_TYPE_CHECK)
    {
        free(word);
        word = get_word(source);
        get_var_by_name(storage, word, lamp, lswitch, lamp_namespace, lswitch_namespace);
        if ((*lamp != NULL && type_check == LAMP_TYPE_CHECK) || (*lswitch != NULL && type_check == SWITCH_TYPE_CHECK))
            return_value = ON;
        *lamp = create_lamp("typecheck",return_value, lamp_namespace);
        return 1; // There was a "lamp?" or "switch?" type check.
    }

    // No type check was made.
    get_var_by_name(storage, word, lamp, lswitch, lamp_namespace, lswitch_namespace);
    return 0;

}

/*
    Gets the lamp/switch by its name. Useful to get value.
    If both lamp and lswitch are NULL, var does not exist.

    Storage *storage: Storage to get from;
    char *var_name: Name to search by; 
    Lamp **lamp: Pointer to assign lamp to;
    LampSwitch **lswitch: Pointer to assign switch to;
    char *lamp_namespace: Namespace to search for lamps;
    char *lswitch_namespace: Namespace to search for switches.
*/
void get_var_by_name(Storage *storage, char *var_name, Lamp **lamp, LampSwitch **lswitch, char *lamp_namespace, char *lswitch_namespace)
{
    char *new_name = NULL;
    char *switch_name_no_directions = NULL; // Used to remove directions from the name. Switch element.
    char *mock_name = NULL; // Used to create lamp. Switch element.
    char *mock_namespace = NULL; // Used to create lamp. Switch element.
    Lamp *temp_lamp = NULL;
    LampSwitch *temp_lswitch = NULL;
    LampSwitch *element_temp = NULL;
    unsigned char negative = 0;
    char *name = var_name;
    *lamp = NULL;
    *lswitch = NULL;
    // Verifies if it is a negative.
    negative = (name[0] == '-');
    if (negative)
        name = &(name[1]);
    // Verifies if is lamp/switch or switch element.  TODO transfer to aux function.
    if (strchr(name,'.') != NULL)
    { // Switch element
        // Get switch.
        new_name = duplicate_string(name);
        if (!has_namespace(new_name))
            new_name = add_default_switch_namespace(new_name,lswitch_namespace);
        switch_name_no_directions = switch_name(new_name);
        element_temp = get_switch(storage,switch_name_no_directions);
        // Get the element.
        if (element_temp != NULL)
        {
            temp_lswitch = get_switch_element(element_temp,convert_to_number_directions(name));
            if (is_lamp(temp_lswitch)) // Switch element is lamp.
            {
                mock_name = (char*) malloc(sizeof(char) * 4);
                strcpy(mock_name,"def");
                if (negative)
                    *lamp = create_lamp(mock_name,!((temp_lswitch->item_arr[0])->value), lamp_namespace);
                else
                    *lamp = create_lamp(mock_name,(temp_lswitch->item_arr[0])->value, lamp_namespace);
            }
            else
            {
                if (negative)
                    invert_switch(temp_lswitch);
                *lswitch = temp_lswitch;
            }
            return;
        }
        else
            return;
    }
    // NOT a switch element.
    new_name = duplicate_string(name);
    if (!has_namespace(new_name))
        new_name = add_default_lamp_namespace(new_name,lamp_namespace);
    temp_lamp = get_lamp(storage,new_name);
    if (temp_lamp != NULL)
    {
        if (negative)
            temp_lamp->value = !(temp_lamp->value);
        *lamp = temp_lamp;
    }
    else
    {
        free(new_name);
        new_name = duplicate_string(name);
        if (!has_namespace(new_name))
            new_name = add_default_switch_namespace(new_name,lswitch_namespace);
        temp_lswitch = get_switch(storage,new_name);
        if (temp_lswitch != NULL)
        {
            if (negative)
                invert_switch(temp_lswitch);
            *lswitch = temp_lswitch;
        }
    }
}
