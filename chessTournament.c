#include "./chessTournament.h"
#include "./chessSystem.h"
#include "./chessReturnsMacros.h"
#include "./chessPlayerStats.h"
#include "./chessGame.h"
#include "./chessId.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define UPPER_CASE_MIN 65
#define UPPER_CASE_MAX 90
#define LOWER_CASE_MIN 97
#define LOWER_CASE_MAX 122
#define SPACE_ASCII_VALUE 32

/* ----------------------------------------------------------------------

                         Data structs defenitions

------------------------------------------------------------------------*/
/** Struct used for pointing at a chess-tournament's details
 * @param location - holds the location of the tournament[string](char*)
 * @param tournament_games - holds the generic data type structure of the tournament's gamees(Map)
 * @param winnerId - holds the ID of the tournament's winner(int)
 */
struct tournament_t
{
    bool tournament_ended;
    char *location;
    int winnerId;
    int max_games_per_player;
    int longest_game;
    int total_game_time;
    int total_players;
    Map tournament_games;
    Map tournamnt_players_stats;
};

/* ----------------------------------------------------------------------

                        internal code functions headers

------------------------------------------------------------------------*/
/**
  * @brief Checks is the first letter of a given string is an upper case
  * 
  * @param str 
  * @return true 
  * @return false 
  */
static bool isFirstLetterUpper(const char *str);

/**
  * @brief Checks is the first letter of a given string is a lower case
  * 
  * @param str 
  * @return true 
  * @return false 
  */
static bool isFirstLetterLowerOrSpace(const char *str);
/**
  * @brief Adds to both given players game time stats in a tournament
  * 
  * @param tournament 
  * @param first_player 
  * @param second_player 
  * @param play_time 
  */
static void tournamentUpdateBothPlayersGameTime(ChessTournament tournament, int first_player, int second_player, int play_time);

/**
  * @brief Adds to a player game time stats in a tournament
  * 
  * @param tournament 
  * @param player_id 
  * @param time 
  */
static void tournamentUpdatePlayerGameTime(ChessTournament tournament, int player_id, int time);

/**
  * @brief Adds a valid game to a tournament(assumes valid values)
  * 
  * @param tournament 
  * @param first_player 
  * @param second_player 
  * @param winner 
  * @param play_time 
  * @return 
  * CHESS_NULL_ARGUMENT
  * CHESS_OUT_OF_MEMORY
  * CHESS_SUCCESS
  */
static ChessResult tournamentAddValidGame(ChessTournament tournament,
                                   int first_player, int second_player, Winner winner, int play_time);
static char *copyLocation(const char *original_location);

/**
  * @brief Checks if both players have not reached the game limit
  * 
  * @param tournament 
  * @param player_id1 
  * @param player_id2 
  * @return ChessResult 
  */
static ChessResult checkBothPlayersNotPlayMaxGames(ChessTournament tournament, int player_id1, int player_id2);

/**
   * @brief Sets the tournament winner(depends on the overall stats)
   * 
   * @param tournament 
   * @return ChessResult 
   */
static ChessResult tournamentSetWinner(ChessTournament tournament);




/**
 * @brief Removes a given player's stats from a given tournament
 * 
 * @param tournament 
 * @param player_id 
 */
void tournamentRemovePlayerStats(ChessTournament tournament, int player_id);
/**
 * @brief Adds both players to the tournament stats list(if he is not there yet)
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 */
void tournamentCheckAddBothPlayersToStats(ChessTournament tournament, int first_player, int second_player);
/**
 * @brief Adds a player to the tournament stats list(if he is not there yet)
 * 
 * @param tournament 
 * @param player 
 */
void tournamentCheckAddPlayerStats(ChessTournament tournament, int player);

/**
 * @brief Checks if a given player have reached his max agme limit
 * 
 * @param tournament 
 * @param player_id 
 * @return ChessResult 
 */
static ChessResult checkPlayerNotPlayMaxGames(ChessTournament tournament, int player_id);

/**
 * @brief Returns a given player's stats from a given tournament
 * 
 * @param tournament 
 * @param player_id 
 * @return PlayerStats 
 */
static PlayerStats tournamentGetPlayerStats(ChessTournament tournament, int player_id);

/**
 * @brief Adds a draw to a player's stats in a given tournament
 * 
 * @param tournament 
 * @param player_id 
 */
static void tournamentAddDrawToPlayerStats(ChessTournament tournament, int player_id);

/**
 * @brief Adds a draw to both player's stats in a given tournament
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 */
static void tournamentAddDrawToBothPlayerStats(ChessTournament tournament, int first_player, int second_player);

/**
 * @brief Adds a loss to a player's stats in a given tournament
 * 
 * @param tournament 
 * @param player_id 
 */
static void tournamentAddLossToPlayerStats(ChessTournament tournament, int player_id);

/**
 * @brief Adds a win to a player's stats in a given tournament
 * 
 * @param tournament 
 * @param player_id 
 */
static void tournamentAddWinToPlayerStats(ChessTournament tournament, int player_id);
/**
 * @brief Updates given player's stats based on given match variables
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @param winner 
 * @param play_time 
 */
static void tournamentUpdatePlayersStats(ChessTournament tournament, int first_player, int second_player, Winner winner, int play_time);
/**
 * @brief Updates given player's result stats based on given match variables
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @param winner 
 */
static void tournamentUpdateGameResultToStats(ChessTournament tournament, int first_player, int second_player, Winner winner);
/**
 * @brief Adds to both players total play time stats in a given tournament
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @param game_time 
 */
void tournamentAddBothPlayersGameTime(ChessTournament tournament, int first_player, int second_player, int game_time);
/**
 * @brief Checks if both of the players have not matched yet in a given tournament
 * 
 * @param tournament 
 * @param first_player 
 * @param second_player 
 * @return true 
 * @return false 
 */
bool checkBothPlayersDidMatch(ChessTournament tournament, int first_player, int second_player);

/* ----------------------------------------------------------------------

                 Header included functions defenitions

------------------------------------------------------------------------*/
bool tournamentCheckEnded(ChessTournament tournament)
{
    return tournament->tournament_ended;
}

ChessResult tournamentSetEnded(ChessTournament tournament)
{

    if (tournament == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (tournament->tournament_ended == true)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (mapGetSize(tournament->tournament_games) == 0)
    {
        return CHESS_NO_GAMES;
    }

    tournament->tournament_ended = true;

    tournamentSetWinner(tournament);
    return CHESS_SUCCESS;
}

ChessResult convertMapToChessResultTournament(MapResult m_result)
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
        c_result = CHESS_TOURNAMENT_ALREADY_EXISTS;
        break;
    case MAP_ITEM_DOES_NOT_EXIST:
        c_result = CHESS_TOURNAMENT_NOT_EXIST;
        break;
    default:
        c_result = CHESS_NULL_ARGUMENT;
        break;
    }

    return c_result;
}

int getPlayerTotalTimeInTournament(ChessTournament tournament, int player_id)
{
    PlayerStats stats = mapGet(tournament->tournamnt_players_stats, (MapKeyElement)&player_id);
    if (stats != NULL)
    {
        return statsGetTime(stats);
    }

    return STATS_NOT_CALCULATED;
}

int getPlayerTotalGamesInTournament(ChessTournament tournament, int player_id)
{
    PlayerStats stats = mapGet(tournament->tournamnt_players_stats, (MapKeyElement)&player_id);
    if (stats != NULL)
    {
        return statsGetTotalGames(stats);
    }

    return STATS_NOT_CALCULATED;
}

ChessTournament createTournamentValid(int max_games_per_player, const char *location)
{
    ChessTournament new_tournament = malloc(sizeof(struct tournament_t));
    if (new_tournament == NULL)
    {
        return NULL;
    }

    new_tournament->tournament_games = mapCreate((copyMapDataElements)copyGame, (copyMapKeyElements)copyIdKey,
                                                 (freeMapDataElements)freeGame, (freeMapKeyElements)freeIdKey, (compareMapKeyElements)compareIdKeys);

    new_tournament->tournamnt_players_stats = mapCreate((copyMapDataElements)copyPlayerStats, (copyMapKeyElements)copyIdKey,
                                                        (freeMapDataElements)freePlayerStats, (freeMapKeyElements)freeIdKey, (compareMapKeyElements)compareIdKeys);

    new_tournament->location = copyLocation(location);
    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->winnerId = WINNER_NOT_DECIDED;
    new_tournament->tournament_ended = false;
    new_tournament->longest_game = STATS_NOT_CALCULATED;
    new_tournament->total_game_time = 0;
    new_tournament->total_players = 0;
    new_tournament->tournament_ended = false;

    return new_tournament;
}

ChessResult isValidLocation(const char *str)
{
    if (str == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    const char *current = str;
    int length = strlen(str);
    if (!isFirstLetterUpper(str))
    {
        return CHESS_INVALID_LOCATION;
    }
    else
    {
        for (int i = 1; i < length; i++)
        {
            current++;
            if (!isFirstLetterLowerOrSpace(current))
                return CHESS_INVALID_LOCATION;
        }
    }
    return CHESS_SUCCESS;
}

void freeTournament(ChessTournament tournament)
{
    if (tournament != NULL)
    {

        mapDestroy(tournament->tournament_games);
        mapDestroy(tournament->tournamnt_players_stats);
        free(tournament->location);
        free(tournament);
    }
}


ChessTournament copyTournament(ChessTournament original_tournament)
{
    ChessTournament newTournament = malloc(sizeof(struct tournament_t));
    if (newTournament == NULL)
    {
        return NULL;
    }
    newTournament->location = copyLocation(original_tournament->location);
    newTournament->tournament_games = mapCopy(original_tournament->tournament_games);
    newTournament->winnerId = original_tournament->winnerId;
    newTournament->tournament_ended = original_tournament->tournament_ended;
    newTournament->max_games_per_player = original_tournament->max_games_per_player;
    newTournament->tournamnt_players_stats = mapCopy(original_tournament->tournamnt_players_stats);
    newTournament->longest_game = original_tournament->longest_game;
    newTournament->total_game_time = original_tournament->total_game_time;
    newTournament->total_players = original_tournament->total_players;
    return newTournament;
}

ChessResult removePlayerFromTournament(ChessTournament tournament, int player_id, Map system_player_stats)
{
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    CHESS_CHECK_NULL_RETURN(tournament);
    CHESS_CHECK_NULL_RETURN(system_player_stats);

    ChessResult current_result = CHESS_PLAYER_NOT_EXIST;
    ChessResult final_result = CHESS_PLAYER_NOT_EXIST;

    tournamentRemovePlayerStats(tournament, player_id);

    MapKeyElement current_key = mapGetFirst(tournament->tournament_games);
    ChessGame current_game = mapGet(tournament->tournament_games, current_key);
    while (current_game != NULL)
    {
        int current_game_remained_player = STATS_NOT_CALCULATED;
        PlayerPresent remained_player_result = STATS_NOT_CALCULATED;
        current_result = removePlayerFromGame(current_game, player_id, &current_game_remained_player, &remained_player_result);

        if (current_result == CHESS_SUCCESS)
        {
            final_result = CHESS_SUCCESS;
            PlayerStats remained_player_tournament_stats = tournamentGetPlayerStats(tournament, current_game_remained_player);
            PlayerStats remained_player_system_stats = (PlayerStats)mapGet(system_player_stats, &current_game_remained_player);
            if (remained_player_result == PLAYER_DRAW)
            {
                statsAddWin(remained_player_tournament_stats);
                statsRemoveDraw(remained_player_tournament_stats);
                statsAddWin(remained_player_system_stats);
                statsRemoveDraw(remained_player_system_stats);
            }
            else if (remained_player_result == PLAYER_LOST)
            {
                statsAddWin(remained_player_tournament_stats);
                statsRemoveLoss(remained_player_tournament_stats);
                statsAddWin(remained_player_system_stats);
                statsRemoveLoss(remained_player_system_stats);
            }
        }
        freeIdKey(current_key);
        current_key = mapGetNext(tournament->tournament_games);
        current_game = mapGet(tournament->tournament_games, current_key);
    }
    freeGame(current_game);
    return final_result;
}

ChessResult tournamentAddGame(ChessTournament tournament, int first_player, int second_player,
                              Winner winner, int play_time)
{

    CHESS_CHECK_TOURNAMENT_NOT_ENDED_RETURN(tournament);

    CHESS_CHECK_PLAYERS_NOT_MATCHED_RETURN(tournament, first_player, second_player);

    ChessResult variable_check = checkValidPlayTime(play_time);
    CHESS_CHECK_NOT_SUCCESS_RETURN(variable_check);

    variable_check = checkBothPlayersNotPlayMaxGames(tournament, first_player, second_player);
    CHESS_CHECK_NOT_SUCCESS_RETURN(variable_check);

    ChessResult add_result = tournamentAddValidGame(tournament, first_player, second_player, winner, play_time);
    CHESS_CHECK_NOT_SUCCESS_RETURN(add_result);

    tournamentUpdatePlayersStats(tournament, first_player, second_player, winner, play_time);
    return CHESS_SUCCESS;
}

MapKeyElement tournamentGetFirstPlayerKeyCopy(ChessTournament tournament)
{
    CHECK_NULL_RETURN(tournament);
    CHECK_NULL_RETURN(tournament->tournamnt_players_stats);

    MapKeyElement first_key = mapGetFirst(tournament->tournamnt_players_stats);

    return first_key;
}

MapKeyElement tournamentGetNextPlayerKeyCopy(ChessTournament tournament)
{
    CHECK_NULL_RETURN(tournament);
    CHECK_NULL_RETURN(tournament->tournamnt_players_stats);

    MapKeyElement next_key = mapGetNext(tournament->tournamnt_players_stats);
    return next_key;
}

PlayerStats tournamentGetPlayerStatsCopy(ChessTournament tournament, MapKeyElement player_id)
{
    CHECK_NULL_RETURN(tournament);
    CHECK_NULL_RETURN(tournament->tournamnt_players_stats);
    CHECK_NULL_RETURN(player_id);
    PlayerStats original = mapGet(tournament->tournamnt_players_stats, player_id);
    return copyPlayerStats(original);
}

int tournamentGetWinnerId(ChessTournament tournament)
{
    CHECK_NULL_RETURN_NOT_CALCULATED(tournament);
    return tournament->winnerId;
}
int tournamentGetLongestGame(ChessTournament tournament)
{
    CHECK_NULL_RETURN_NOT_CALCULATED(tournament);
    return tournament->longest_game;
}

int tournamentGetTotalPlayTime(ChessTournament tournament)
{
    CHECK_NULL_RETURN_NOT_CALCULATED(tournament);
    return tournament->total_game_time;
}

int tournamentGetPlayersAmount(ChessTournament tournament)
{
    CHECK_NULL_RETURN_NOT_CALCULATED(tournament);
    return mapGetSize(tournament->tournamnt_players_stats);
}

int tournamentGetGamesAmount(ChessTournament tournament)
{
    CHECK_NULL_RETURN_NOT_CALCULATED(tournament);
    return mapGetSize(tournament->tournament_games);
}

char *tournamentGetLocationCopy(ChessTournament tournament)
{
    CHECK_NULL_RETURN(tournament);
    return copyLocation(tournament->location);
}

ChessResult checkValidTournamentId(int id)
{
    ChessResult result;
    (id > 0) ? (result = CHESS_SUCCESS) : (result = CHESS_INVALID_ID);
    return result;
}

ChessResult checkValidMaxGames(int max_games)
{
    ChessResult result;
    (max_games > 0) ? (result = CHESS_SUCCESS) : (result = CHESS_INVALID_MAX_GAMES);
    return result;
}
/* ----------------------------------------------------------------------

                Non header included functions defenitions

------------------------------------------------------------------------*/








static char *copyLocation(const char *original_location)
{
    if (original_location == NULL)
    {
        return NULL;
    }
    int length = strlen(original_location);
    char *new_location = (char *)malloc((sizeof(char) * (length + 1)));
    if (new_location == NULL)
    {
        return NULL;
    }
    strcpy(new_location, original_location);
    return new_location;
}



static bool isFirstLetterUpper(const char *str)
{
    int firstLetter = (int)(*(str));
    return (firstLetter >= UPPER_CASE_MIN) && (firstLetter <= UPPER_CASE_MAX);
}

static bool isFirstLetterLowerOrSpace(const char *str)
{
    int firstLetter = (int)(*(str));
    return (firstLetter == SPACE_ASCII_VALUE) || ((firstLetter >= LOWER_CASE_MIN) && (firstLetter <= LOWER_CASE_MAX));
}

static ChessResult tournamentAddValidGame(ChessTournament tournament,
                                   int first_player, int second_player, Winner winner, int play_time)
{

    ChessGame new_game = gameCreate(first_player, second_player, play_time, winner);
    int key = mapGetSize(tournament->tournament_games);
    MapResult add_result = mapPut((Map)tournament->tournament_games, (MapKeyElement)&key, (MapDataElement)new_game);
    freeGame(new_game);
    tournament->total_game_time += play_time;
    tournamentUpdateBothPlayersGameTime(tournament, first_player, second_player, play_time);
    return convertMapToChessResultTournament(add_result);
}

static void tournamentUpdateBothPlayersGameTime(ChessTournament tournament, int first_player, int second_player, int play_time)
{
    tournamentUpdatePlayerGameTime(tournament, first_player, play_time);
    tournamentUpdatePlayerGameTime(tournament, second_player, play_time);

    if (play_time > tournament->longest_game)
    {
        tournament->longest_game = play_time;
    }
}

static void tournamentUpdatePlayerGameTime(ChessTournament tournament, int player_id, int time)
{
    PlayerStats stats = mapGet(tournament->tournamnt_players_stats, &player_id);
    if (stats != NULL)
    {
        statsAddTime(stats, time);
    }
}


static ChessResult checkBothPlayersNotPlayMaxGames(ChessTournament tournament, int player_id1, int player_id2)
{
    ChessResult check_valid = checkPlayerNotPlayMaxGames(tournament, player_id1);

    if (check_valid != CHESS_SUCCESS)
    {
        return check_valid;
    }
    check_valid = checkPlayerNotPlayMaxGames(tournament, player_id2);
    if (check_valid != CHESS_SUCCESS)
    {
        return check_valid;
    }

    return CHESS_SUCCESS;
}

static ChessResult checkPlayerNotPlayMaxGames(ChessTournament tournament, int player_id)
{
    if (tournament == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if (mapGetSize(tournament->tournament_games) == 0)
    {
        return CHESS_SUCCESS;
    }

    int counter = 0;
    int max_games = tournament->max_games_per_player;

    MapKeyElement current_key = mapGetFirst(tournament->tournament_games);
    ChessGame current_game = mapGet(tournament->tournament_games, current_key);

    while (current_game != NULL && counter < max_games)
    {

        if (playerIsInGame(current_game, player_id) != PLAYER_NOT_IN_GAME)
        {
            counter++;
        }
        freeIdKey(current_key);
        current_key = mapGetNext(tournament->tournament_games);
        current_game = mapGet(tournament->tournament_games, current_key);
    }
    freeIdKey(current_key);

    if (counter >= max_games)
    {
        return CHESS_EXCEEDED_GAMES;
    }
    else
    {
        return CHESS_SUCCESS;
    }
}



bool checkBothPlayersDidMatch(ChessTournament tournament, int first_player, int second_player)
{
    CHESS_CHECK_NULL_RETURN(tournament);
    MapKeyElement current_key = mapGetFirst(tournament->tournament_games);

    ChessGame current_game = mapGet(tournament->tournament_games, current_key);
    while (current_key != NULL)
    {
        freeIdKey(current_key);
        CHECK_TRUE_RETURN(checkBothPlayersInGame(current_game, first_player, second_player));
        current_key = mapGetNext(tournament->tournament_games);
        current_game = mapGet(tournament->tournament_games, current_key);
    }

    return false;
}



static ChessResult tournamentSetWinner(ChessTournament tournament)
{
    CHESS_CHECK_NULL_RETURN(tournament);

    Map tournament_stats = tournament->tournamnt_players_stats;

    MapKeyElement current_key = mapGetFirst(tournament_stats);
    int winner_id = *(int *)(current_key);

    PlayerStats current_stats = mapGet(tournament_stats, current_key);
    PlayerStats max_stats = createBlankPlayerStats();
    if (current_key == NULL)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    while (current_key != NULL)
    {
        if (statsTournamentCompareHigher(current_stats, max_stats) == FIRST_PLAYER)
        {
            free(max_stats);
            max_stats = copyPlayerStats(current_stats);

            winner_id = *(int *)current_key;
        }
        free(current_key);
        current_key = mapGetNext(tournament_stats);
        current_stats = mapGet(tournament_stats, current_key);
    }

    tournament->winnerId = winner_id;
    free(max_stats);

    return CHESS_SUCCESS;
}

static void tournamentUpdatePlayersStats(ChessTournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
    tournamentCheckAddBothPlayersToStats(tournament, first_player, second_player);
    tournamentUpdateGameResultToStats(tournament, first_player, second_player, winner);
    tournamentUpdateBothPlayersGameTime(tournament, first_player, second_player, play_time);
}

static void tournamentUpdateGameResultToStats(ChessTournament tournament, int first_player, int second_player, Winner winner)
{
    int winner_id = statsGetWinnerId(first_player, second_player, winner);
    int loser_id = statsGetLoserId(first_player, second_player, winner);
    if (winner_id != STATS_NOT_CALCULATED)
    {
        tournamentAddWinToPlayerStats(tournament, winner_id);
        tournamentAddLossToPlayerStats(tournament, loser_id);
    }
    else
    {
        tournamentAddDrawToBothPlayerStats(tournament, first_player, second_player);
    }
}

static void tournamentAddWinToPlayerStats(ChessTournament tournament, int player_id)
{
    CHECK_NULL_VOID(tournament);
    PlayerStats player_stats = tournamentGetPlayerStats(tournament, player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddWin(player_stats);
}

static void tournamentAddLossToPlayerStats(ChessTournament tournament, int player_id)
{
    CHECK_NULL_VOID(tournament);
    PlayerStats player_stats = tournamentGetPlayerStats(tournament, player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddLoss(player_stats);
}

static void tournamentAddDrawToBothPlayerStats(ChessTournament tournament, int first_player, int second_player)
{
    tournamentAddDrawToPlayerStats(tournament, first_player);
    tournamentAddDrawToPlayerStats(tournament, second_player);
}

static void tournamentAddDrawToPlayerStats(ChessTournament tournament, int player_id)
{
    CHECK_NULL_VOID(tournament);
    PlayerStats player_stats = tournamentGetPlayerStats(tournament, player_id);
    CHECK_NULL_VOID(player_stats);
    statsAddDraw(player_stats);
}

static PlayerStats tournamentGetPlayerStats(ChessTournament tournament, int player_id)
{
    CHECK_NULL_RETURN(tournament);
    return mapGet(tournament->tournamnt_players_stats, &player_id);
}

void tournamentCheckAddBothPlayersToStats(ChessTournament tournament, int first_player, int second_player)
{
    tournamentCheckAddPlayerStats(tournament, first_player);
    tournamentCheckAddPlayerStats(tournament, second_player);
}

void tournamentCheckAddPlayerStats(ChessTournament tournament, int player)
{

    if (player == PLAYER_REMOVED)
    {
        return;
    }
    if (!mapContains(tournament->tournamnt_players_stats, (MapKeyElement)(&player)))
    {
        PlayerStats new_stats = createBlankPlayerStats();
        zeroPlayerStats(new_stats);
        mapPut(tournament->tournamnt_players_stats, (MapKeyElement)&player, (MapDataElement)new_stats);
        tournament->total_players++;
        free(new_stats);
    }
}

void tournamentAddPlayerGameTime(ChessTournament tournament, int player_id, int game_time)
{
    PlayerStats stats = tournamentGetPlayerStats(tournament, player_id);
    CHECK_NULL_VOID(stats);
    statsAddTime(stats, game_time);
}

void tournamentAddBothPlayersGameTime(ChessTournament tournament, int first_player, int second_player, int game_time)
{
    tournamentAddPlayerGameTime(tournament, first_player, game_time);
    tournamentAddPlayerGameTime(tournament, second_player, game_time);
}






void tournamentRemovePlayerStats(ChessTournament tournament, int player_id)
{
    if (mapRemove(tournament->tournamnt_players_stats, &player_id) == MAP_SUCCESS)
    {
        tournament->total_players--;
    }
}






