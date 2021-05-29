#ifndef _CHESS_CHECK_MACROS_H
#define _CHESS_CHECK_MACROS_H


#define CHECK_TRUE_RETURN(bool_expression) \
    if(bool_expression == true) \
        return true;\

#define CHECK_NULL_RETURN(ptr) \
    if(ptr == NULL) \
        return NULL;\

#define CHECK_NULL_RETURN_NOT_CALCULATED(ptr) \
    if(ptr == NULL) \
        return STATS_NOT_CALCULATED;\

#define CHESS_CHECK_NULL_RETURN(ptr) \
    if(ptr == NULL) \
        return CHESS_NULL_ARGUMENT;\

#define CHECK_NULL_VOID(ptr) \
    if(ptr == NULL) \
        return ;\

#define CHESS_CHECK_NOT_SUCCESS_RETURN(res) \
    if(res != CHESS_SUCCESS) \
        return res;\

#define CHESS_CHECK_TOURNAMENT_NOT_EXIST_RETURN(chess,id) \
     if (mapContains(chess->system_tournaments, &tournament_id))\
        return CHESS_TOURNAMENT_ALREADY_EXISTS;\

#define CHESS_CHECK_TOURNAMENT_DOES_EXIST_RETURN(chess,id) \
     if (!mapContains(chess->system_tournaments, &tournament_id))\
        return CHESS_TOURNAMENT_NOT_EXIST;\

#define CHESS_CHECK_TOURNAMENT_NOT_ENDED_RETURN(tournament) \
     if (tournamentCheckEnded(tournament) == true)\
        return CHESS_TOURNAMENT_ENDED;\

#define CHESS_CHECK_PLAYERS_NOT_MATCHED_RETURN(tournament,first_player,second_player) \
     if (checkBothPlayersDidMatch(tournament, first_player, second_player) == true)\
        return CHESS_GAME_ALREADY_EXISTS;\

#define CHESS_CHECK_NOT_SUCCESS_RETURN_NULL(res) \
    if(res != CHESS_SUCCESS) \
        return res;\

#define CHECK_DRAW_CONTINUE_ELSE_RET(res) \
    if(res != DRAW) \
        return res;\

#endif 
     