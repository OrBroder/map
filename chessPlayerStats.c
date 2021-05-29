

#include "./chessPlayerStats.h"
#include "./chessReturnsMacros.h"
#include <stdbool.h>
#include <stdlib.h>
/* ----------------------------------------------------------------------

                         Player stats required constants

------------------------------------------------------------------------*/
#define WIN_VALUE 2
#define DRAW_VALUE 1
#define LOSS_VALUE 0
#define LEVEL_WINS_VAL 6
#define LEVEL_LOSSES_VAL 10
#define LEVEL_DRAWS_VAL 2
/* ----------------------------------------------------------------------

                         Data structs defenitions

------------------------------------------------------------------------*/


 struct player_rank_t
{
    int id;
    double level;
} ;

 struct player_stats_t
{
    int time_played;
    int wins;
    int losses;
    int draws;
    int score;
} ;

/* ----------------------------------------------------------------------

                Non header included functions declerations

------------------------------------------------------------------------*/
/**
 * @brief Calculates the tournament score for a given player's stats
 * 
 * @param player_stats 
 * @return int 
 */
static int statsGetTournamentPoints(PlayerStats player_stats);

/**
 * @brief Compares losses between two player stats
 * 
 * @param first_stats 
 * @param second_stats 
 * @return 
 * FIRST_PLAYER - first player has more losses
 * SECOND_PLAYER - second player has more losses
 * DRAW - both players have same amount of losses
 * 
 */
static Winner statsCompareLosses(PlayerStats first_stats,PlayerStats second_stats);

/**
 * @brief Compares wins between two player stats
 * 
 * @param first_stats 
 * @param second_stats 
 * @return 
 * FIRST_PLAYER - first player has more wins
 * SECOND_PLAYER - second player has more wins
 * DRAW - both players have same amount of wins
 * 
 */
static Winner statsCompareWins(PlayerStats first_stats,PlayerStats second_stats);

/**
 * @brief Compares rank between two player stats(tournament criteria)
 * 
 * @param first_stats 
 * @param second_stats 
 * @return 
 * FIRST_PLAYER - first player has higher score
 * SECOND_PLAYER - second player has higher score
 * DRAW - both players have the same score
 * 
 */
static Winner statsCompareTournamentRank(PlayerStats first_stats,PlayerStats second_stats);

/**
 * @brief Return the opposite Winner value given
 * 
 * @param winner 
 * @return 
 * FIRST_PLAYER - if given SECOND_PLAYER
 * SECOND_PLAYER - if given FIRST_PLAYER
 * DRAW - if given DRAW
 */
static Winner statsFlipWinner(Winner winner);

/**
 * @brief Sorts a list of player's ranks(consisting of id's and level's) by the ranks(higher to lowest)
 * 
 * @param level_list 
 * @param size - length of the list
 */
static void sortPlayersLevelsList(PlayerRank level_list,int size);

/**
 * @brief 
 * 
 * @param stats 
 * @return true - all stats are non-negative
 * @return false 
 */
static bool checkValidStats(PlayerStats stats);



/* ----------------------------------------------------------------------

                 Header included functions defenitions

------------------------------------------------------------------------*/
void insertLevelListToFile(PlayerRank list,int size, FILE* file)
{
    
    PlayerRank current_player = list;
    sortPlayersLevelsList(list,size);
    for(int i = 0; i < size; i++)
    {
        fprintf(file ,  "%d %.2f\n",current_player->id, current_player->level);
        current_player ++;
    }
}

PlayerRank statsGetNextPlayerRankList(PlayerRank list)
{
    return ++list ;
}

PlayerRank statsGetPlayerRankList(int size)
{
   return malloc(sizeof(struct player_rank_t)*size);
}

int statsGetWinnerId(int first_id,int second_id,Winner winner)
{
    if(winner == FIRST_PLAYER)
    {
        return first_id;
    }
    else if(winner == SECOND_PLAYER)
    {
        return second_id;
    }

    return STATS_NOT_CALCULATED;
}

int statsGetLoserId(int first_id,int second_id,Winner winner)
{
    winner = statsFlipWinner(winner);
    return statsGetWinnerId(first_id,second_id,winner);
}

PlayerStats createBlankPlayerStats()
{
    return createPlayerStats(STATS_NOT_CALCULATED, STATS_NOT_CALCULATED, STATS_NOT_CALCULATED);
}

 PlayerStats createPlayerStats(int wins, int losses, int draws)
{
    PlayerStats new_tournament_stats = malloc(sizeof(struct player_stats_t));
    if (new_tournament_stats == NULL)
    {
        return NULL;
    }
        
    new_tournament_stats->time_played = 0;
    new_tournament_stats->wins = wins;
    new_tournament_stats->losses = losses;
    new_tournament_stats->draws = draws;
    new_tournament_stats->score = STATS_NOT_CALCULATED;
    return new_tournament_stats;
}

 PlayerStats copyPlayerStats(PlayerStats original_stats)
{
    PlayerStats new_tournament_stats = malloc(sizeof(struct player_stats_t));
    new_tournament_stats->time_played = original_stats->time_played;
    new_tournament_stats->wins = original_stats->wins;
    new_tournament_stats->losses = original_stats->losses;
    new_tournament_stats->draws = original_stats->draws;
    new_tournament_stats->score = original_stats->score;
    return new_tournament_stats;
}

 void freePlayerStats(PlayerStats original_tournament_stats)
{
    if(original_tournament_stats != NULL)
    {
        free(original_tournament_stats);
    }
    
}


int statsGetTime(PlayerStats stats)
{
    if(stats == NULL)
        return STATS_NOT_CALCULATED;
    return stats->time_played;
}


 ChessResult statsAddTime(PlayerStats stats,int time_played)
{
    if(stats == NULL)
        return CHESS_NULL_ARGUMENT;

    stats->time_played += time_played;
    return CHESS_SUCCESS;
} 

void zeroPlayerStats(PlayerStats stats)
{
    if(stats!= NULL)
        {
            stats->draws = 0;
            stats->losses = 0;
            stats->wins = 0;
            stats->score = 0;
        }
}

void rankSetLevel(PlayerRank rank,double level)
{
    rank->level = level;
}

void rankSetId(PlayerRank rank,int id)
{
    rank->id = id;
}

void statsAddWin(PlayerStats stats)
{
    stats->wins ++;
}

void statsAddLoss(PlayerStats stats)
{
    stats->losses ++;
}

void statsAddDraw(PlayerStats stats)
{
    stats->draws ++;
}

void statsRemoveWin(PlayerStats stats)
{
    stats->wins --;
}

void statsRemoveLoss(PlayerStats stats)
{
    stats->losses --;
}

void statsRemoveDraw(PlayerStats stats)
{
    stats->draws --;
}

void calculateStats(PlayerStats stats)
{
    if(stats != NULL)
        {
            if(checkValidStats(stats) == true)
            {
                stats->score = calculateScore(stats->wins,stats->draws,stats->losses);
            }
        }
        
}

int calculateScore(int wins,int draws, int losses)
{
    return wins*WIN_VALUE + losses* LOSS_VALUE + draws*DRAW_VALUE;
}

int statsGetTotalGames(PlayerStats stats)
{
    if((stats == NULL)||!checkValidStats(stats))
        return STATS_NOT_CALCULATED;
    return stats->draws + stats->losses + stats->wins;
}

int rankGetLevel(PlayerRank rank)
{
    return rank->level;
}

int rankGetId(PlayerRank rank)
{
    return rank->id;
}

double calculateLevel(PlayerStats chess_stats)
{

    int wins = chess_stats->wins;
    int losses = chess_stats->losses;
    int draws = chess_stats->draws;
    int total = draws + losses + wins;
    double sum = LEVEL_WINS_VAL*wins - LEVEL_LOSSES_VAL*losses + LEVEL_DRAWS_VAL*draws;
    return (sum/(double)total);
}

 void statsSubtractStats(PlayerStats stats_summary, PlayerStats stats_update)
{
    
    stats_summary->draws -= stats_update->draws;
    stats_summary->wins -= stats_update->wins;
    stats_summary->losses -= stats_update->losses;
    stats_summary->time_played -= stats_update->time_played;
}

void statsAddStats(PlayerStats stats_summary, PlayerStats stats_update)
{
    
    stats_summary->draws += stats_update->draws;
    stats_summary->wins += stats_update->wins;
    stats_summary->losses += stats_update->losses;
}

Winner statsTournamentCompareHigher(PlayerStats first_stats,PlayerStats second_stats)
{
    Winner res = statsCompareTournamentRank(first_stats,second_stats);
    CHECK_DRAW_CONTINUE_ELSE_RET(res);

    res = statsFlipWinner(statsCompareLosses(first_stats,second_stats));
    CHECK_DRAW_CONTINUE_ELSE_RET(res);

    return statsCompareWins(first_stats,second_stats);
    
}
/* ----------------------------------------------------------------------

                Non header included functions defenitions

------------------------------------------------------------------------*/
static void sortPlayersLevelsList(PlayerRank level_list,int size)
{
    for(int i = 0; i< size-1;i++)
        for(int j = 0; j < size - i -1;j++)
        {
            PlayerRank current = &level_list[j];
            PlayerRank next = &level_list[j+1];
            if(current->level < next->level 
            || ((current->level == next->level)&& current->level > next->level))
            {
                double swap_level = current->level;
                int swap_id = current->id;
                current->level = next->level;
                current->id = next->id;
                next->level = swap_level;
                next->id = swap_id;
            }
                 
        }
}

static bool checkValidStats(PlayerStats stats)
{
    if(stats!=NULL)
    {
        if(stats->draws >=0 && stats->losses >= 0 && stats->wins >=0)
        {
            return true;
        }
    }

    return false;
    
}




static int statsGetTournamentPoints(PlayerStats player_stats)
{
    if(checkValidStats(player_stats))
    {
        return player_stats->wins*WIN_VALUE + player_stats->draws*DRAW_VALUE + player_stats->losses*LOSS_VALUE;
    }
    else
    {
        return STATS_NOT_CALCULATED;
    }
}

static Winner statsCompareLosses(PlayerStats first_stats,PlayerStats second_stats)
{
        int first_losses = first_stats->losses;
        int second_losses = second_stats->losses;
        if(first_losses > second_losses)
        {
            return FIRST_PLAYER;
        }
        else if(first_losses < second_losses)
        {
            return SECOND_PLAYER;
        }

        return DRAW;
}
static Winner statsCompareWins(PlayerStats first_stats,PlayerStats second_stats)
{
        int first_wins = first_stats->wins;
        int second_wins = second_stats->wins;
        if(first_wins > second_wins)
        {
            return FIRST_PLAYER;
        }
        else if(first_wins < second_wins)
        {
            return SECOND_PLAYER;
        }

        return DRAW;
}

static Winner statsCompareTournamentRank(PlayerStats first_stats,PlayerStats second_stats)
{
    int first_tournament_rank = statsGetTournamentPoints(first_stats);
    int second_tournament_rank = statsGetTournamentPoints(second_stats);
    if(first_tournament_rank > second_tournament_rank)
    {
        return FIRST_PLAYER;
    }
    else if(first_tournament_rank < second_tournament_rank)
    {
        return SECOND_PLAYER;
    }

    return DRAW;
}

static Winner statsFlipWinner(Winner winner)
{
    switch (winner)
    {
    case FIRST_PLAYER:
        return SECOND_PLAYER;
        break;
    case SECOND_PLAYER:
        return FIRST_PLAYER;
        break;
    default:
        return DRAW;
        break;
    }
    return DRAW;
}






