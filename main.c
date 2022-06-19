/*
    pefcos, 08/2021
    Lamp interpreter.
*/

#include "main.h"

int main(int argc, char *argv[])
{
    FILE *source = NULL;
    IState *istate = NULL;
    unsigned char debug = 0;
    switch(argc)
    {
    case 1:
        printf("No file to interpret. To specify a file, call the interpreter using \'lamp <filename.lamp>\'.\n");
        break;
    
    case 3:
        if (!strcmp(argv[2],"-d") || !strcmp(argv[2],"--debug"))
            debug = 1;
        else
        {
            printf("Unknown argument %s.\n",argv[2]);
            break;
        }
    case 2:
        if (!strcmp("-v",argv[1]) || !strcmp("--version",argv[1]))
        {
            printf("lampi v1.1\n");
            break;
        }
        if (!strcmp("-h",argv[1]) || !strcmp("--help",argv[1]))
        {
            printf("lampi v1.1\n");
            printf("Usage: lampi <filename> [-d]\n");
            printf("<filename>: Path or name of the file with lamp code to interpret.\n");
            printf("[-d]: Debug option.\n");
            break;
        }
        source = fopen(argv[1],"r");
        if (source == NULL)
        {
            printf("Error opening file %s. Please specify the full filename with the extension and remember that uppercase and lowercase letters are different.\n",argv[1]);
            break;
        }
        istate = istate_init(source);
        istate->debug = debug; // Sets debug.
        if (interpret(istate) == END)
        {
            if (istate->debug)
                printf("Program finished successfully.\n");
        }
        else
            print_end_message(istate->execution_end,istate->word,which_line(istate->source));
        break;

    default:
        printf("Too many arguments! Please execute the program using \'lamp <filename.lamp>\'.\n");
    }
    
    return 0;
}
