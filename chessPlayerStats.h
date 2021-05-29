#ifndef _CHESS_STATS_H
#define _CHESS_STATS_H

#include<stdio.h>
#include<stdbool.h>
#include"./chessSystem.h"
/* ----------------------------------------------------------------------

                         Player stats required constants

------------------------------------------------------------------------*/
#define WINNER_NOT_DECIDED -1
#define STATS_NOT_CALCULATED -1
#define PLAYER_REMOVED -1

/* ----------------------------------------------------------------------

                         Data structs declerations

------------------------------------------------------------------------*/
typedef struct player_stats_t *PlayerStats;

typedef struct player_rank_t *PlayerRank;

/* ----------------------------------------------------------------------

                         Header functions declerations

------------------------------------------------------------------------*/
/**
 * @brief Prints player ranks list's details into a given file 
 * 
 * @param list 
 * @param size 
 * @param file 
 */
void insertLevelListToFile(PlayerRank list,int size, FILE* file);
/**
 * @brief Calulates a player level(system criteria) by given stats
 * 
 * @param chess_stats 
 * @return double 
 */
double calculateLevel(PlayerStats chess_stats);

/**
 * @brief Returns the adjacent PlayerRank location to a given one [used for iterating lists of ranks] 
 * 
 * @param list 
 * @return Next cell of the given PlayerRank list location
 */
PlayerRank statsGetNextPlayerRankList(PlayerRank list);

/**
 * @brief Allocates a new list of player ranks with a given length[values are uninitialized]
 * 
 * @param size Length of the list 
 * @return PlayerRank list[length determined by size parameter]
 */
PlayerRank statsGetPlayerRankList(int size);

/**
 * @brief Determines the winner of two given id's by given Winner value and returns his id
 * 
 * @param first_id 
 * @param second_id 
 * @param winner Winner enum [presented at chessSystem.h]
 * @return Corresponding id given matching the winner value[returns STATS_NOT_CALCULATED const if winner value can't determine a winner]
 */
int statsGetWinnerId(int first_id,int second_id,Winner winner);

/**
 * @brief Determines the loser of two given id's by given Winner value and returns his id
 * 
 * @param first_id 
 * @param second_id 
 * @param winner Winner enum [presented at chessSystem.h]
 * @return Corresponding id given matching the winner value[returns STATS_NOT_CALCULATED const if winner value can't determine a winner] 
 */
int statsGetLoserId(int first_id,int second_id,Winner winner);

/**
 * @brief Create a Blank Player Stats object[values initialized with const STATS_NOT_CALCULATED]
 * 
 * @return PlayerStats struct with empty values 
 */
PlayerStats createBlankPlayerStats();

/**
 * @brief Create a Player Stats object filled with given values
 * 
 * @param wins 
 * @param losses 
 * @param draws 
 * @return PlayerStats struct with corresponding values  
 */
 PlayerStats createPlayerStats(int wins, int losses, int draws);

 /**
  * @brief Makes a copy of given PlayerStats type[used primarilly in map GDT as copy function]
  * 
  * @param original_stats 
  * @return PlayerStats 
  */
 PlayerStats copyPlayerStats(PlayerStats original_stats);

 /**
  * @brief Frees a PlayerStats sturcture
  * 
  * @param original_stats structure to be freed
  */
 void freePlayerStats(PlayerStats original_stats);

 /**
  * @brief Returns the time played value of given stats structure
  * 
  * @param stats 
  * 
  */
 int statsGetTime(PlayerStats stats);

 /**
  * @brief Adds time to a given stats structure's time variable
  * 
  * @param stats 
  * @param time_played 
  * @return 
  * CHESS_NULL_ARGUMENT - if the given stats is NULL
  * CHESS_SUCCESS - if the process was successfull
  */ 
ChessResult statsAddTime(PlayerStats stats,int time_played);

/**
 * @brief Turns a given stat's variables to zero
 * 
 * @param stats 
 */
void zeroPlayerStats(PlayerStats stats);

/**
 * @brief Changes a given rank's level variable
 * 
 * @param rank 
 * @param level 
 */
void rankSetLevel(PlayerRank rank,double level);

/**
 * @brief Changes a given rank's id variable
 * 
 * @param rank 
 * @param id 
 */
void rankSetId(PlayerRank rank,int id);

/**
 * @brief Adds an aditional win to a given player stats
 * 
 * @param stats 
 */
void statsAddWin(PlayerStats stats);

/**
 * @brief Adds an aditional Loss to a given player stats
 * 
 * @param stats 
 */
void statsAddLoss(PlayerStats stats);

/**
 * @brief Adds an aditional draw to a given player stats
 * 
 * @param stats 
 */
void statsAddDraw(PlayerStats stats);

/**
 * @brief Subtracrs a win from a given player stats
 * 
 * @param stats 
 */
void statsRemoveWin(PlayerStats stats);

/**
 * @brief Subtracrs a loss from a given player stats
 * 
 * @param stats 
 */
void statsRemoveLoss(PlayerStats stats);

/**
 * @brief Subtracrs a draw from a given player stats
 * 
 * @param stats 
 */
void statsRemoveDraw(PlayerStats stats);

/**
 * @brief Calculates a given stat's score value[relies on stat's preformance variables]
 * 
 * @param stats 
 */
void calculateStats(PlayerStats stats);

/**
 * @brief Calculates the score for given preformance values
 * 
 * @param wins 
 * @param draws 
 * @param losses 
 * @return The score calculated 
 */
int calculateScore(int wins,int draws, int losses);

/**
 * @brief Returns the total games played by a given player's stats
 * 
 * @param stats 
 * @return Total games played
 */
int statsGetTotalGames(PlayerStats stats);

/**
 * @brief Compares two player's stats and determines which one have higher score
 * 
 * @param first_stats 
 * @param second_stats 
 * @return Winner 
 */
Winner statsTournamentCompareHigher(PlayerStats first_stats,PlayerStats second_stats);

/**
 * @brief Substracts all of the stats given in the second argument from the first one
 * 
 * @param stats_summary 
 * @param stats_update 
 */
void statsSubtractStats(PlayerStats stats_summary, PlayerStats stats_update);

#endif