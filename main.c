/*
    Pedro Fronchetti Costa da Silva, 08/2021
    Lamp interpreter.
*/

#include "main.h"

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage)
{
    char *word = NULL;
    char *name = NULL; // Variable for names.
    unsigned char value = OFF;
    Lamp *lamp_ptr = NULL;
    LampSwitch *lswitch_ptr = NULL;
    word = get_word(source);
    while (word != NULL)
    {
        // Lamp declaration/assignment.
        if (!strcmp("lamp",word))
        {
            free(word);
            word = get_word(source);
            name = word;
            if (!has_namespace(name))
                name = add_default_lamp_namespace(name); 
            word = get_word(source);
            value = get_value(word);
            lamp_ptr = get_lamp(storage,name);
            if (lamp_ptr == NULL)
            {
                printf("Created lamp %s with value %d.\n",name,(int) value);
                lamp_ptr = create_lamp(name, value);
                store_lamp(storage,lamp_ptr);
            }
            else
            {
                printf("Assigned value %d to lamp %s.\n",(int) value, name);
                lamp_ptr->value = value;
                free(name);
            }
            name = NULL;
            free(word);
            word = NULL;
        }
        // Switch declaration/assignment.
        if (word != NULL && !strcmp("switch",word))
        {
            free(word);
            word = get_word(source);
            name = word;
            if (!has_namespace(name))
                name = add_default_switch_namespace(name); 
            word = get_word(source);
            value = get_value(word);
            lswitch_ptr = get_switch(storage,name);
            if (lswitch_ptr == NULL)
            {
                printf("Created switch %s.\n",name);
                lswitch_ptr = make_switch(source,name);
                store_switch(storage,lswitch_ptr);
            }
            else
            {
                // TODO
            }
            name = NULL;
            free(word);
            word = NULL;
        }
        // Lamp/switch delete.
        if (word != NULL && !strcmp("delete",word))
        {
            free(word);
            word = get_word(source);
            if (!strcmp("lamp",word))
            {
                free(word);
                word = get_word(source);
                name = word;
                if (!has_namespace(name))
                    name = add_default_lamp_namespace(name); // Defaults to lamp
                if (get_lamp(storage,name) != NULL)
                    remove_storage_lamp(storage, name);
            }
            else if (!strcmp("switch",word))
            {
                free(word);
                word = get_word(source);
                name = word;
                if (!has_namespace(name))
                    name = add_default_switch_namespace(name); // Defaults to lamp
                if (get_switch(storage,name) != NULL)
                    1;
            }
            else
            {
                // EXCEPTION UNKNOWN TYPE
            }
            free(name);
            name = NULL;
            word = NULL;

        }
        // Display lamp value.
        else if (word != NULL && !strcmp("display",word))
        {
            free(word);
            word = get_word(source);
            if (!strcmp("lamp",word))
            {
                free(word);
                word = get_word(source);
                name = word;
                if (!has_namespace(name))
                    name = add_default_lamp_namespace(name);
                lamp_ptr = get_lamp(storage,name);
                if (lamp_ptr == NULL) 
                {
                    return 0;
                }
                display_lamp(lamp_ptr);
            }
            else if (!strcmp("switch",word))
            {

            }
            else
            {
                //EXCEPTION INVALID TYPE
            }
        }
        // Next word.
        word = get_word(source);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    Storage *storage = create_storage();
    FILE *source = NULL;
    switch(argc)
    {
    case 1:
        printf("No file to interpret. To specify a file, call the interpreter using \'lamp <filename.lamp>\'.\n");
        break;
    
    case 2:
        source = fopen(argv[1],"r");
        if (source == NULL)
        {
            printf("Error opening file %s. Please specify the full filename with the extension and remember that uppercase and lowercase letters are different.\n",argv[1]);
            break;
        }
        if (interpret(source, storage))
            printf("Program finished successfully.\n");
        else
            printf("Program execution interrupted.\n");
        break;
    
    default:
        printf("Too many arguments! Please execute the program using \'lamp <filename.lamp>\'.\n");
    }
    
    return 0;
}