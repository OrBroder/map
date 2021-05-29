
#ifndef _CHESS_GAME_H
#define _CHESS_GAME_H
#include "./chessPlayerStats.h"
#include <stdbool.h>

/* ----------------------------------------------------------------------

                         Data structs declerations

------------------------------------------------------------------------*/
typedef struct game_t * ChessGame;


/* ----------------------------------------------------------------------

                         Enum defenition

------------------------------------------------------------------------*/
typedef enum
{
    PLAYER_IS_FIRST,
    PLAYER_IS_SECOND,
    PLAYER_NOT_IN_GAME,
    PLAYER_WON,
    PLAYER_LOST,
    PLAYER_DRAW,
} PlayerPresent;
/* ----------------------------------------------------------------------

                         Header functions declerations

------------------------------------------------------------------------*/
/**
 * @brief Get's the player's result of the game
 * 
 * @param game 
 * @param player 
 * @return 
 * PLAYER_WON
 * PLAYER_LOST
 * PLAYER_DRAW
 * PLAYER_NOT_IN_GAME
 */
PlayerPresent getPlayerGameResult(ChessGame game, int player);

/**
 * @brief Checks if both of the players are attending the given game
 * 
 * @param game 
 * @param first_player 
 * @param second_player 
 * @return true 
 * @return false 
 */
bool checkBothPlayersInGame(ChessGame game, int first_player,int second_player);

/**
 * @brief Removes a player from a game
 * 
 * @param game 
 * @param removed_player_id 
 * @param remained_player_id - Pointer filled with the remaining player id 
 * @param remained_player_previous_result  - Pointer filled with the remaining player game result prior to removal
 * @return ChessResult 
 * CHESS_SUCCESS 
 * CHESS_PLAYER_NOT_EXIST
 */
ChessResult removePlayerFromGame(ChessGame game, int removed_player_id,
int* remained_player_id,PlayerPresent* remained_player_previous_result );
/**
 * @brief Frees an allocated game
 * 
 * @param game 
 */
void freeGame(ChessGame game);
/**
 * @brief Creates a copy of a given game
 * 
 * @param original_game 
 * @return ChessGame 
 */
ChessGame copyGame(ChessGame original_game);
/**
 * @brief Checks if a player is attending a given game
 * 
 * @param game 
 * @param id 
 * @return  
 * PLAYER_IS_FIRST
 * PLAYER_IS_SECOND 
 * PLAYER_NOT_IN_GAME
 */
PlayerPresent playerIsInGame(ChessGame game, int id);

/**
 * @brief Creates a game with given values
 * 
 * @param first_player 
 * @param second_player 
 * @param duration 
 * @param result 
 * @return ChessGame 
 */
ChessGame gameCreate(int first_player, int second_player, int duration, Winner result);

/**
 * @brief Checks if given values for new match are valid
 * 
 * @param tournament_id 
 * @param first_player 
 * @param second_player 
 * @return 
 * CHESS_SUCCESS
 * CHESS_INVALID_ID
 * 
 */
ChessResult checkGameVaraiables(int tournament_id, int first_player, int second_player);

/**
 * @brief Checks if given play time is valid
 * 
 * @param play_time 
 * @return ChessResult 
 */
ChessResult checkValidPlayTime(int play_time);
 

#endif 