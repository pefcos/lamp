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

#define ERROR -1
#define OFF 0
#define ON 1