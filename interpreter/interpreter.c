#include "interpreter.h"

/*
    Initializes an interpreter state.
*/
IState *istate_init(FILE *source)
{
    IState *istate = (IState*) malloc(sizeof(IState));
    istate->word = NULL;
    istate->name = NULL; // Variable for names.
    istate->depth = 0; // Used in circuit ignoring.
    istate->value = OFF;
    istate->lamp_ptr = NULL;
    istate->lamp_ptr_ref = NULL;
    istate->lswitch_ptr = NULL;
    istate->lswitch_ptr_ref = NULL;
    istate->stack = NULL;
    istate->storage = create_storage();
    istate->source = source;
    istate->execution_end = NOT_END;
    // Get circuit references.
    get_circ_refs(istate->storage,source);
    fseek(source,0,SEEK_SET);
    istate->source = source;
    return istate;
}

/*
    Lamp declaration/assignment.
*/
IState *lamp_declaration_assignment(IState *istate)
{
    free(istate->word);
    istate->word = get_word(istate->source);
    istate->name = istate->word;
    if (!has_namespace(istate->name))
        istate->name = add_default_lamp_namespace(istate->name); 
    istate->word = get_word(istate->source);
    if (!strcmp(istate->word,"on") || !strcmp(istate->word,"off")) // Tries to get on/off literal values.
        istate->value = get_value(istate->word);
    else // Tries to get reference to other variable.
    {
        get_var_by_name(istate->storage, istate->word, &(istate->lamp_ptr_ref), &(istate->lswitch_ptr_ref));
        if (istate->lamp_ptr_ref != NULL)
            istate->value = istate->lamp_ptr_ref->value;
        else
        {
            istate->execution_end = EXCEPTION_NO_VAR_FOUND;
            return istate; // Error no variable found.
        }
    }
    istate->lamp_ptr = get_lamp(istate->storage,istate->name);
    if (istate->lamp_ptr == NULL)
    {
        if (istate->debug)
            printf("Created lamp %s with value %d.\n",istate->name,(int) istate->value);
        istate->lamp_ptr = create_lamp(istate->name, istate->value);
        store_lamp(istate->storage,istate->lamp_ptr);
    }
    else
    {
        if (istate->debug)
            printf("Assigned value %d to lamp %s.\n",(int) istate->value, istate->name);
        istate->lamp_ptr->value = istate->value;
        free(istate->name);
    }
    istate->name = NULL;
    free(istate->word);
    istate->word = NULL;
    return istate;
}

/*
    Makes a switch based on words from file.
    The order of inserting is: recalculate directions, append to array in dir position (IF DIR != NULL).

    IState *istate: Interpreter state.
*/
LampSwitch *make_switch(IState *istate)
{
    register int i = 0;
    register int j = 0;
    FILE *source = istate->source;
    char *name = istate->name;
    char *word = NULL;
    int open = 0;
    int close = 0;
    int cdir_len = 0; // Length of current directions array.
    int aux_len = 0; // Used in next_directions function as length of aux_directions.
    unsigned char *current_directions = NULL; //Stores current directions. This works as a cursor to situate the next information that should be recieved.
    unsigned char *aux_directions = NULL; // Helps expanding the original array.
    LampSwitch *result = NULL; // No lamp switch to start.
    LampSwitchItem *to_add = NULL; // Item to add to switch.
    // Initializes variables
    result = (LampSwitch*) malloc(sizeof(LampSwitch));
    result->item_arr = NULL;
    result->item_arr_len = 0;
    if (!has_namespace(name))
        name = add_default_switch_namespace(name);
    if (name != NULL && !validate_name(name))
        return NULL;
    result->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(result->name,name);
    word = get_word(source);
    if (word == NULL)
        return NULL; // Error for invalid switch due to never reaching the NULL condition on current_directions.
    else if(get_value(word) == ERROR)
    {
        get_var_by_name(istate->storage, word, &(istate->lamp_ptr_ref), &(istate->lswitch_ptr_ref));
        if (istate->lswitch_ptr_ref != NULL)
        {
            free(istate->lswitch_ptr_ref->name);
            istate->lswitch_ptr_ref->name = duplicate_string(name);
            return istate->lswitch_ptr_ref;
        }
        istate->execution_end = EXCEPTION_NO_VAR_FOUND;
        return NULL; 
    }
    do
    {
        open = count_open(word);
        close = count_close(word);
        if (open > 0 && close > 0)
            return NULL;  // Error for invalid switch item, opening and closing in the same item.
        for (i = 0; i < open; i++)
        {
            aux_directions = malloc((cdir_len + 1) * sizeof(unsigned char));
            for (j = 0; j < cdir_len; j++)
                aux_directions[j] = current_directions[j]; // Copies array elements to new array.
            free(current_directions);
            current_directions = aux_directions;
            aux_directions = NULL;
            // Insert element at end of direction array. cdir_len has not been incremented yet.
            current_directions[cdir_len] = OFF;
            cdir_len++;
        }
        to_add = (LampSwitchItem*) malloc(sizeof(LampSwitchItem));
        to_add->dir_arr_len = cdir_len;
        to_add->directions = current_directions;
        to_add->value = get_value(word);  
        append_to_switch(result,to_add);  
        to_add = NULL;                        
        //Test end condition and update directions.
        aux_directions = next_directions(current_directions,cdir_len,&aux_len);
        current_directions = aux_directions;
        aux_directions = NULL;
        cdir_len = aux_len; 
        if (word != NULL)
            free(word); // Frees previous iteration word.
        if (cdir_len != 0)
        {
            word = get_word(source);
            if (word == NULL)
                return NULL; // Error for invalid switch due to never reaching the NULL condition on current_directions.
        }     
    } while (cdir_len != 0);
    return result;
}

/*
    Switch declaration/assignment.
*/
IState *switch_declaration_assignment(IState *istate)
{
    free(istate->word);
    istate->word = get_word(istate->source);
    istate->name = istate->word;
    if (!has_namespace(istate->name))
        istate->name = add_default_switch_namespace(istate->name);
    istate->word = NULL;
    istate->lswitch_ptr = get_switch(istate->storage,istate->name);
    if (istate->lswitch_ptr == NULL)
    {
        istate->lswitch_ptr = make_switch(istate);
        if (istate->debug)
            printf("Created switch %s.\n",istate->name);
        store_switch(istate->storage,istate->lswitch_ptr);
    }
    else
    {
        remove_storage_switch(istate->storage,istate->name);
        istate->lswitch_ptr = make_switch(istate);
        if (istate->debug)
            printf("Assigned to switch %s.\n",istate->name);
        store_switch(istate->storage,istate->lswitch_ptr);
    }
    istate->name = NULL;
    free(istate->word);
    istate->word = NULL;
    return istate;
}

/*
    Lamp/Switch deleting.
*/
IState *lamp_switch_delete(IState *istate)
{
    free(istate->word);
    istate->word = get_word(istate->source);
    if (!strcmp("lamp",istate->word))
    {
        free(istate->word);
        istate->word = get_word(istate->source);
        istate->name = istate->word;
        if (!has_namespace(istate->name))
            istate->name = add_default_lamp_namespace(istate->name); // Defaults to lamp
        if (get_lamp(istate->storage,istate->name) != NULL)
            remove_storage_lamp(istate->storage, istate->name);
        if (istate->debug)
            printf("Deleted lamp %s.\n",istate->name);
    }
    else if (!strcmp("switch",istate->word))
    {
        free(istate->word);
        istate->word = get_word(istate->source);
        istate->name = istate->word;
        if (!has_namespace(istate->name))
            istate->name = add_default_switch_namespace(istate->name); // Defaults to lamp
        if (get_switch(istate->storage,istate->name) != NULL)
            remove_storage_switch(istate->storage, istate->name);
        if (istate->debug)
            printf("Deleted switch %s.\n",istate->name);
    }
    else
    {
        istate->execution_end = EXCEPTION_UNKNOWN_TYPE;// EXCEPTION UNKNOWN TYPE
        return istate;
    }
    free(istate->name);
    istate->name = NULL;
    istate->word = NULL;
    return istate;
}

/*
    Lamp/Switch displaying.
*/
IState *lamp_switch_display(IState *istate)
{
    unsigned char block = 0;
    free(istate->word);
    istate->word = get_word(istate->source);
    block = !strcmp(("block"),istate->word);
    if (!strcmp("word",istate->word) || block)
    {
        free(istate->word);
        istate->word = get_word(istate->source); // Assumes its a var name.
    }
    get_var_by_name(istate->storage, istate->word, &(istate->lamp_ptr_ref), &(istate->lswitch_ptr_ref));
    if (istate->lamp_ptr_ref != NULL)
    {
        if (istate->debug)
            printf("Displaying lamp %s:\n",istate->word);
        display_lamp(istate->lamp_ptr_ref,block);
    }
    else if (istate->lswitch_ptr_ref != NULL)
    {
        if (istate->debug)
            printf("Displaying switch %s:\n",istate->word);
        display_switch(istate->lswitch_ptr_ref,block);
    }
    else
    {
        free(istate->word);
        istate->execution_end = EXCEPTION_NO_VAR_FOUND;// EXCEPTION_NO_VAR_FOUND
        return istate;
    }
    free(istate->word);
    return istate;
}

/*
    Ignores a circuit definition.
*/
IState *ignore_circuit(IState *istate)
{
    istate->depth = 1;
    while (istate->word != NULL && istate->depth > 0)
    {
        free(istate->word);
        istate->word = get_word(istate->source);
        if (!strcmp(istate->word,"circuit"))
            (istate->depth)++;
        if (!strcmp(istate->word,"ground"))
            (istate->depth)--;
    }
    if (istate->word == NULL)
    {
        istate->execution_end = EXCEPTION_UNGROUNDED_CIRCUIT; // NO ground for circuit.
        return istate;
    }
    free(istate->word);
    istate->word = NULL;
    return istate;
}

/*
    Returns circuit.
*/
IState *circuit_ground(IState *istate)
{
    if (istate->stack == NULL)
    {
        istate->execution_end = END; // End execution.
        return istate;
    }
    fseek(istate->source,stack_pop(&(istate->stack)),SEEK_SET);
    free(istate->word);
    return istate;
}

/*
    Powers circuit.
*/
IState *power_circuit(IState *istate)
{
    istate->value = OFF;
    free(istate->word);
    istate->word = get_word(istate->source);
    istate->name = istate->word;
    istate->word = get_word(istate->source); // Gets on/off value to decide if powers circuit or not.
    if (!strcmp(istate->word,"on") || !strcmp(istate->word,"off")) // Tries to get on/off literal values.
        istate->value = get_value(istate->word);
    else // Tries to get reference to other to see if turns on or off.
    {
        get_var_by_name(istate->storage, istate->word, &(istate->lamp_ptr_ref), &(istate->lswitch_ptr_ref));
        if (istate->lamp_ptr_ref != NULL)
            istate->value = istate->lamp_ptr_ref->value;
        else
        {
            istate->execution_end = EXCEPTION_NO_VAR_FOUND;
            return istate; // Error no variable found.
        }
    }
    if (istate->value == ON)
    {
        stack_push(ftell(istate->source),&(istate->stack));
        call_circuit(istate->storage,istate->source,istate->name);
    }
    free(istate->name);
    free(istate->word);
    return istate;
}

/*
    Ignores any text between two "##" delimiters.
*/
IState *ignore_comment(IState *istate)
{
    do 
    {
        free(istate->word);
        istate->word = get_word(istate->source);
    } while (strcmp("##",istate->word));
    free(istate->word);
    istate->word = NULL;
    return istate;
}

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(IState *istate)
{
    istate->word = get_word(istate->source);
    while (istate->word != NULL)
    {
        // Lamp declaration/assignment.
        if (!strcmp("lamp",istate->word))
        {
            istate = lamp_declaration_assignment(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Switch declaration/assignment.
        if (istate->word != NULL && !strcmp("switch",istate->word))
        {
            istate = switch_declaration_assignment(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Lamp/switch delete.
        if (istate->word != NULL && !strcmp("delete",istate->word))
        {
            istate = lamp_switch_delete(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Display lamp value.
        else if (istate->word != NULL && !strcmp("display",istate->word))
        {
            istate = lamp_switch_display(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Define circuit.
        else if (istate->word != NULL && !strcmp("circuit",istate->word))
        {
            istate = ignore_circuit(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // End circuit.
        else if (istate->word != NULL && !strcmp("ground",istate->word))
        {
            istate = circuit_ground(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Power circuit.
        else if (istate->word != NULL && !strcmp("power",istate->word))
        {
            istate = power_circuit(istate);
            if (istate->execution_end != NOT_END)
                return istate->execution_end;
        }
        // Comment.
        else if (istate->word != NULL && !strcmp("##",istate->word))
        {
            istate = ignore_comment(istate);
        }
        // Next word.
        istate->word = get_word(istate->source);
    }
    return END;
}