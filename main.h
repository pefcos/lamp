/*
    Initial concept by: Arthur Allebrandt Werlang.
    Final design and interpreter implementation: Pedro Fronchetti Costa da Silva.

    Lamp interpreter.
*/
// Includes lamp type.
#ifndef TYPE_LAMP_H
#define TYPE_LAMP_H
#include "./types/lamp.h"
#endif
// Includes switch type.
#ifndef TYPE_SWITCH_H
#define TYPE_SWITCH_H
#include "./types/switch.h"
#endif
// Includes storage.
#ifndef STORAGE_H
#define STORAGE_H
#include "./storage/storage.h"
#endif

/*
    Interprets the lamp code in the given file.

    FILE *source: File to draw code from;
    Storage *storage: Storage to store values in.
*/
int interpret(FILE *source, Storage *storage);