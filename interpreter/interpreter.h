// Includes lamp type.
#ifndef TYPE_LAMP_H
#define TYPE_LAMP_H
#include "../types/lamp.h"
#endif
// Includes switch type.
#ifndef TYPE_SWITCH_H
#define TYPE_SWITCH_H
#include "../types/switch.h"
#endif
// Includes storage.
#ifndef STORAGE_H
#define STORAGE_H
#include "../storage/storage.h"
#endif
// Includes storage.
#ifndef UTILS_H
#define UTILS_H
#include "../utils/utils.h"
#endif

struct interpreter_state
{
    char *word;
    char *name; // Variable for names.
    int depth; // Used in circuit ignoring.
    unsigned char value;
    Lamp *lamp_ptr;
    Lamp *lamp_ptr_ref;
    LampSwitch *lswitch_ptr;
    LampSwitch *lswitch_ptr_ref;
    Stack *stack;
    Storage *storage;
    FILE *source;
    unsigned char debug; // Enables/disables prints explaining code.
    unsigned char execution_end; // Return type for end of program. While 0 does not return.
    // Namespaces
    char *lamp_namespace;
    char *lswitch_namespace;
};

typedef struct interpreter_state IState;

/*
    Initializes an interpreter state.
*/
IState *istate_init(FILE *source);

// Namespace functions

IState *set_lamp_namespace(IState *istate, char *namespace);

IState *set_switch_namespace(IState *istate, char *namespace);

/*
    Lamp declaration/assignment.

*/
IState *lamp_declaration_assignment(IState *istate);

/*
    Copies a switch from the referenced switch.

    IState *istate: Interpreter state.
*/
LampSwitch *assign_reference_to_switch(IState *istate);

/*
    Builds a switch in reduced format.

    IState *istate: Interpreter state.
*/
LampSwitch *reduced_switch_constructor(IState *istate);

/*
    Builds a switch using '(' and ')', together with references and values.
    
    IState *istate: Interpreter state.
*/
LampSwitch *switch_constructor(IState *istate);

/*
    Makes a switch based on words from file.

    IState *istate: Interpreter state.
*/
LampSwitch *make_switch(IState *istate);  

/*
    Switch declaration/assignment.

*/
IState *switch_declaration_assignment(IState *istate);

/*
    Lamp/Switch deleting.
*/
IState *lamp_switch_delete(IState *istate);

/*
    Lamp/Switch displaying.
*/
IState *lamp_switch_display(IState *istate);

/*
    Ignores a circuit definition.
*/
IState *ignore_circuit(IState *istate);

/*
    Ignores any text between two "##" delimiters.
*/
IState *ignore_comment(IState *istate);

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(IState *istate);