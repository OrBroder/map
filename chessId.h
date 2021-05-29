#ifndef _CHESS_ID_H
#define _CHESS_ID_H
#include "./mtm_map/map.h"
/**
 * @brief Copy function for id's(presented as int*). Used primarily at mapd GDT sturcture for copying keys
 * 
 * @param id_key 
 * @return A new copy of id key
 */
 MapKeyElement copyIdKey(MapKeyElement id_key);
 
 /**
  * @brief Compares two key elements presented as integer id's
  * 
  * @param id1 
  * @param id2 
  * @return Matches the comare function criterias presented at map.h
  */
 int compareIdKeys(MapKeyElement id1, MapKeyElement id2);

 /**
  * @brief Frees allocation of map key presented as id(int value)
  * 
  * @param id_key 
  */
 void freeIdKey(MapKeyElement id_key);
 
 #endif