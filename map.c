#include "./map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NOT_FOUND -1
#define EMPTY_ARGUMENT -2

/* ----------------------------------------------------------------------

                        code structs

------------------------------------------------------------------------*/

typedef struct entry
{
    MapDataElement data;
    MapKeyElement key;
    struct entry* next;
} *Element;

struct Map_t
{
    int size;
    int created;
    int deleted;
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
MapResult newPushElement(MapKeyElement key,MapDataElement data,  Element previous,Element next);
Element createNewElement(Map map ,  MapKeyElement key,MapDataElement data);
MapResult putNewBlankFirst(Map map, MapKeyElement key,MapDataElement data);
MapResult putNewFirst(Map map, MapKeyElement key,MapDataElement data);
MapResult replaceElementData(Map map,Element element, MapKeyElement key,MapDataElement data);
MapResult putNewMid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front);
MapResult mapPutAid(Map map, MapKeyElement key,MapDataElement data,Element back, Element front,PutCase putC);

    typedef enum PutCase_t {
    PUT_INITIALIZED,
    PUT_EMPTY,
    PUT_REPLACE,
    PUT_MID,
    PUT_FIRST,
    PUT_LAST,
} PutCase;

/* ----------------------------------------------------------------------

                        functions defenitions

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
    
    newMap->created = 0;
    newMap->deleted = 0;
    newMap->size = 0;

    
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
          printf("DESTROY !\n");
        printf("Key: %d",*(int*)(toDelete->key) );
        temp = temp->next;
        map->freeKeyFnc(toDelete->key);
        map->freeDataFnc(toDelete->data);
        free(toDelete);
    }

    free(map);
}

Map mapCopy(Map map)
{
    if(!map)
    {
        return NULL;
    }
    Map newMap = mapCreate(map->copyDataFnc, map->copyKeyFnc,map->freeDataFnc, map->freeKeyFnc,map->compareKeyFnc);
    Element ptr = map->first;
    while(ptr != NULL)
    {   
        mapPut(newMap,ptr->key,ptr->data);
        ptr = ptr->next;
    }
    return newMap;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    
    if(!map || !keyElement||!dataElement)
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
    while(ptr)
    {
        if(map->compareKeyFnc(ptr->key,element) == 0)
        {
            return true;
        }
        
    }

    return false;
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
 
    if(!map || !keyElement)
    {
        return MAP_NULL_ARGUMENT;
    }

    Element ptr = map->first;
    if(ptr != NULL)
    {
        if(map->compareKeyFnc(ptr->key,keyElement)==0)
            {
                printf("changing new first3 \n");
                map->first = ptr->next;
                map->freeKeyFnc(ptr->key);
                map->freeDataFnc(ptr->data);
                free(ptr);
                map->size --;
                map->deleted ++;
                return MAP_SUCCESS;
            }
    }

    while(ptr->next != NULL)
    {
        if((map->compareKeyFnc(ptr->next->key,keyElement))==0)
        {
            Element next = ptr->next->next;
            Element removed = ptr->next;
            ptr->next = next;
           
            map->freeKeyFnc(removed->key);
            map->freeDataFnc(removed->data);
            free(removed);
            return MAP_SUCCESS;
        }

        ptr = ptr->next;
        
    }

    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map)
{
    if(map == NULL || map->first ==NULL)
    {
        return NULL;
    }
    printf("key in first is : %d\n" , *(int*)(map->first->key));
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

       return result;
   }
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


















