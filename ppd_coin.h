/*########################################################################
# COSC1076 - Advanced Programming Techniques
# Semester 2 2016 Assignment #2
# * Full Name        : Peng Gao
# * Student Number   : s3457558
# * Course Code      : COSC1076
# * Program Code     : BP162
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_shared.h"

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN

struct ppd_system;

/**
 * The number of denominations of currency available in the system
 **/
#define NUM_DENOMS 8

#define COIN_DELIM ","

#define DOLLAR_MAX_LEN 4

#define DOLLAR_MAX_NUM_LEN 4

#define MONEY_MAX_LINE_LEN DOLLAR_MAX_LEN + 1 + DOLLAR_MAX_NUM_LEN

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
	FIVE_CENTS,
	TEN_CENTS,
	TWENTY_CENTS,
	FIFTY_CENTS,
	ONE_DOLLAR,
	TWO_DOLLARS,
	FIVE_DOLLARS,
	TEN_DOLLARS,
	OTHER_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
	/**
	 * the denomination type
	 **/
	enum denomination denom;
	/**
	 * the count of how many of these are in the cash register
	 **/
	unsigned count;
};

/**
 * this function to check the valid of denomination user input
 * */
BOOLEAN check_denomination(const int *cents);

/**
 * this function to check the valid of money number
 * for example there are 20 500 cents
 * */
BOOLEAN check_denomination_count(const int *deno_count);

/**
 * create a new coin
 */
struct coin * create_coin(enum denomination denom, int count);

/**
 * convert a line to a coin
 */
struct coin * line_to_coin(char *line_char);

/**
 * insert into system cash_register array
 */
BOOLEAN insert_coin(struct ppd_system* ppd, struct coin* strCoin,
		unsigned index);

/**
 * save the coins to the coins file
 **/
BOOLEAN save_coins(struct ppd_system *ppd);

/*
 * return a enum denomination for a money
 */
enum denomination getDenoType(unsigned money);

/**
 *
 */
char * getDenoCharFromDeno(enum denomination deno);

/**
 * get Denomination Full name which can display in the coin list
 */
char * getDenominationFullchar(enum denomination deno);

/**
 * Finding the coins denomnation which is in the coin List
 * return the position of the denomination in the coin array
 */
int find_index_of_denomination(const struct coin *cash_register,
		enum denomination denom);

/**
 * add denomination of system cash_register by a pArray_coin array;
 */
BOOLEAN add_denomination(struct ppd_system *ppd, struct coin *pArray_coin);

/**
 * reduce the mount of denomination ,
 * which is in cash_register by a pArray_coin array;
 */
BOOLEAN del_denomination(struct ppd_system *ppd, struct coin *pArray_coin);

/**
 * convert money to a struct coin
 */
BOOLEAN calculte_denominations(int *arg_cents, struct coin *arg_coin,
		const int max_money, const struct coin *cash_register);

/**
 * convert money to array of struct coin
 * for change
 */
BOOLEAN calculte_coins(int *arg_cents, struct coin *arg_coin,
		const struct coin *cash_register);

/**
 * reset Coin Counter
 */
BOOLEAN reset_coin_list(struct ppd_system *ppd);

/*
void free_coin(struct coin *my_coin);

void free_coin_list(struct ppd_system *ppd);
*/

#endif
