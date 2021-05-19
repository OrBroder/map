#include "./map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NOT_FOUND -1
#define EMPTY_ARGUMENT -2

/* ----------------------------------------------------------------------

                        code structs

------------------------------------------------------------------------*/
/** Struct used as node for holding the data and key given by the user
 * @param data - pointer for a single entry data
 * @param key - pointer for a key(used for the map lexicgraphical order and referencing the data back)
 * @param next - pointer for the next entry(essential for the linked list implementation)
 */
typedef struct entry
{
    MapDataElement data;
    MapKeyElement key;
    struct entry* next;
} *Element;

/** Struct used for holding the map data structure
 * @param first - pointer for the "head" of the linked list
 * @param iterator - pointer for an entry. used for external iteration of the map 
 * @param copyDataFnc - pointer for a function used for allocating a copy of a given data
 * @param copyKeyFnc - pointer for a function used for allocating a copy of a given key
 * @param copyDataFnc - pointer for a function used for releasing memory for a given data adress
 * @param copyKeyFnc - pointer for a function used for releasing memory for a given key adress
 * @param compareKeyFnc - pointer for a function used for comparing keys 
 */
struct Map_t
{
    Element first;
    Element iterator;
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
  typedef enum PutCase_t {
    PUT_INITIALIZED,
    PUT_EMPTY,
    PUT_REPLACE,
    PUT_MID,
    PUT_FIRST,
    PUT_LAST,
} PutCase;

/**
* createNewElement: Allocates a new Element.
*
* @param map - Map pointer for usage of the map's functions for allocating a new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Element with copies of the given data and key. next node defined NULL
*/
Element createNewElement(Map map,  MapKeyElement key,MapDataElement data);

/**
* mapPutAid(assists mapPut): adds a new entry for a map(or replaces an entry's data)
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @param back - Potential previous node of the new entry(if fits the case)
* @param front - Holds the "next"'s adress for the new entry
* @param putC - Holds the info of the relevant edge case of mapAdd(used for directing to the relevant specific function)
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult mapPutAid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front,PutCase putC);

/**
* putNewBlankFirst: adds an entry for an empty map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewBlankFirst(Map map, MapKeyElement key,MapDataElement data);

/**
* putNewFirst: adds an entry at the head of a map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewFirst(Map map, MapKeyElement key,MapDataElement data);

/**
* replaceElementData: replaces the data of a given element
*
* @param map - Map pointer of the data structure for the new entry.
* Essential for allocating the new data and freeing the old one using the map's functions
* @param element - Entry pointer of the targer entry(which will have his data is changed)
* @param key - Key pointer of the targer entry
* @param data - Data pointer to be copied for the replacement of the Data
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult replaceElementData(Map map,Element element, MapKeyElement key,MapDataElement data);

/**
* putNewMid: adds an entry at the middle of a map's list
*
* @param map - Map pointer of the data structure for the new entry
* @param key - Key pointer to be copied for the new entry
* @param data - Data pointer to be copied for the new entry
* @param back - Potential previous node of the new entry(if fits the case)
* @param front - Holds the "next"'s adress for the new entry
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult putNewMid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front);

/**
* destroyElement: frees an entrie's data, key and it's own adress
*
* @param map - Map pointer of the data structure. Essential for the map's freeing functions.
* @param element - Entry pointer to be deleted
* @return
* void
*/
void destroyElement(Map map,Element element);

/**
* removeFirstElement: Deletes the first entry of a map's list
*
* @param map - Map pointer of the data structure.
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult removeFirstElement(Map map);

/**
* removeMidElement: Deletes the entry next to a given one
*
* @param map - Map pointer of the data structure.
* @param back - Entry pointer of the entry ahead of the entry to be deleted
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult removeMidElement(Map map,Element back);

/**
* mapRemoveAid: Deletes an entry
*
* @param map - Map pointer of the data structure.
* @param back - Entry pointer of the entry ahead of the entry to be deleted
* @param putC -  Holds the info of the relevant edge case of mapRemove(used for directing to the relevant specific function)
* @return
* returns the standard results of MAP_RESULT enum as described in the map header
*/
MapResult mapRemoveAid(Map map,Element back,PutCase putC);
  

/* ----------------------------------------------------------------------

                        header-included function's defenitions

------------------------------------------------------------------------*/

Map mapCreate(copyMapDataElements copyDataFnc,
              copyMapKeyElements copyKeyFnc,
              freeMapDataElements freeDataFnc,
              freeMapKeyElements freeKeyFnc,
              compareMapKeyElements compareKeyFnc)
{
    if (!copyDataFnc || !copyKeyFnc || !freeDataFnc || !freeKeyFnc || !compareKeyFnc)
    {
        return NULL;
    }
    Map newMap = (Map)malloc(sizeof(struct Map_t));
    
    if (!newMap)
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
    if (!map)
    {
        return;
    }

    Element temp = map->first;
    
    while(temp != NULL)
    {
      
        Element toDelete = temp;
        temp = temp->next;
        destroyElement(map,toDelete);
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

    Element ptr = originalMap->first;
    while(ptr != NULL)
    {   
        mapPut(newMap,ptr->key,ptr->data);
        ptr = ptr->next;
    }
    return newMap;
}

int mapGetSize(Map map)
{
    if(map == NULL)
    {
        return NOT_FOUND;
    }

    Element ptr = map->first;
    int counter = 0;

    
    while(ptr != NULL)
    {
       
        counter ++;
        ptr = ptr->next;
    }
    return counter;
    
}

bool mapContains(Map map, MapKeyElement element)
{
    if(!map || !element)
    {
        return false;
    }

    Element ptr = map->first;
    while(ptr != NULL)
    {
        if(map->compareKeyFnc(ptr->key,element) == 0)
        {
            return true;
        }
        
    }

    return false;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    
    if(map == NULL || keyElement == NULL||dataElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    PutCase putC = PUT_INITIALIZED;

    Element back = NULL;
    Element front = NULL;
    if(map->first == NULL)
    {
        putC = PUT_EMPTY;
    }

    else if(map->compareKeyFnc(map->first->key,keyElement) > 0)
    {
       putC = PUT_FIRST;
    }
    else 
    {
         back = NULL;
         front = map->first;
        while(front != NULL && putC == PUT_INITIALIZED)
        {
            if(map->compareKeyFnc(front->key,keyElement) == 0)
            {
                putC = PUT_REPLACE;
            }
            else if(map->compareKeyFnc(front->key,keyElement) > 0)
            {
                putC = PUT_MID;
            }
            else
            {
                back = front;
                front = front->next;
            }
        }
        if(putC == PUT_INITIALIZED)
        {
            putC = PUT_LAST;
        }

    }
  
    return mapPutAid(map,keyElement,dataElement,back,front,putC);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
   
    if(!map || !keyElement)
    {
        return NULL;
    }

    Element ptr = map->first;
    while(ptr!= NULL)
    {
       
        if(map->compareKeyFnc(ptr->key,keyElement) == 0)
        {
            return ptr->data;
        }
        ptr = ptr->next;
    }

    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    
   if(map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    PutCase putC = PUT_INITIALIZED;

    Element back = NULL;
    Element front = map->first;
    while(map->first != NULL && putC == PUT_INITIALIZED)
    {
        if(map->compareKeyFnc(front->key,keyElement) == 0)
            {
                front = front->next;
                if(front != NULL)
                {
                    if(back != NULL)
                    {
                        putC = PUT_MID;
                    }
                    else
                    {
                        putC = PUT_FIRST; 
                    }
                }
                else
                {
                    if(back != NULL)
                    {
                        putC = PUT_LAST;
                    }
                    else
                    {
                        putC = PUT_EMPTY;
                    }
                }
               

            }

        if(putC == PUT_INITIALIZED)
        {
            back = front;
            front = front->next;
        }
    }  
  return mapRemoveAid(map,back,putC);
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
    if (!map)
    {
        return MAP_NULL_ARGUMENT;
    }

    Element temp = map->first;
    while(!temp)
    {
        Element toDelete = temp;
        temp = temp->next;
        map->freeDataFnc(toDelete);
        map->freeKeyFnc(toDelete);
        free(toDelete);
    }

    return MAP_SUCCESS;
}

/* ----------------------------------------------------------------------

             non header-included function's defenitions(aid functions)

------------------------------------------------------------------------*/

void destroyElement(Map map,Element element)
{
    if(element != NULL)
        {
            map->freeDataFnc(element->data);
            map->freeKeyFnc(element->key);
            free(element);
        }
}


MapResult mapRemoveAid(Map map,Element back,PutCase putC)
{
    MapResult result;
     switch (putC)
   {
   case PUT_EMPTY:
        result =  removeFirstElement(map);
       break;
   case PUT_FIRST:
       result =  removeFirstElement(map);
       break;
    case PUT_MID:
        result =  removeMidElement(map,back);
       break;
    case PUT_LAST:
        result = removeMidElement(map,back);
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

MapResult removeFirstElement(Map map)
{
    if(map == NULL|| map->first == NULL||map->first->key == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Element newFirst = map->first->next;
    destroyElement(map,map->first);
    map->first = newFirst;
    return MAP_SUCCESS; 
}

MapResult removeMidElement(Map map,Element back)
{
    if(map == NULL|| back == NULL||back->next == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    Element victim = back->next;
    Element front = victim->next;

    destroyElement(map,victim);
    back->next = front;

    return MAP_SUCCESS; 
}

MapResult mapPutAid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front,PutCase putC)
{
    MapResult result;
     switch (putC)
   {
   case PUT_EMPTY:
        result = putNewBlankFirst(map,key,data);
       break;
   case PUT_FIRST:
        result = putNewFirst(map,key,data);
       break;
   case PUT_REPLACE:
        result = replaceElementData(map,front,key,data); 
       break;
    case PUT_MID:
        result = putNewMid(map,key,data,back,front);
       break;
    case PUT_LAST:
        result = putNewMid(map,key,data,back,NULL);
       break;
    default :
        return MAP_NULL_ARGUMENT;

       
   }

   return result;
}

 MapResult putNewBlankFirst(Map map, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Element newElement = createNewElement(map,key,data);
    if(newElement == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    map->first = newElement;
    return MAP_SUCCESS;
}

MapResult putNewFirst(Map map, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Element newElement = createNewElement(map,key,data);
    if(newElement == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    Element temp = map->first;
    map->first = newElement;
    newElement->next = temp;
    return MAP_SUCCESS;
}

MapResult replaceElementData(Map map,Element element, MapKeyElement key,MapDataElement data)
{
    if(map == NULL || key == NULL || data == NULL||element == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    MapDataElement newData = map->copyDataFnc(data);
    if(newData == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    map->freeDataFnc(element->data);
    element->data = newData;
    return MAP_SUCCESS;
}

MapResult putNewMid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front)
{
    if(map == NULL || key == NULL || data == NULL|| back == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Element newElement = createNewElement(map,key,data);
    if(newElement == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }

    back->next = newElement;
    newElement->next = front;
    
    return MAP_SUCCESS;
}

 Element createNewElement(Map map , MapKeyElement key,MapDataElement data)
{
    if(!data|| ! key)
    {
        return NULL;
    }
    Element newElm = malloc(sizeof(struct entry));
    if(newElm != NULL)
    {
        newElm->data = map->copyDataFnc(data);
        if(newElm->data == NULL)
        {
            free(newElm);
            return NULL;   
        }

        newElm->key = map->copyKeyFnc(key);
        if(newElm->key == NULL)
        {
            free(newElm->data);
            free(newElm);
            return NULL;   
        }
        newElm->next = NULL;
        
    }
    return newElm;
}


















