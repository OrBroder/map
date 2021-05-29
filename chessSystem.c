#include "./chessReturnsMacros.h"
#include "./chessPlayerStats.h"
#include "./mtm_map/map.h"
#include "./chessId.h"
#include "./chessSystem.h"
#include "./chessGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "./chessTournament.h"
#define WINNER_NOT_DECIDED -1
#define STATS_NOT_CALCULATED -1
#define PLAYER_REMOVED -1




/* ----------------------------------------------------------------------

                        code structs

------------------------------------------------------------------------*/


/** Struct used for pointing at a chess tournaments managing system details
 * @param system_tournaments - holds the generic data type structure of the system's tournaments(Map)
 */
struct chess_system_t
{
    Map system_tournaments;
    Map players_system_stats;
};
/* ----------------------------------------------------------------------

                        internal code functions declerations

------------------------------------------------------------------------*/
/**
 * @brief Deletes a chess system's tournaments map
 * 
 * @param chess 
 */
static void chessDeleteSystemTournaments(ChessSystem chess);

/**
 * @brief Checks if a chess system does not have a tournament with the given key
 * 
 * @param chess 
 * @param tournament_id 
 * @return ChessResult 
 */
static ChessResult chessCheckTournamentNotExist(ChessSystem chess,int tournament_id);

/**
 * @brief Returns a player's system stats by given id
 * 
 * @param chess 
 * @param player_id 
 * @return PlayerStats 
 */
static PlayerStats chessGetPlayerStats(ChessSystem chess,int player_id);

/**
 * @brief Adds a draw to a player's system stats
 * 
 * @param chess 
 * @param player_id 
 */
static void chessAddDrawToPlayerStats(ChessSystem chess,int player_id);

/**
 * @brief Adds a loss to a player's system stats
 * 
 * @param chess 
 * @param player_id 
 */
static void chessAddLossToPlayerStats(ChessSystem chess,int player_id);

/**
 * @brief Adds a win to a player's system stats
 * 
 * @param chess 
 * @param player_id 
 */
static void chessAddWinToPlayerStats(ChessSystem chess,int player_id);
/**
 * @brief Adds to both players system stats a draw
 * 
 * @param chess 
 * @param first_player 
 * @param second_player 
 */
static void chessAddDrawToBothPlayerStats(ChessSystem chess,int first_player,int second_player);
/**
 * @brief Adds to both players system and tournament statistics by given game variables
 * 
 * @param chess 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @param winner 
 * @param play_time 
 */
static void chessUpdatePlayersSystemAndTournamentStats(ChessSystem chess,ChessTournament tournament,int first_player,int second_player,Winner winner,int play_time);
/**
 * @brief Adds to both players system statistics by given game variables
 * 
 * @param chess 
 * @param first_player 
 * @param second_player 
 * @param winner 
 * @param play_time 
 */
static void chessUpdateSystemGameResult(ChessSystem chess,int first_player,int second_player,Winner winner,int play_time);

/**
 * @brief Removes a player's preformance stats from the system
 * 
 * @param chess 
 * @param player_id 
 * @return ChessResult 
 */
static ChessResult chessRemovePlayerStats(ChessSystem chess, int player_id);
/**
 * @brief Converts MapResult(map.h enum) values to corresponding values of ChessResult(chessSystem.h enum)
 * 
 * @param m_result 
 * @return ChessResult 
 */
static ChessResult chessConvertMapToChessResult(MapResult m_result);
/**
 * @brief Initiate system's variables(creates statistics map and tournament map)
 * 
 * @param new_chess_system 
 */
static void chessInitiateSystemVariables(ChessSystem new_chess_system);
/**
 * @brief Adds to both players system stats play time
 * 
 * @param chess 
 * @param firts_player 
 * @param second_player 
 * @param play_time 
 */
static void chessAddPlayTimeToBothPlayerStats(ChessSystem chess,int firts_player,int second_player,int play_time);
/**
 * @brief Adds a tournament to the system(assusmes valid variables)
 * 
 * @param chess 
 * @param tournament_id 
 * @param max_games_per_player 
 * @param tournament_location 
 * @return ChessResult 
 */
static ChessResult chessAddValidTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char *tournament_location);
/**
 * @brief Delets a players system stats record
 * 
 * @param chess 
 */
static void chessDeleteSystemPlayersStats(ChessSystem chess);
/**
 * @brief Returns a list containing the system's players levels and ranks
 * 
 * @param chess 
 * @return PlayerRank 
 */
static PlayerRank insertLevelsIds(ChessSystem chess);
/**
 * @brief Adds both players to system's players stats(if they do not have stats already)
 * 
 * @param chess 
 * @param first_player 
 * @param second_player 
 */
static void chessCheckAddBothPlayersSystem(ChessSystem chess, int first_player, int second_player);
/**
 * @brief Adds a player to system's players stats(if he is not there yet)
 * 
 * @param chess 
 * @param player 
 */
static void checkAddPlayerSystem(ChessSystem chess, int player);
/**
 * @brief Updates players system stats according to a removed tournament(deletes their preformance in the tournament)
 * 
 * @param chess 
 * @param tournament 
 */
static void chessUpdateSystemPlayersStatsRemoveTournament(ChessSystem chess,ChessTournament tournament);
/**
 * @brief Returns the Player total games played in a given system
 * 
 * @param chess_sys 
 * @param player_id 
 * @return int 
 */
static int chessGetPlayerTotalGamesInSystem(ChessSystem chess_sys,int player_id);


/* ----------------------------------------------------------------------

                        header-included function's defenitions

------------------------------------------------------------------------*/
ChessSystem chessCreate()
{
    ChessSystem new_chess_system = malloc(sizeof(struct chess_system_t));
    CHECK_NULL_RETURN(new_chess_system);
    chessInitiateSystemVariables(new_chess_system);
    
    return new_chess_system;
}

void chessDestroy(ChessSystem chess)
{
   
        CHECK_NULL_VOID(chess);
        chessDeleteSystemTournaments(chess);
        chessDeleteSystemPlayersStats(chess);
        free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char *tournament_location)
{
    CHESS_CHECK_NULL_RETURN(chess);
    ChessResult check_result;

    
    check_result = checkValidTournamentId(tournament_id);
    CHESS_CHECK_NOT_SUCCESS_RETURN(check_result);

    check_result = chessCheckTournamentNotExist(chess, tournament_id);
    CHESS_CHECK_NOT_SUCCESS_RETURN(check_result);

    check_result = isValidLocation(tournament_location);
    CHESS_CHECK_NOT_SUCCESS_RETURN(check_result);

    check_result = checkValidMaxGames(max_games_per_player);
    CHESS_CHECK_NOT_SUCCESS_RETURN(check_result);

    return chessAddValidTournament(chess, tournament_id,  max_games_per_player,tournament_location);
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    CHESS_CHECK_NULL_RETURN(chess);

    ChessResult variable_check = checkGameVaraiables(tournament_id, first_player, second_player);
    CHESS_CHECK_NOT_SUCCESS_RETURN(variable_check);

    
    CHESS_CHECK_TOURNAMENT_DOES_EXIST_RETURN(chess,tournament_id);
    ChessTournament tournament = mapGet((Map)chess->system_tournaments, (MapKeyElement)&tournament_id);
     
    variable_check = tournamentAddGame(tournament, first_player, second_player,winner,  play_time);

    CHESS_CHECK_NOT_SUCCESS_RETURN(variable_check);
    
    chessUpdatePlayersSystemAndTournamentStats(chess,tournament,first_player,second_player,winner,play_time);

    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    
    if (tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if (chess == NULL || chess->system_tournaments == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(!mapContains(chess->system_tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    ChessTournament touranament_removed = mapGet(chess->system_tournaments,(MapKeyElement)&tournament_id);

    chessUpdateSystemPlayersStatsRemoveTournament(chess,touranament_removed);
     return convertMapToChessResultTournament(mapRemove((chess->system_tournaments), (MapKeyElement)&tournament_id));

}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }

    ChessResult current_result = CHESS_PLAYER_NOT_EXIST;
    ChessResult final_result = CHESS_PLAYER_NOT_EXIST;

    MapKeyElement current_key = mapGetFirst(chess->system_tournaments);
    
    ChessTournament current_tournament = mapGet(chess->system_tournaments, current_key);
    while (current_tournament != NULL)
    {
        current_result = removePlayerFromTournament(current_tournament, player_id,chess->players_system_stats);
        if (current_result == CHESS_SUCCESS)
        {
            final_result = CHESS_SUCCESS;
        }
         freeIdKey(current_key);
        current_key = mapGetNext(chess->system_tournaments);
       
        current_tournament = mapGet(chess->system_tournaments, current_key);
    }
    chessRemovePlayerStats(chess,player_id);
    freeIdKey(current_key);

    return final_result;
}

ChessResult chessEndTournament (ChessSystem chess, int
tournament_id)
{
    CHESS_CHECK_NULL_RETURN(chess);
    ChessResult result = checkValidTournamentId(tournament_id);
    CHESS_CHECK_NOT_SUCCESS_RETURN(result);
    CHESS_CHECK_TOURNAMENT_DOES_EXIST_RETURN(chess,tournament_id);
    ChessTournament tournament = mapGet(chess->system_tournaments,&tournament_id);
    if(tournament == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    return tournamentSetEnded(tournament);
}

double chessCalculateAveragePlayTime (ChessSystem chess, int
player_id, ChessResult* chess_result)
{
    if(chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return (double)STATS_NOT_CALCULATED;
    }
    if(player_id < 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return (double)STATS_NOT_CALCULATED;
    }
    
    PlayerStats player_system_stats = chessGetPlayerStats(chess,player_id);
    if(player_system_stats == NULL)
        {
           *chess_result = CHESS_PLAYER_NOT_EXIST;
            return (double)STATS_NOT_CALCULATED; 
        }

    
    int total_time = statsGetTime(player_system_stats);
    int total_games = chessGetPlayerTotalGamesInSystem(chess,player_id);
    double average = total_time/(double)total_games;

        *chess_result = CHESS_SUCCESS;
        return average;

}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    CHESS_CHECK_NULL_RETURN(chess);
    if(chess == NULL||file == NULL)
    {
        return CHESS_SAVE_FAILURE;
    }

   ChessResult result = CHESS_SUCCESS;
   int length = mapGetSize(chess->players_system_stats);
   PlayerRank list = insertLevelsIds(chess);
   insertLevelListToFile(list,length,file);
   free(list);
    
    return result;
    
}


/* ----------------------------------------------------------------------

             non header-included function's defenitions(aid functions)

------------------------------------------------------------------------*/

static ChessResult chessRemovePlayerStats(ChessSystem chess, int player_id)
{
      if(chess == NULL)
      {
          return CHESS_NULL_ARGUMENT;
      }
    
        if(player_id <= 0)
        {
            return CHESS_INVALID_ID;
        }

   
    return chessConvertMapToChessResult(mapRemove(chess->players_system_stats,&player_id));
}

static void chessDeleteSystemTournaments(ChessSystem chess)
{
    CHECK_NULL_VOID(chess);
    mapDestroy(chess->system_tournaments);
}

static ChessResult chessAddValidTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char *tournament_location)
{
    ChessTournament new_tournament = createTournamentValid(max_games_per_player, tournament_location);
    mapPut(chess->system_tournaments, &tournament_id, new_tournament);
    freeTournament(new_tournament);
    return CHESS_SUCCESS;
}

static void chessInitiateSystemVariables(ChessSystem new_chess_system)
{
    new_chess_system->system_tournaments = mapCreate((copyMapDataElements)copyTournament, copyIdKey, (freeMapDataElements)freeTournament, freeIdKey, (compareMapKeyElements)(compareIdKeys));
    new_chess_system->players_system_stats = mapCreate((copyMapDataElements)copyPlayerStats,(copyMapKeyElements)copyIdKey, 
    (freeMapDataElements)freePlayerStats, (freeMapKeyElements)freeIdKey, (compareMapKeyElements)compareIdKeys);
}



static void chessUpdateSystemPlayersStatsRemoveTournament(ChessSystem chess,ChessTournament tournament)
{
     CHECK_NULL_VOID(chess);
     CHECK_NULL_VOID(tournament);
   
    MapKeyElement current_player_key = tournamentGetFirstPlayerKeyCopy(tournament);
    PlayerStats current_player_tournament_stats = tournamentGetPlayerStatsCopy(tournament,current_player_key);
    
    while(current_player_tournament_stats != NULL)
    {
        PlayerStats current_player_system_stats = chessGetPlayerStats(chess,*(int*)current_player_key);
        statsSubtractStats(current_player_system_stats,current_player_tournament_stats);
        
        freeIdKey(current_player_key);
        freePlayerStats(current_player_tournament_stats);
        current_player_key = tournamentGetNextPlayerKeyCopy(tournament);
        current_player_tournament_stats = tournamentGetPlayerStatsCopy(tournament,current_player_key);
    }
    

}



static void checkAddPlayerSystem(ChessSystem chess, int player)
{
    if(player == PLAYER_REMOVED)
    {
        return;
    }
    if (!mapContains(chess->players_system_stats, (MapKeyElement)(&player)))
    {
       
        PlayerStats new_stats = createBlankPlayerStats();
        zeroPlayerStats(new_stats);
        mapPut(chess->players_system_stats, (MapKeyElement)&player, (MapDataElement)new_stats);
        free(new_stats);
       
    }
}

static int chessGetPlayerTotalGamesInSystem(ChessSystem chess_sys,int player_id)
{
    int current_games;
    int total_games = 0;
    
    MapKeyElement current_key = mapGetFirst(chess_sys->system_tournaments);
    ChessTournament current_tournament = mapGet(chess_sys->system_tournaments,current_key);
    if(current_tournament == NULL)
    {
        return STATS_NOT_CALCULATED;
    }
    while (current_key != NULL)
    {
         current_games = getPlayerTotalGamesInTournament(current_tournament,player_id);
         if(current_games != STATS_NOT_CALCULATED)
         {
             total_games += current_games;
         }

        freeIdKey(current_key);
        current_key = mapGetNext(chess_sys->system_tournaments);
        current_tournament = mapGet(chess_sys->system_tournaments,current_key);
        
    }
    return total_games;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char*
path_file)
{
    FILE *fptr;
    fptr = fopen(path_file,"w");
    bool is_one_tournament_ended = false;
   
    MapKeyElement current_key = mapGetFirst(chess->system_tournaments);
    ChessTournament current_tournament = mapGet(chess->system_tournaments,current_key);
    
    while(current_key != NULL)
    {
        if(tournamentCheckEnded(current_tournament))
        {
        !is_one_tournament_ended ? is_one_tournament_ended = true:true;
        int current_winner = tournamentGetWinnerId(current_tournament);
        int current_longest_game = tournamentGetLongestGame(current_tournament);
        int current_games_amount = tournamentGetGamesAmount(current_tournament);
        int total_play_time = tournamentGetTotalPlayTime(current_tournament);
        double current_average_game_time = (double)total_play_time /current_games_amount;
        char* current_location =tournamentGetLocationCopy(current_tournament);
        int current_total_players = tournamentGetPlayersAmount(current_tournament);

        fprintf(fptr,"%d\n",current_winner);
        fprintf(fptr,"%d\n",current_longest_game);
        fprintf(fptr,"%.2f\n",current_average_game_time);
        fprintf(fptr,"%s\n",current_location);
        fprintf(fptr,"%d\n",current_games_amount);
        fprintf(fptr,"%d\n",current_total_players);
        free(current_location);
        }
        
        freeIdKey(current_key);
        current_key = mapGetNext(chess->system_tournaments);
        current_tournament = mapGet(chess->system_tournaments,current_key);
        
    }

    fclose(fptr);
    if(!is_one_tournament_ended)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    return CHESS_SUCCESS;
}



static void chessDeleteSystemPlayersStats(ChessSystem chess)
{
    CHECK_NULL_VOID(chess);
    mapDestroy(chess->players_system_stats);
}




static void chessCheckAddBothPlayersSystem(ChessSystem chess, int first_player, int second_player)
{
    checkAddPlayerSystem(chess,first_player);
    checkAddPlayerSystem(chess,second_player);
}




static PlayerRank insertLevelsIds(ChessSystem chess)
{
    int players_amount = mapGetSize(chess->players_system_stats);
    MapKeyElement current_key = mapGetFirst(chess->players_system_stats);
    
    PlayerStats current_stats = mapGet(chess->players_system_stats, current_key);
    PlayerRank final_list = statsGetPlayerRankList(players_amount);
    
    PlayerRank current_list_pos = final_list;

    while (current_key != NULL)
    {
        int id = (*(int*)current_key);
        
        double current_level = calculateLevel(current_stats);
       
        rankSetId(current_list_pos,id);
        rankSetLevel(current_list_pos,current_level);
        current_list_pos = statsGetNextPlayerRankList(current_list_pos);

        freeIdKey(current_key);
        current_key = mapGetNext(chess->players_system_stats);
        current_stats = mapGet(chess->players_system_stats,current_key);
        
    }

    return final_list;
}

static void chessUpdatePlayersSystemAndTournamentStats(ChessSystem chess,ChessTournament tournament,int first_player,int second_player,Winner winner,int play_time)
{
    chessCheckAddBothPlayersSystem(chess,first_player,second_player);
    chessUpdateSystemGameResult(chess,first_player,second_player,winner,play_time);

}

static void chessUpdateSystemGameResult(ChessSystem chess,int first_player,int second_player,Winner winner,int play_time)
{

    int winner_id = statsGetWinnerId(first_player,second_player,winner);
    int loser_id = statsGetLoserId(first_player,second_player,winner);
    if(winner_id != STATS_NOT_CALCULATED)
    {
         chessAddWinToPlayerStats(chess,winner_id);
         chessAddLossToPlayerStats(chess,loser_id);
    }
    else
    {
        chessAddDrawToBothPlayerStats(chess,first_player,second_player);
    }
    chessAddPlayTimeToBothPlayerStats(chess,winner_id,loser_id,play_time);

}

static void chessAddPlayTimeToBothPlayerStats(ChessSystem chess,int firts_player,int second_player,int play_time)
{
    statsAddTime(chessGetPlayerStats(chess,firts_player),play_time);
    statsAddTime(chessGetPlayerStats(chess,second_player),play_time);
}

static void chessAddWinToPlayerStats(ChessSystem chess,int player_id)
{
    CHECK_NULL_VOID(chess);
    PlayerStats player_stats = chessGetPlayerStats(chess,player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddWin(player_stats);
}

static void chessAddLossToPlayerStats(ChessSystem chess,int player_id)
{
    CHECK_NULL_VOID(chess);
    PlayerStats player_stats = chessGetPlayerStats(chess,player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddLoss(player_stats);
}

static void chessAddDrawToBothPlayerStats(ChessSystem chess,int first_player,int second_player)
{
    chessAddDrawToPlayerStats(chess,first_player);
    chessAddDrawToPlayerStats(chess,second_player);
}

static void chessAddDrawToPlayerStats(ChessSystem chess,int player_id)
{
    CHECK_NULL_VOID(chess);
    PlayerStats player_stats = chessGetPlayerStats(chess,player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddDraw(player_stats);
}



static PlayerStats chessGetPlayerStats(ChessSystem chess,int player_id)
{
    CHECK_NULL_RETURN(chess);
    return mapGet(chess->players_system_stats,&player_id);
}





static ChessResult chessConvertMapToChessResult(MapResult m_result)
{

    ChessResult c_result = CHESS_NULL_ARGUMENT;

    switch (m_result)
    {
    case MAP_SUCCESS:
        c_result = CHESS_SUCCESS;
        break;
    case MAP_OUT_OF_MEMORY:
        c_result = CHESS_OUT_OF_MEMORY;
        break;
    case MAP_NULL_ARGUMENT:
        c_result = CHESS_NULL_ARGUMENT;
        break;
    case MAP_ITEM_ALREADY_EXISTS:
        c_result = CHESS_GAME_ALREADY_EXISTS;
        break;
    case MAP_ITEM_DOES_NOT_EXIST:
        c_result = CHESS_PLAYER_NOT_EXIST;
        break;
    default:
        c_result = CHESS_NULL_ARGUMENT;
        break;
    }

    return c_result;

}


static ChessResult chessCheckTournamentNotExist(ChessSystem chess,int tournament_id)
{
    CHESS_CHECK_NULL_RETURN(chess);
    ChessResult result;
     mapContains(chess->system_tournaments,&tournament_id) ?
      (result =  CHESS_TOURNAMENT_ALREADY_EXISTS) : (result = CHESS_SUCCESS);
     return result;
}




        





