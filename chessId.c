#include "./mtm_map/map.h"
#include "./chessReturnsMacros.h"
#include "./chessId.h"
#include <stdio.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------

                    Header included functions defenitions

------------------------------------------------------------------------*/
 MapKeyElement copyIdKey(MapKeyElement id_key)
{
    MapKeyElement new_key = malloc(sizeof(int));
    CHECK_NULL_RETURN(new_key);
    *(int *)new_key = *(int *)id_key;
    return new_key;
}

 int compareIdKeys(MapKeyElement id1, MapKeyElement id2)
{
    return *(int *)id1 - *(int *)id2;
}

 void freeIdKey(MapKeyElement id_key)
{
    free(id_key);
}