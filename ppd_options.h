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
#include "ppd_main.h"

/**
 * @file ppd_options.h declares all functions to implement the menu
 * options that can be called from the main menu. 
 **/
#define EXTRA_LEN_COLUM 1

/**
 * This option allows the user to request a display of the items 
 * available for purchase. This is the data loaded into the linked 
 * list in the requirement 2.  
 **/
BOOLEAN display_items(struct ppd_system *);

/** This option allows the user to purchase an item from the menu.
 * This function is called from the main menu when the user has
 * finally decided to purchase an item. This function implements 
 * requirement 5 of the assignment specification. 
 **/
BOOLEAN purchase_item(struct ppd_system *);

/** 
 * You must save all data to the data files that were provided on the
 * command line when the program loaded up. This function implements 
 * requirement 6 of the assignment specification.
 **/

BOOLEAN save_system(struct ppd_system *);

/**
 * This option adds an item to the system. This function implements 
 * requirement 7 of of assignment specification. 
 **/
BOOLEAN add_item(struct ppd_system *);

/**
 * Remove an item from a category and delete it from the system,
 * including free memory that is no longer being used. 
 * This function implements requirement 8 of the assignment 
 * specification
 **/
BOOLEAN remove_item(struct ppd_system *);

/**
 * This option will require you to iterate over every stock in the
 * list and set its 'on hand' count to the default value specified in
 * the startup code. 
 * This function implements requirement 9 of the assignment 
 * specification
 **/
BOOLEAN reset_stock(struct ppd_system *);

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code. 
 * This requirement implements part 3 of requirement 18 in the 
 * assignment specifications.
 **/
BOOLEAN reset_coins(struct ppd_system *);

/**
 * This option will require you to display the coins as follows. In
 * particular, the counts of coins should be correctly aligned, and
 * they should be sorted from lowest to highest value: 
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
BOOLEAN display_coins(struct ppd_system *);

/**
 * abort_system for administrator
 */
BOOLEAN abort_system(struct ppd_system *);

/**
 * get the field's max length of item's name
 */
unsigned max_name_length(struct ppd_list * item_list);

/**
 * get all fields' max length of item
 */
BOOLEAN max_column_length(struct ppd_system *ppd, unsigned *l_ID,
		unsigned *l_Name, unsigned *l_OnHand, unsigned *l_Price);

/**
 * valid the item's id
 */
struct ppd_stock * check_input_item_id(char *input_itemId,
		struct ppd_list * item_list);

/**
 * valid input the money whether it is correct
 */
BOOLEAN check_input_item_money(struct ppd_stock *stock,
		struct coin *arr_in_Coins, int *val_out_Cents);
/**
 * check the transaction if it is valid
 */
BOOLEAN check_transaction(int arg_out_cents, struct coin *arg_out_coins);

/**
 * excute update system
 */
BOOLEAN update_transaction(struct ppd_system *system,
		struct ppd_stock *stock, int arg_out_cents,
		struct coin *arg_in_coins, struct coin *arg_out_coins);

