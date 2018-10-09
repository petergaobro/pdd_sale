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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#include "ppd_shared.h"

#ifndef PPD_STOCK
#define PPD_STOCK

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The default stock level that all new stock should start at and that
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20

/**
 * the maximum length of a price as a string
 **/
#define PRICELEN 5

/**
 * the maximum length of onhand items as a String
 **/
#define ONHAND_LEN 2

/**
 * the number of line separate char
 **/
#define SEPARATE_CHAR_NUM 4

/**
 * the maximum length of stock line
 **/
#define STOCK_MAX_LINE IDLEN + NAMELEN + DESCLEN + PRICELEN \
	+ ONHAND_LEN + SEPARATE_CHAR_NUM

/**
 * the maximum number onhand value
 **/
#define MAX_ONHAND 99

/**
 * the separate char
 **/
#define DELIM "|"

/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price
{
	/**
	 * the dollar value for some price
	 **/
	unsigned dollars,
	/**
	 * the cents value for some price
	 **/
	cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
	/**
	 * the unique id for this item
	 **/
	char id[IDLEN + 1];
	/**
	 * the name of this item
	 **/
	char name[NAMELEN + 1];
	/**
	 * the description of this item
	 **/
	char desc[DESCLEN + 1];
	/**
	 * the price of this item
	 **/
	struct price price;
	/**
	 * how many of this item do we have on hand?
	 **/
	unsigned on_hand;

};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
	/* pointer to the data held for the node */
	struct ppd_stock * data;
	/* pointer to the next node in the list */
	struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
	/**
	 * the beginning of the list
	 **/
	struct ppd_node * head;
	/**
	 * how many nodes are there in the list?
	 **/
	unsigned count;
};

/**
 * this is the header structure for all the datatypes that is
 * passed around and manipulated
 **/
struct ppd_system
{
	/**
	 * the container for all the money manipulated by the system
	 **/
	struct coin cash_register[NUM_DENOMS];

	/**
	 * the linked list - note that this is a pointer - how does that
	 * change what we need to do for initialization of the list?
	 **/
	struct ppd_list * item_list;

	/**
	 * the name of the coin file - we need this for saving as all menu
	 * items only take the one parameter of the ppd_system
	 **/
	const char * coin_file_name;
	/**
	 * the name of the stock file
	 **/
	const char * stock_file_name;

	/* are the coins loaded in from a file ? */
	BOOLEAN coin_from_file;
};
/*----------------------------------------------------*/
/* initialize the coin list */
void init_coin_list(struct coin *cash_register);

/* initialise the stock list */
BOOLEAN init_stock_list(struct ppd_list ** stockList);

/**
 * validating the ID of item
 **/
BOOLEAN check_item_id(const char * itemID);

/**
 * validating the name of item
 **/
BOOLEAN check_item_name(const char * itemName);

/**
 * validating the describe of item
 **/
BOOLEAN check_item_dec(const char * itemDec);

/**
 * validating the price of item
 * convert a char price to a struct price
 **/
BOOLEAN check_item_price(const char * char_price, struct price * itemPrice);

/**
 * validating the itemOnHand
 **/
BOOLEAN check_item_OnHand(const char * itemOnHand, unsigned int * itemHandNumber);

/**
 * convert read line to a new stock Item
 **/
struct ppd_stock * line_to_stockItem(char * stockLine);

/**
 * create a new stock
 * parameters are all fields
 */
struct ppd_stock * create_stock(const char * sId, const char * sName,
		const char * sDesc, const struct price sPrice, const unsigned sHand);

/**
 * save the stock list to the stock file
 **/
BOOLEAN save_stock(struct ppd_system *ppd);

/*
 * if there is no stock then return NULL
 */
struct ppd_node * find_node_byId(const char * sId,
		const struct ppd_list* itemList);

/*
 * if there is no stock then return NULL
 * */
struct ppd_stock * find_stock_byId(const char * sId
		, const struct ppd_list * itemList);

/**
 * find the stock by its name
 * if there is no stock then return NULL
 */
struct ppd_stock * find_stock_byName(const char * sName,
		const struct ppd_list* itemList);

/**
 * insert a new stock into itemList
 */
BOOLEAN insert_stock(struct ppd_stock * newstock
		, struct ppd_list * itemList);

/**
 * free the memory of the item list
 */
void free_stock_list(struct ppd_list* itemList);

/*
 * free every node
*/
void free_node(struct ppd_node* node);

/**
 * free a stock
 */
void free_stock(struct ppd_stock* stock);

/*
 * generate new item id
 * */
BOOLEAN generateStockId(struct ppd_system *ppd,char *newId);

/**
 * a new stock name
 */
BOOLEAN acceptStockName(struct ppd_system *ppd,char *newName);

/**
 * a new stock description
 */
BOOLEAN acceptStockDesc(struct ppd_system *ppd, char *newDesc);

/**
 * the price of a new item
 */
BOOLEAN acceptStockPrice(struct ppd_system *ppd, struct price *newPrice);

/**
 * delete a item by item id
 */
BOOLEAN deleteItemById(struct ppd_system *ppd, char * itemId);

/**
 * reset Stock count to default value
 */
BOOLEAN resetOnHand(struct ppd_system *ppd);


#endif

