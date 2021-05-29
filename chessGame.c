#include <stdio.h>
#include <stdlib.h>
#include "./chessSystem.h"
#include "./chessGame.h"

/* ----------------------------------------------------------------------

                         Data structs defenitions

------------------------------------------------------------------------*/
/** Struct used for pointing at a chess-game's details
 * @param  player1ID - holds the first player's of the game's ID(int)
 * @param player1ID - holds the second player's of the game's ID(int)
 * @param gameDuration - holds the duration of the game in seconds(int)
 * @param winner - holds the result of the game(Winner enum)
 */
struct game_t
{
    int player1ID;
    int player2ID;
    int gameDuration;
    Winner result;
};

/* ----------------------------------------------------------------------

                    Header included functions defenitions

------------------------------------------------------------------------*/
PlayerPresent getPlayerGameResult(ChessGame game, int player)
{
    PlayerPresent player_position = playerIsInGame(game, player);
    if (player_position != PLAYER_NOT_IN_GAME)
    {
        switch (game->result)
        {
        case FIRST_PLAYER:
            if (player_position == PLAYER_IS_FIRST)
            {
                return PLAYER_WON;
            }
            else
            {
                return PLAYER_LOST;
            }
            break;
        case SECOND_PLAYER:
             if (player_position == PLAYER_IS_SECOND)
            {
                return PLAYER_WON;
            }
            else
            {
                return PLAYER_LOST;
            }
            break;
        case DRAW:
                return PLAYER_DRAW;
            break;
        default:
            break;
        }
    }

    return PLAYER_NOT_IN_GAME;
}


 ChessResult removePlayerFromGame(ChessGame game, int removed_player_id,
  int* remained_player_id,PlayerPresent* remained_player_previous_result )
{
    if (removed_player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    PlayerPresent removed_player_position = playerIsInGame(game, removed_player_id);
    if (removed_player_position != PLAYER_NOT_IN_GAME)
    {
        if (removed_player_position == PLAYER_IS_FIRST)
        {
            *remained_player_id = game->player2ID;
            
            if(game->result == FIRST_PLAYER)
            {
                *remained_player_previous_result = PLAYER_LOST;
            }
            else if(game->result == DRAW)
            {
                 *remained_player_previous_result = PLAYER_DRAW;
            }
           
            game->result = SECOND_PLAYER;
            game->player1ID = PLAYER_REMOVED;
        }
        else
        {
            *remained_player_id = game->player1ID;
            if(game->result == SECOND_PLAYER)
            {
               *remained_player_previous_result = PLAYER_LOST;
            }
            else if(game->result == DRAW)
            {
               *remained_player_previous_result = PLAYER_DRAW;
            }
           
            game->result = FIRST_PLAYER;
            game->player2ID = PLAYER_REMOVED;
        }
        return CHESS_SUCCESS;
    }

    return CHESS_PLAYER_NOT_EXIST;
}

 void freeGame(ChessGame game)
{

    if (game != NULL)
    {
        free(game);
    }
}

 ChessGame copyGame(ChessGame original_game)
{
    ChessGame new_game = malloc(sizeof(struct game_t));
    if (new_game == NULL)
    {
        return NULL;
    }
    new_game->player1ID = original_game->player1ID;
    new_game->player2ID = original_game->player2ID;
    new_game->gameDuration = original_game->gameDuration;
    new_game->result = original_game->result;

    return new_game;

    return new_game;
}
 ChessGame gameCreate(int first_player, int second_player, int duration, Winner result)
{
    ChessGame new_game = malloc(sizeof(struct game_t));
    if (new_game == NULL)
    {
        return NULL;
    }
    new_game->player1ID = first_player;
    new_game->player2ID = second_player;
    new_game->gameDuration = duration;
    new_game->result = result;

    return new_game;
}

 ChessResult checkGameVaraiables(int tournament_id, int first_player, int second_player)
{
    if (tournament_id <= 0 || first_player <= 0 || second_player <= 0 || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }
   

    return CHESS_SUCCESS;
}

 PlayerPresent playerIsInGame(ChessGame game, int id)
{
    if (game->player1ID == id)
    {
        return PLAYER_IS_FIRST;
    }
    else if (game->player2ID == id)
    {
        return PLAYER_IS_SECOND;
    }

    return PLAYER_NOT_IN_GAME;
}

 bool checkBothPlayersInGame(ChessGame game, int first_player,int second_player)
{
    if(game == NULL)
    {
        return false;
    }
    return (game->player1ID == first_player && game->player2ID == second_player)
    ||(game->player2ID == first_player && game->player1ID == second_player);
}

 ChessResult checkValidPlayTime(int play_time)
{
    if(play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    return CHESS_SUCCESS;

}
