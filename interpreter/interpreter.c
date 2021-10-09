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
        istate->lswitch_ptr = make_switch(istate->source,istate->name);
        if (istate->debug)
            printf("Created switch %s.\n",istate->name);
        store_switch(istate->storage,istate->lswitch_ptr);
    }
    else
    {
        remove_storage_switch(istate->storage,istate->name);
        istate->lswitch_ptr = make_switch(istate->source,istate->name);
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
    }
    else if (!strcmp("switch",istate->word))
    {
        free(istate->word);
        istate->word = get_word(istate->source);
        istate->name = istate->word;
        if (!has_namespace(istate->name))
            istate->name = add_default_switch_namespace(istate->name); // Defaults to lamp
        if (get_switch(istate->storage,istate->name) != NULL)
            1; // TODO remove storage switch.
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
        display_lamp(istate->lamp_ptr_ref,block);
    }
    else if (istate->lswitch_ptr_ref != NULL)
    {
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
        // Next word.
        istate->word = get_word(istate->source);
    }
    return END;
}