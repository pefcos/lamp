#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Length of the hash table that stores lamps and switches. Prime number large enough to store plenty of stuff without collisions.
#define HASHTABLE_LEN 601
// Maximum name length for variable. Ex: swi
#define MAX_VARNAME_LEN 255
// Maximum name length for word. Ex. swi.on.on.off.off.on.on.off.on.off.off
#define MAX_WORD_LEN 655
// Maximum number of accesses in a word (.on and .off)
#define MAX_ACCESSES 400

#define ERROR 255
#define OFF 0
#define ON 1

// Exceptions and end codes:
#define NOT_END 0
#define END 1
#define ERROR_NO_VAR_FOUND 43
#define ERROR_UNKNOWN_TYPE 45
#define ERROR_UNGROUNDED_CIRCUIT 51
#define ERROR_INVALID_SWITCH_COMPONENT 55
#define ERROR_UNKNOWN_WORD 57
#define ERROR_UNKNOWN_CIRCUIT 59
