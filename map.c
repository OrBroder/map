#include "./map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NOT_FOUND -1
#define EMPTY_NO_SIZE -1
#define EMPTY_ARGUMENT -2

/* ----------------------------------------------------------------------

                        code structs

------------------------------------------------------------------------*/
/** Struct used as node for holding the data and key given by the user while pointing to the next entry by lexicographic order
 * @param data - pointer for a single entry data
 * @param key - pointer for a key(used for sorting the entries and calling the data back to the user)
 * @param next - pointer for the next entry(essential for the linked list implementation)
 */
typedef struct entry_t
{
    MapDataElement data;
    MapKeyElement key;
    struct entry_t* next;
} *Entry;

/** Struct used for holding the map data structure
 * @param first - pointer for the "head" of the linked list(first entry by order of keys)
 * @param iterator - pointer for an entry. used for external iteration of the map 
 * @param copyDataFnc - pointer for a function used for allocating a copy of a given data
 * @param copyKeyFnc - pointer for a function used for allocating a copy of a given key
 * @param copyDataFnc - pointer for a function used for releasing memory for a given data adress
 * @param copyKeyFnc - pointer for a function used for releasing memory for a given key adress
 * @param compareKeyFnc - pointer for a function used for comparing keys 
 */
struct Map_t
{
    Entry first;
    Entry iterator;
    copyMapDataElements copyDataFnc;
    copyMapKeyElements copyKeyFnc;
    freeMapDataElements freeDataFnc;
    freeMapKeyElements freeKeyFnc;
    compareMapKeyElements compareKeyFnc;
};

/* ----------------------------------------------------------------------

                        internal code functions headers

------------------------------------------------------------------------*/
 
 /** Type used for categorizing edge cases for functions mapPut mapRmove and directing to matching functions
  * using the aid functions
  * @param PUT_INITIALIZED - Indicates no edge cases detected yet
  * @param PUT_EMPTY - Indicates adding a new entry for an empty list(no other entries)   
  * @param PUT_REPLACE - Indicates the existance of the given key(replacement of data)
  * @param PUT_MID - Indicates that the destined location of the new entry is not at the edges of the list
  * @param PUT_FIRST - Indicates that the new entry location is at the start of the list(other entries exist)
  * @param PUT_LAST - Indicates that the new entry location is at the tail of the list(no entries after)
  * 
 */
  typedef enum PutResult_t {
    PUT_INITIALIZED,
    PUT_EMPTY,
    PUT_REPLACE,
    PUT_MID,
    PUT_FIRST,
    PUT_LAST,
} PutResult;

/**
* createNewEntry: Allocates a new Entry.
*
* @param map - Map pointer for usage of the map's functions for allocating a new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Entry with copies of the given data and key. next node defined NULL
*/
Entry createNewEntry(Map map,  MapKeyElement key,MapDataElement data);

/**
* mapPutAid(assists mapPut): adds a new entry for a map(or replaces an entry's data)
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @param back - Potential previous node of the new entry(if fits the case)
* @param front - Holds the "next"'s adress for the new entry
* @param putRes - Holds the info of the relevant edge case of mapAdd(used for directing to the relevant specific function)
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult mapPutAid(Map map, MapKeyElement key,MapDataElement data,Entry back, Entry front,PutResult putRes);

/**
* putNewEntryEmptyMap: adds an entry for an empty map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewEntryEmptyMap(Map map, MapKeyElement key,MapDataElement data);

/**
* putNewEntryFirst: adds an entry at the head of a map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewEntryFirst(Map map, MapKeyElement key,MapDataElement data);

/**
* replaceEntryData: replaces the data of a givenEntry
*
* @param map - Map pointer of the data structure for the new entry.
* Essential for allocating the new data and freeing the old one using the map's functions
* @paramEntry - Entry pointer of the targer entry(which will have his data is changed)
* @param key - Key pointer of the targer entry
* @param data - Data pointer to be copied for the replacement of the Data
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult replaceEntryData(Map map,Entry entry, MapKeyElement key,MapDataElement data);

/**
* putNewEntryMid: adds an entry at the middle of a map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @param back - Potential previous node of the new entry(if fits the case)
* @param front - Holds the "next"'s adress for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewEntryMid(Map map, MapKeyElement key,MapDataElement data,Entry back, Entry front);

/**
* destroyEntry: frees an entrie's data, key and it's own adress
*
* @param map - Map pointer of the data structure. Essential for the map's freeing functions.
* @paramEntry - Entry pointer to be deleted
* @return
* void
*/
void destroyEntry(Map map,Entry entry);

/**
* removeFirstEntry: Deletes the first entry of a map's list
*
* @param map - Map pointer of the data structure.
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult removeFirstEntry(Map map);

/**
* removeMidLastEntry: Deletes the entry next to a given one
*
* @param map - Map pointer of the data structure.
* @param back - Entry pointer of the entry ahead of the entry to be deleted
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult removeMidLastEntry(Map map,Entry back);

/**
* mapRemoveAid: Deletes an entry
*
* @param map - Map pointer of the data structure.
* @param back - Entry pointer of the entry ahead of the entry to be deleted
* @param putRes -  Holds the info of the relevant edge case of mapRemove(used for directing to the relevant specific function)
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult mapRemoveAid(Map map,Entry back,PutResult putRes);

  /**
* mapCopyList: Makes a copy of the first given map's list of entries and places it in the second
* @param originalMap - Map pointer of the data structure which will have it's list copied
* @param destinationMap - Map pointer of the data structure which will store the copy of the list*/
void mapCopyList(Map originalMap, Map destinationMap);

  

/* ----------------------------------------------------------------------

                        header-included function's defenitions

------------------------------------------------------------------------*/

Map mapCreate(copyMapDataElements copyDataFnc,
              copyMapKeyElements copyKeyFnc,
              freeMapDataElements freeDataFnc,
              freeMapKeyElements freeKeyFnc,
              compareMapKeyElements compareKeyFnc)
{
    if (copyDataFnc == NULL || copyKeyFnc == NULL || freeDataFnc == NULL || freeKeyFnc == NULL || compareKeyFnc == NULL)
    {
        return NULL;
    }
    Map newMap = (Map)malloc(sizeof(struct Map_t));
    
    if (newMap == NULL)
    {
        return NULL;
    }

    newMap->iterator = NULL;
    newMap->first =NULL;
    newMap->copyDataFnc = copyDataFnc;
    newMap->copyKeyFnc = copyKeyFnc;
    newMap->freeDataFnc = freeDataFnc;
    newMap->freeKeyFnc = freeKeyFnc;
    newMap->compareKeyFnc = compareKeyFnc;

    
    return newMap;
}

void mapDestroy(Map map)
{
    if (map == NULL)
    {
        return;
    }

   Entry current = map->first;
    
    while(current != NULL)
    {
      
       Entry toDelete = current;
        current = current->next;
        destroyEntry(map,toDelete);
    }

    free(map);
}

Map mapCopy(Map originalMap)
{
    if(originalMap == NULL)
    {
        return NULL;
    }
    Map newMap = mapCreate(originalMap->copyDataFnc, originalMap->copyKeyFnc,
    originalMap->freeDataFnc, originalMap->freeKeyFnc,originalMap->compareKeyFnc);

    mapCopyList(originalMap,newMap);
    return newMap;
}

int mapGetSize(Map map)
{
    if(map == NULL)
    {
        return EMPTY_NO_SIZE;
    }

    Entry current = map->first;
   

    int counter = 0; 
    while(current != NULL)
    {  
        counter ++;
        current = current->next;
    }
    return counter;
    
}

bool mapContains(Map map, MapKeyElement key)
{
    if(map == NULL||key == NULL)
    {
        return false;
    }

   Entry current = map->first;
    while(current != NULL)
    {
        if(map->compareKeyFnc(current->key,key) == 0)
        {
            return true;
        }
        current = current->next;
        
    }

    return false;
}

MapResult mapPut(Map map, MapKeyElement inputKey, MapDataElement data)
{
    
    if(map == NULL || inputKey == NULL||data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    PutResult putRes = PUT_INITIALIZED;

   Entry back = NULL;
   Entry front = NULL;
    if(map->first == NULL)
    {
        putRes = PUT_EMPTY;
    }

    else if(map->compareKeyFnc(map->first->key,inputKey) > 0)
    {
       putRes = PUT_FIRST;
    }
    else 
    {
         back = NULL;
         front = map->first;
        while(front != NULL && putRes == PUT_INITIALIZED)
        {
            if(map->compareKeyFnc(front->key,inputKey) == 0)
            {
                putRes = PUT_REPLACE;
            }
            else if(map->compareKeyFnc(front->key,inputKey) > 0)
            {
                putRes = PUT_MID;
            }
            else
            {
                back = front;
                front = front->next;
            }
        }
        if(putRes == PUT_INITIALIZED)
        {
            putRes = PUT_LAST;
        }

    }
  
    return mapPutAid(map,inputKey,data,back,front,putRes);
}

MapDataElement mapGet(Map map, MapKeyElement inputKey)
{
   
    if(map == NULL || inputKey == NULL)
    {
        return NULL;
    }

   Entry current = map->first;
    while(current!= NULL)
    {
       
        if(map->compareKeyFnc(current->key,inputKey) == 0)
        {
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement inputKey)
{
    
   if(map == NULL || inputKey == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    PutResult putRes = PUT_INITIALIZED;

   Entry back = NULL;
   Entry front = map->first;
    while(front != NULL && putRes == PUT_INITIALIZED)
    {   
        if(map->compareKeyFnc(front->key,inputKey) == 0)
            {
    
                front = front->next;
                if(front != NULL)
                {
                    if(back != NULL)
                    {
                        putRes = PUT_MID;
                    }
                    else
                    {
                        putRes = PUT_FIRST; 
                    }
                }
                else
                {
                    if(back != NULL)
                    {
                        putRes = PUT_LAST;
                    }
                    else
                    {
                        putRes = PUT_EMPTY;
                    }
                }
               

            }

        if(putRes == PUT_INITIALIZED)
        {
            back = front;
            front = front->next;
        }
    }  
        
  return mapRemoveAid(map,back,putRes);
}

MapKeyElement mapGetFirst(Map map)
{
    if(map == NULL || map->first ==NULL)
    {
        return NULL;
    }
    map->iterator = map->first;

    return map->copyKeyFnc(map->first->key);
}

MapKeyElement mapGetNext(Map map)
{
    if(map == NULL || map->iterator ==NULL||map->iterator->next == NULL)
    {
        return NULL;
    }

    map->iterator = map->iterator->next;
    
    return map->copyKeyFnc(map->iterator->key);
}

MapResult mapClear(Map map)
{
    if (map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

   Entry current = map->first;
    while(current != NULL)
    {
       Entry toDelete = current;
        current = current->next;
        mapRemove(map,toDelete->key);
    }

    return MAP_SUCCESS;
}

/* ----------------------------------------------------------------------

             non header-included function's defenitions(aid functions)

------------------------------------------------------------------------*/

void destroyEntry(Map map,Entry entry)
{
    if(entry != NULL)
        {
            map->freeDataFnc(entry->data);
            map->freeKeyFnc(entry->key);
            free(entry);
        }
}


MapResult mapRemoveAid(Map map,Entry back,PutResult putRes)
{
    MapResult result;
     switch (putRes)
   {
   case PUT_EMPTY:
        result =  removeFirstEntry(map);
       break;
   case PUT_FIRST:
       result =  removeFirstEntry(map);
       break;
    case PUT_MID:
        result =  removeMidLastEntry(map,back);
       break;
    case PUT_LAST:
        result = removeMidLastEntry(map,back);
       break;
    case PUT_INITIALIZED:
        result = MAP_ITEM_DOES_NOT_EXIST;
        break;
    default:
        result = MAP_NULL_ARGUMENT;
      break;
   }

    return result;
}

MapResult removeFirstEntry(Map map)
{
    if(map == NULL|| map->first == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
   Entry newFirst = map->first->next;
    destroyEntry(map,map->first);
    map->first = newFirst;
    return MAP_SUCCESS; 
}

MapResult removeMidLastEntry(Map map,Entry back)
{
    if(map == NULL|| back == NULL||back->next == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

   Entry victim = back->next;
   Entry front = victim->next;

    destroyEntry(map,victim);
    back->next = front;

    return MAP_SUCCESS; 
}

MapResult mapPutAid(Map map, MapKeyElement key,MapDataElement data,Entry back,Entry front,PutResult putRes)
{
    MapResult result;
     switch (putRes)
   {
   case PUT_EMPTY:
        result = putNewEntryEmptyMap(map,key,data);
       break;
   case PUT_FIRST:
        result = putNewEntryFirst(map,key,data);
       break;
   case PUT_REPLACE:
        result = replaceEntryData(map,front,key,data); 
       break;
    case PUT_MID:
        result = putNewEntryMid(map,key,data,back,front);
       break;
    case PUT_LAST:
        result = putNewEntryMid(map,key,data,back,NULL);
       break;
    default :
        return MAP_NULL_ARGUMENT;

       
   }

   return result;
}

 MapResult putNewEntryEmptyMap(Map map, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
   Entry newEntry = createNewEntry(map,key,data);
    if(newEntry == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    map->first = newEntry;
    return MAP_SUCCESS;
}

MapResult putNewEntryFirst(Map map, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
   Entry newEntry = createNewEntry(map,key,data);
    if(newEntry == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

   Entry current = map->first;
    map->first = newEntry;
    newEntry->next = current;
    return MAP_SUCCESS;
}

MapResult replaceEntryData(Map map,Entry entry, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL||entry == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    MapDataElement newData = map->copyDataFnc(data);
    if(newData == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    map->freeDataFnc(entry->data);
   entry->data = newData;
    return MAP_SUCCESS;
}

MapResult putNewEntryMid(Map map, MapKeyElement key,MapDataElement data,Entry back,Entry front)
{
    if(map == NULL || key == NULL || data == NULL|| back == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
   Entry newEntry = createNewEntry(map,key,data);
    if(newEntry == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    back->next = newEntry;
    newEntry->next = front;
    
    return MAP_SUCCESS;
}

Entry createNewEntry(Map map , MapKeyElement key,MapDataElement data)
{
    if(data == NULL|| key == NULL)
    {
        return NULL;
    }
   Entry newEntry = malloc(sizeof(struct entry_t));
    if(newEntry != NULL)
    {
        newEntry->data = map->copyDataFnc(data);
        if(newEntry->data == NULL)
        {
            free(newEntry);
            return NULL;   
        }

        newEntry->key = map->copyKeyFnc(key);
        if(newEntry->key == NULL)
        {
            free(newEntry->data);
            free(newEntry);
            return NULL;   
        }
        newEntry->next = NULL;
        
    }
    return newEntry;
}

void mapCopyList(Map originalMap, Map destinationMap)
{
    if(originalMap == NULL || destinationMap == NULL || originalMap->first == NULL)
    {
        return;
    }

     Entry current = originalMap->first;
    while(current != NULL)
    {   
        mapPut(destinationMap,current->key,current->data);
        current = current->next;
    }
}
















