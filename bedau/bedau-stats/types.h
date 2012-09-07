/* $Header: /disks/disk1/Users/terry/s/me/bedau/RCS/types.h,v 1.1 1994/03/29 05:30:59 terry Exp terry $ */
#include <stdlib.h>

typedef enum {FALSE, TRUE} BOOLEAN;
typedef long               LONG;
typedef char               CHAR;
typedef CHAR *             STRING;
typedef int                INT;
typedef unsigned int       UNSIGNED;
typedef short              SHORT;
typedef void               VOID;
typedef float              FLOAT;
typedef double             DOUBLE;

#include "hash.h"

typedef enum {
    BOOLEAN_OPT,
    CHAR_OPT,
    STRING_OPT,
    INT_OPT,
    FLOAT_OPT,
    DOUBLE_OPT,
    LONG_OPT,
    FUNC0_OPT,
    FUNC1_OPT,
} OPTION_TYPE;

typedef struct {
    STRING name;
    STRING abbreviation;
    OPTION_TYPE type;
    BOOLEAN required;
    BOOLEAN has_lower_limit;
    DOUBLE lower_limit;
    BOOLEAN has_upper_limit;
    DOUBLE upper_limit;
    VOID *address;
    STRING explanation;
} OPTION;

typedef struct {
#if DEBUG
    STRING name;
#endif
    DOUBLE usage;
    LONG last_alive;
} HASH_DATA;

#include "public.h"
#include "globals.h"
