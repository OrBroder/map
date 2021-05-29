#ifndef _CHESS_TOURNAMENT_H
#define _CHESS_TOURNAMENT_H

#include "./chessPlayerStats.h"
#include "./chessSystem.h"
#include "./mtm_map/map.h"
/* ----------------------------------------------------------------------

                         Data structs declerations

------------------------------------------------------------------------*/
typedef struct tournament_t *ChessTournament;

/* ----------------------------------------------------------------------

                         Header functions declerations

------------------------------------------------------------------------*/
/**
 * @brief Checks if a tournament has ended
 * 
 * @param tournament 
 * @return true 
 * @return false 
 */
 bool tournamentCheckEnded(ChessTournament tournament);

 /**
  * @brief Sets a tournament as 'ended'
  * 
  * @param tournament 
  * @return 
  * CHESS_NULL_ARGUMENT
  * CHESS_TOURNAMENT_ENDED - If already ended
  * CHESS_NO_GAMES - If it has no games(can not end)
  * CHESS_SUCCESS
  * 
  */
 ChessResult tournamentSetEnded(ChessTournament tournament);

 
/**
 * @brief Converts return values of MapResult to ChessResult
 * 
 * @param m_result 
 * @return 
 * MAP_SUCCESS - CHESS_SUCCESS
 * MAP_OUT_OF_MEMORY - CHESS_OUT_OF_MEMORY
 * MAP_NULL_ARGUMENT - CHESS_NULL_ARGUMENT
 * MAP_ITEM_ALREADY_EXISTS - CHESS_TOURNAMENT_ALREADY_EXISTS
 * MAP_ITEM_DOES_NOT_EXIST - CHESS_TOURNAMENT_NOT_EXIST
 */
ChessResult convertMapToChessResultTournament(MapResult m_result);

/**
 * @brief Get a player's total play time in a tournament 
 * 
 * @param tournament 
 * @param player_id 
 * @return 
 * STATS_NOT_CALCULATED - if the player is not found in the tournament  
 */

int getPlayerTotalTimeInTournament(ChessTournament tournament,int player_id);

/**
 * @brief Gets a player games amount in a tournament object
 * 
 * @param tournament 
 * @param player_id 
 * @return 
 * STATS_NOT_CALCULATED - if the player is not found in the tournament 
 */
int getPlayerTotalGamesInTournament(ChessTournament tournament,int player_id);




/**
 * @brief Creates a tournament(Assuming valid values given)
 * 
 * @param max_games_per_player 
 * @param location 
 * @return ChessTournament 
 */
 ChessTournament createTournamentValid(int max_games_per_player, const char *location);

 /**
  * @brief Checks if a given string is a valid tournament location(Upper case followed by lower cases and spaces)
  * 
  * @param str 
  * @return ChessResult 
  */
 ChessResult isValidLocation(const char *str);
 
 /**
  * @brief Free function for tournaments(primarily used as a copy function for Map)
  * 
  * @param tournament 
  */
 void freeTournament(ChessTournament tournament);
  /**
  * @brief Copy function for tournaments(primarily used as a copy function for Map)
  * 
  * @param tournament 
  */
 ChessTournament copyTournament(ChessTournament original_tournament);

/**
 * @brief Removes a player from a tournament(and delets his stats from the given stats map)
 * 
 * @param tournament 
 * @param player_id 
 * @param system_player_stats 
 * @return ChessResult 
 * CHESS_PLAYER_NOT_EXIST
 * CHESS_SUCCESS
 * CHESS_INVALID_ID
 * CHESS_NULL_ARGUMENT
 */
ChessResult removePlayerFromTournament(ChessTournament tournament, int player_id,Map system_player_stats);

/**
 * @brief Adds a game to a tournament
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @param winner 
 * @param play_time 
 * @return 
 * CHESS_TOURNAMENT_ENDED
 * CHESS_GAME_ALREADY_EXISTS
 * CHESS_INVALID_PLAY_TIME
 * CHESS_EXCEEDED_GAMES
 * CHESS_OUT_OF_MEMORY
 * CHESS_SUCCESS
 * 
 */
ChessResult tournamentAddGame(ChessTournament tournament,int first_player, int second_player,
Winner winner, int play_time);
/**
 * @brief Returns a copy of the first player in the tournament stats key
 * 
 * @param tournament 
 * @return MapKeyElement 
 */
MapKeyElement tournamentGetFirstPlayerKeyCopy(ChessTournament tournament);
/**
 * @brief Returns the next copy of the player stats key called
 * 
 * @param tournament 
 * @return MapKeyElement 
 */
MapKeyElement tournamentGetNextPlayerKeyCopy(ChessTournament tournament);
/**
 * @brief Returns a players stats copy
 * 
 * @param tournament 
 * @param player_id 
 * @return PlayerStats 
 */
PlayerStats tournamentGetPlayerStatsCopy(ChessTournament tournament,MapKeyElement player_id);

/**
 * @brief Returns the tournament's winner
 * 
 * @param tournament 
 * @return int 
 */
int tournamentGetWinnerId(ChessTournament tournament);
/**
 * @brief Returns the tournament's longest game time
 * 
 * @param tournament 
 * @return int 
 */
int tournamentGetLongestGame(ChessTournament tournament);
/**
 * @brief Returns the tournament's total play time
 * 
 * @param tournament 
 * @return int 
 */
int tournamentGetTotalPlayTime(ChessTournament tournament);
/**
 * @brief Returns the tournament's winner
 * 
 * @param tournament 
 * @return int 
 */
int tournamentGetPlayersAmount(ChessTournament tournament);
/**
 * @brief Returns the tournament's amount of games
 * 
 * @param tournament 
 * @return int 
 */
int tournamentGetGamesAmount(ChessTournament tournament);
/**
 * @brief Returns a copy of the tournament's location
 * 
 * @param tournament 
 * @return char* 
 */
char* tournamentGetLocationCopy(ChessTournament tournament);
/**
 * @brief Checks if the id given is a valid tournament id
 * 
 * @param id 
 * @return 
 * CHESS_INVALID_ID
 * CHESS_SUCCESS
 */
ChessResult checkValidTournamentId(int id);
/**
 * @brief Checks if the max games amount given is a valid 
 * 
 * @param max_games 
 * @return 
 * CHESS_INVALID_MAX_GAMES
 * CHESS_SUCCESS
 */
ChessResult checkValidMaxGames(int max_games);

#endif