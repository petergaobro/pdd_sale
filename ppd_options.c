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

#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * Requirement #4
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{

	struct ppd_node * currentNode;
	struct ppd_stock * currentData;

	unsigned lenLine = 0;
	unsigned lenID = IDLEN;
	unsigned lenName = 0;
	unsigned lenAvailable = strlen("Available");
	unsigned lenPrice = PRICELEN;

	max_column_length(system, &lenID, &lenName, &lenAvailable, &lenPrice);

	lenLine = lenID + lenName + lenAvailable + lenPrice + 8;

	currentNode = system->item_list->head;

	printf("\n%-s\n\n", "Items Menu");
	printf("%-*s| ", lenID, "ID");
	printf("%-*s| ", lenName, "Name");
	printf("%-*s| ", lenAvailable, "Available");
	printf("%-*s", lenPrice, "Price");
	printf("\n");
	print_line(lenLine, '-');

	do
	{
		currentData = currentNode->data;
		printf("%-*s| ", lenID, currentData->id);
		printf("%-*s| ", lenName, currentData->name);
		printf("%-*d| ", lenAvailable, currentData->on_hand);
		printf("$%2d.%02d", currentData->price.dollars, currentData->price.cents);
		printf("\n");
		currentNode = currentNode->next;
	} while (currentNode != NULL);

	print_line(lenLine, '-');
	printf("\n");

	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
	char input_itemId[IDLEN + EXTRA_CHARS];
	unsigned len_Itemid;
	struct ppd_list *dataList = NULL;
	struct ppd_stock *stock = NULL;
	char * title = "Purchase Item";
	struct coin arr_in_Coins[NUM_DENOMS];/* user give money */
	int val_out_Cents = 0;/* the change money of user */
	struct coin arr_ou_Coins[NUM_DENOMS];/* the change money of user */
	int i = 0;
	/*initialize the arr_in_Coins and arr_ou_Coins */
	for (i = 0; i < NUM_DENOMS; i++)
	{
		struct coin new_coin;
		new_coin.denom = i;
		new_coin.count = 0;
		arr_in_Coins[i] = new_coin;
		arr_ou_Coins[i] = new_coin;
	}
	/*print the title */
	printf("%s\n", title);
	print_line(strlen(title), '-');
	dataList = system->item_list;
	/* begin purchase */
	while (1 == 1)
	{
		printf("Please enter the ID of the item you wish to purchase:");
		if (fgets(input_itemId, IDLEN + EXTRA_CHARS, stdin) == NULL)
		{
			return FALSE;
		}
		else
		{
			if (input_itemId[0] == '\n')
			{
				return FALSE;
			}
			len_Itemid = strlen(input_itemId);
			if (input_itemId[len_Itemid - 1] != '\n')
			{
				read_rest_of_line();
			}
			else
			{
				input_itemId[len_Itemid - 1] = '\0';
			}
		}
		/* check item whether it is valid */
		stock = check_input_item_id(input_itemId, dataList);
		if (stock == NULL)
		{
			continue;
		}
		/* check input money */
		if (check_input_item_money(stock, arr_in_Coins, &val_out_Cents)
				== FALSE)
		{
			return FALSE;
		}
		break;
	}
	calculte_coins(&val_out_Cents, arr_ou_Coins, system->cash_register);
	if (check_transaction(val_out_Cents, arr_ou_Coins) == FALSE)
	{
		return FALSE;
	}
	update_transaction(system, stock, val_out_Cents, arr_in_Coins,
			arr_ou_Coins);
	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
	/* save stock.data*/
	save_stock(system);
	/* save coin.data*/
	save_coins(system);
	/* free ppd system */
	system_free(system);
	printf("Stocks and Coins have been saved to the file."
			" You have exit the system.\n");
	exit(EXIT_SUCCESS);
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
	char newId[IDLEN];
	char *newName, *newDesc;
	struct price newPrice;
	struct ppd_stock *newStock;
	newName = malloc(NAMELEN + EXTRA_CHARS);
	if (!newName)
	{
		perror("Failed to allocate memory");
		return FALSE;
	}

	newDesc = malloc(DESCLEN + EXTRA_CHARS);
	if (!newDesc)
	{
		perror("Failed to allocate memory");
		return FALSE;
	}

	if (generateStockId(system, newId) == FALSE)
	{
		return FALSE;
	}
	else
	{
		printf("This new meal item will have the Item id of %s.\n", newId);
	}

	if (acceptStockName(system, newName) == FALSE)
	{
		return FALSE;
	}
	if (acceptStockDesc(system, newDesc) == FALSE)
	{
		return FALSE;
	}
	if (acceptStockPrice(system, &newPrice) == FALSE)
	{
		return FALSE;
	}
	newStock = create_stock(newId, newName, newDesc, newPrice,
	DEFAULT_STOCK_LEVEL);
	free(newName);
	free(newDesc);

	if (newStock == NULL)
	{
		printf("Error: create a new stock.\n");
		return FALSE;
	}
	if (insert_stock(newStock, system->item_list) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
	char input_itemId[IDLEN + EXTRA_CHARS];
	unsigned len_Itemid;
	/*===============================================*/
	while (1 == 1)
	{
		printf("Enter the item id of the item to remove form the menu:");
		if (fgets(input_itemId, IDLEN + EXTRA_CHARS, stdin) == NULL)
		{
			return FALSE;
		}
		else
		{
			if (input_itemId[0] == '\n')
			{
				return FALSE;
			}
			len_Itemid = strlen(input_itemId);
			if (input_itemId[len_Itemid - 1] != '\n')
			{
				read_rest_of_line();
			}
			else
			{
				input_itemId[len_Itemid - 1] = '\0';
			}
		}
		if (check_item_id(input_itemId) == FALSE)
		{
			continue;
		}

		if (deleteItemById(system, input_itemId) == FALSE)
		{
			printf("There is no the item '%s'.\n", input_itemId);
			return FALSE;
		}
		else
		{
			break;
		}
	}
	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
	return resetOnHand(system);
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
	reset_coin_list(system);
	return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
	/*struct coin loc_cash_register[NUM_DENOMS];*/

	int i = 0;
	int a = 0,b = 0;
	printf("Coins Summary\n");
	printf("---------------------\n");
	printf("Denomination |  Count\n");
	a = strlen("Denomination") + 1;
	b = strlen("Count") + 1;

	for (i = (NUM_DENOMS - 1); i >= 0; i--)
	{
		char *denomName = getDenominationFullchar(system->cash_register[i].denom);
		printf("%-*s| ", a, denomName);
		printf(" %-*d", b, system->cash_register[i].count);
		printf("\n");
	}

	return TRUE;
}

/**
 * user quit the system
 */
BOOLEAN abort_system(struct ppd_system *system)
{
	/* free system memory */
	system_free(system);
	printf("You have exit the system.\n");
	exit(EXIT_SUCCESS);
}

/**
 * get max length of item in all item list
 */
unsigned max_name_length(struct ppd_list * item_list)
{
	unsigned retValue = 0;
	struct ppd_node * current;
	struct ppd_stock * currentData;
	current = item_list->head;
	do
	{
		unsigned lenTemp = 0;
		currentData = current->data;
		/* name length */
		lenTemp = strlen(currentData->name);
		if (lenTemp > retValue)
		{
			retValue = lenTemp;
		}
		current = current->next;
	} while (current != NULL);
	return retValue;
}

/**
 * calculate every column's maximal length
 */
BOOLEAN max_column_length(struct ppd_system *ppd, unsigned *l_ID,
		unsigned *l_Name, unsigned *l_OnHand, unsigned *l_Price)
{
	struct ppd_node * currentNode;
	struct ppd_stock * currentData;
	unsigned lenDollars;
	unsigned lenCents;
	unsigned lenTemp = 0;
	currentNode = ppd->item_list->head;
	if (currentNode == NULL)
	{
		printf("Error:currentNode is null!");
		return FALSE;
	}
	while (currentNode != NULL)
	{
		currentData = currentNode->data;
		/* ID length */
		lenTemp = strlen(currentData->id);
		if (lenTemp > *l_ID)
		{
			*l_ID = lenTemp;
		}
		/* name length*/
		lenTemp = strlen(currentData->name);
		if (lenTemp > *l_Name)
		{
			*l_Name = lenTemp;
		}
		/* Avaliable length */
		lenTemp = getLengthofInt(currentData->on_hand);
		if (lenTemp > *l_OnHand)
		{
			*l_OnHand = lenTemp;
		}
		/* price length */
		lenDollars = 2;
		lenCents = lenDollars;
		if ((lenDollars + lenCents + 1) > *l_Price)
		{
			*l_Price = lenDollars + lenCents + 1;
		}
		currentNode = currentNode->next;
	}

	*l_ID = *l_ID + EXTRA_LEN_COLUM;
	*l_Name = *l_Name + EXTRA_LEN_COLUM;
	*l_OnHand = *l_OnHand + EXTRA_LEN_COLUM;
	*l_Price = *l_Price + EXTRA_LEN_COLUM;
	return TRUE;
}

/**
 * to check item that user input by itemid
 * @return if it's sucessful,return ppd_stock else return NULL
 */
struct ppd_stock * check_input_item_id(char *input_itemId,
		struct ppd_list * item_list)
{
	struct ppd_stock *stock = NULL;
	if (check_item_id(input_itemId) == FALSE)
	{
		return NULL;
	}
	stock = find_stock_byId(input_itemId, item_list);
	if (stock == NULL)
	{
		fprintf(stderr, "Error:There is no this item,"
				"please input another itemId.\n");
		return NULL;
	}
	if (stock->on_hand == 0)
	{
		fprintf(stderr, "Error: The rest count of the item is 0,"
				"you cannot purchase.\n");
		return NULL;
	}
	printf("You have selected \"%s %s\".\n"
			"This will cost you $%2d.%02d.\n", stock->name, stock->desc,
			stock->price.dollars, stock->price.cents);
	printf("Please hand over the money - "
			"type in the value of each note/coin in cents.\n");
	printf("Press enter on a new and empty line to "
			"cancel this purchase:\n");
	return stock;
}

/*
 * inputTotalCents should not more 1000
 * and shoud not be negative number
 * */
BOOLEAN check_input_item_money(struct ppd_stock *stock,
		struct coin *arr_in_Coins, int *val_out_Cents)
{
	char prompt[LINE_LEN + EXTRA_CHARS];
	unsigned itemPriceCents = (stock->price.dollars) * 100
			+ (stock->price.cents);
	int inputCents = 0;
	int inputTotal = 0;
	int changeCents = 0;
	enum denomination denom;
	int i = 0;
	changeCents = inputCents - itemPriceCents;
	while (changeCents < 0)
	{
		sprintf(prompt, "You still need to give us $%d.%02d:",
				abs(changeCents / 100), abs(changeCents % 100));

		if (getInteger(&inputCents, 4, prompt, 0, 1000) == FALSE)
		{
			continue;
		}
		if (inputCents == 0)
		{
			if (inputTotal > 0)
			{
				printf("Your cancle the purchase,"
						"refund your money $%d.%02d.\n", inputTotal / 100,
						inputTotal % 100);
			}
			return FALSE;
		}
		if (check_denomination(&inputCents) == FALSE)
		{
			printf("Error:$%d.%02d is not a valid denomination of money.\n",
					inputCents / 100, inputCents % 100);
			continue;
		}
		inputTotal = inputTotal + inputCents;
		changeCents = inputCents + changeCents;
		denom = getDenoType(inputCents);
		for (i = 0; i < NUM_DENOMS; i++)
		{
			if (arr_in_Coins[i].denom == denom)
			{
				arr_in_Coins[i].count = arr_in_Coins[i].count + 1;
			}
		}
	}
	*val_out_Cents = changeCents;
	return TRUE;
}

BOOLEAN check_transaction(int arg_out_cents, struct coin *arg_out_coins)
{
	int i = 0;
	if (arg_out_cents > 0)
	{
		int money_count = 0;
		for (i = 0; i < NUM_DENOMS; i++)
		{
			money_count = money_count + arg_out_coins[i].count;
		}
		if (money_count == 0)
		{
			printf("There are not sufficient denominations."
					"Transaction will be cancelled!\n");
			return FALSE;
		}
	}
	return TRUE;
}

BOOLEAN update_transaction(struct ppd_system *system, struct ppd_stock *stock,
		int arg_out_cents, struct coin *arg_in_coins,
		struct coin *arg_out_coins)
{
	int i = 0;
	char * ten_d = "$10", *five_d = "$5", *two_d = "$2", *one_d = "$1";
	char * fifty_c = "50c", *twenty_c = "20c", *ten_c = "10c", *five_c = "5c";
	/*we have got arr_in_Coins and arr_ou_Coins,
	 *so we can update cash_register list and on_hand*/
	add_denomination(system, arg_in_coins);
	del_denomination(system, arg_out_coins);
	stock->on_hand = stock->on_hand - 1;
	/* print purchase information */
	printf("Thank you. Here is your %s, and your change of $%d.%02d: ",
			stock->name, abs(arg_out_cents / 100), abs(arg_out_cents % 100));

	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (arg_out_coins[i].count > 0)
		{
			if (arg_out_coins[i].denom == TEN_DOLLARS)
			{
				printf("%s ", ten_d);
			}
			if (arg_out_coins[i].denom == FIVE_DOLLARS)
			{
				printf("%s ", five_d);
			}
			if (arg_out_coins[i].denom == TWO_DOLLARS)
			{
				printf("%s ", two_d);
			}
			if (arg_out_coins[i].denom == ONE_DOLLAR)
			{
				printf("%s ", one_d);
			}
			if (arg_out_coins[i].denom == FIFTY_CENTS)
			{
				printf("%s ", fifty_c);
			}
			if (arg_out_coins[i].denom == TWENTY_CENTS)
			{
				printf("%s ", twenty_c);
			}
			if (arg_out_coins[i].denom == TEN_CENTS)
			{
				printf("%s ", ten_c);
			}
			if (arg_out_coins[i].denom == FIVE_CENTS)
			{
				printf("%s ", five_c);
			}
		}
	}

	printf("\nPlease come again soon.\n");

	return TRUE;
}
