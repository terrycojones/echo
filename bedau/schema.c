#include "types.h"

BOOLEAN
legal_schema(schema)
STRING schema;
{
    if (!schema || !strlen(schema)){
	return FALSE;
    }

    return TRUE;
}
