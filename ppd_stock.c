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
#include "ppd_stock.h"
#include "ppd_utility.h"

/**
 * @file ppd_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

/**
 * initial the coin list
 */
void init_coin_list(struct coin *cash_register)
{
	int i;
	for (i = 0; i < NUM_DENOMS; i++)
	{
		struct coin myCoin;
		myCoin.denom = OTHER_DOLLARS;
		myCoin.count = 0;
		cash_register[i] = myCoin;
	}

}

/**
 * initial the stock list from file
 */
BOOLEAN init_stock_list(struct ppd_list ** stockList)
{
	/* allocate space for the ppd_list */
	void * memory = malloc(sizeof(struct ppd_list));
	if (!memory)
	{
		perror("Can not allocate memory");
		return FALSE;
	}

	/* set ppd_list 0 */
	memset(memory, 0, sizeof(struct ppd_list));

	*stockList = (struct ppd_list*) memory;

	return TRUE;
}

/**
 * to check whether item id is valid
 */
BOOLEAN check_item_id(const char * itemID)
{
	BOOLEAN retValue;
	if (itemID[0] != 'I')
	{
		retValue = FALSE;
		printf("The itemID '%s' must begin with 'I'.\n", itemID);
	}
	else if (strlen(itemID) != IDLEN)
	{
		retValue = FALSE;
		printf("The length of itemID '%s' is not valid . \n", itemID);
	}
	else
	{
		retValue = TRUE;
	}
	return retValue;
}

/**
 * to check whether name id is valid
 */
BOOLEAN check_item_name(const char * itemName)
{
	BOOLEAN retValue;
	int nameLen = strlen(itemName);
	if (nameLen < 1 || nameLen > NAMELEN)
	{
		retValue = FALSE;
		printf("The length of itemName '%s' is not valid. \n", itemName);
	}
	else
	{
		retValue = TRUE;
	}
	return retValue;
}

/**
 * to check whether item describle is valid
 */
BOOLEAN check_item_dec(const char * itemDec)
{
	BOOLEAN retValue;
	int decLen = strlen(itemDec);
	if (decLen > DESCLEN)
	{
		retValue = FALSE;
		printf("The length of itemDesc '%s' is too long . \n", itemDec);
	}
	else
	{
		retValue = TRUE;
	}
	return retValue;
}

/**
 * to check whether item price is valid
 */
BOOLEAN check_item_price(const char * char_price, struct price * itemPrice)
{
	long dollar, cent;
	const char * p = char_price;
	char* end;
	int len = strlen(p);
	if (len < 4 || len > PRICELEN)
	{
		printf("Error: The format of price '%s' is not valid"
				",the patten must like'0.00'\n", char_price);
		return FALSE;
	}
	/* dollar */
	dollar = strtol(p, &end, 10);
	if (dollar < 0 || dollar > 10)
	{
		printf("Error: you did not enter valid price '%s'.\n", char_price);
		return FALSE;
	}

	if (*end != '.')
	{
		printf("Error: the price,%s, is not valid.\n", char_price);
		return FALSE;
	}
	p = end + 1;
	cent = strtol(p, &end, 10);
	if (cent < 0 || cent > 95)
	{
		printf("Error: you did not enter valid price '%s'.\n", char_price);
		return FALSE;
	}
	if (cent % 5 != 0)
	{
		printf("Error: you did not enter valid price '%s'.\n", char_price);
		return FALSE;
	}
	if (dollar == 10 && cent > 0)
	{
		printf("Error: the price must less 10 dollar '%s'.\n", char_price);
		return FALSE;
	}

	if (dollar <= 0 && cent <= 0)
	{
		printf("Error: the price cannot be negative '%s'.\n", char_price);
		return FALSE;
	}
	itemPrice->dollars = dollar;
	itemPrice->cents = cent;
	return TRUE;
}

/**
 * to check whether item count is valid
 */
BOOLEAN check_item_OnHand(const char * itemOnHand,
		unsigned int * itemHandNumber)
{
	char* end;
	*itemHandNumber = strtol(itemOnHand, &end, 10);
	/* Validate integer result. */
	if (*end != '\0')
	{
		printf("Error: the Onhand of item '%s' is not a numeric.\n",
				itemOnHand);
		return FALSE;
	}
	/* validate the range of the hand number */
	if (*itemHandNumber < 0 || *itemHandNumber > MAX_ONHAND)
	{
		printf("Input was not within range %d - %d.\n", 0, MAX_ONHAND);
		return FALSE;
	}

	return TRUE;
}

/**
 * convert a readline to a stock item
 */
struct ppd_stock * line_to_stockItem(char * stockLine)
{
	int i = 0;
	char *token;
	char *sId, *sName, *sDesc;
	struct price sPrice;
	unsigned int sHand;
	struct ppd_stock * stock = NULL;
	/*====================================*/
	token = strtok(stockLine, DELIM);
	if (token == NULL)
	{
		printf("The line is not valid. \n");
		return NULL;
	}
	while (token != NULL)
	{
		switch (i)
		{
		case 0:
			if (check_item_id(token) == FALSE)
			{
				return FALSE;
			}
			else
			{
				sId = token;
			}
			break;
		case 1:
			if (check_item_name(token) == FALSE)
			{
				return FALSE;
			}
			else
			{
				sName = token;
			}
			break;
		case 2:
			if (check_item_dec(token) == FALSE)
			{
				return FALSE;
			}
			else
			{
				sDesc = token;
			}
			break;
		case 3:
			if (check_item_price(token, &sPrice) == FALSE)
			{
				return FALSE;
			}
			break;
		case 4:
			if (check_item_OnHand(token, &sHand) == FALSE)
			{
				return FALSE;
			}
			break;
		default:
			printf("There are too many fields in a line of the item.\n");
			break;
		}
		/*printf("\nToken # %d = %s", i, token);*/
		token = strtok(NULL, DELIM);
		i++;
	}
	if (i != 5)
	{
		printf("The number of fields of the item in a line is not 5.\n");
		return FALSE;
	}
	stock = create_stock(sId, sName, sDesc, sPrice, sHand);
	if (stock == NULL)
	{
		return NULL;
	}
	else
	{
		return stock;
	}
}

/**
 * create a new stock
 */
struct ppd_stock * create_stock(const char * sId, const char * sName,
		const char * sDesc, const struct price sPrice, const unsigned sHand)
{
	struct ppd_stock * newStock = NULL;

	/* allocate space for the a new stock */
	newStock = malloc(sizeof(struct ppd_stock));
	if (newStock == NULL)
	{
		perror("failed to allocate memory");
		return NULL;
	}
	/* copy the values in */
	strcpy(newStock->id, sId);
	strcpy(newStock->name, sName);
	strcpy(newStock->desc, sDesc);
	newStock->price = sPrice;
	newStock->on_hand = sHand;
	return newStock;
}

/**
 * save a new stock,add the stock to itemlist
 */
BOOLEAN save_stock(struct ppd_system *ppd)
{
	FILE* fpstock;
	struct ppd_node * current = ppd->item_list->head;
	struct ppd_stock * data = NULL;
	if ((fpstock = fopen(ppd->stock_file_name, "w")) == NULL)
	{
		printf("\n fopen failed - could not open file : %s\n",
				ppd->stock_file_name);
		return FALSE;
	}
	while (current != NULL)
	{
		data = current->data;
		if (check_item_id(data->id) == FALSE)
		{
			fprintf(stderr, "Save a line error:'%s'.\n", data->id);
			fclose(fpstock);
			return FALSE;
		}
		if (check_item_name(data->name) == FALSE)
		{
			fprintf(stderr, "Save a line error:'%s'.\n", data->name);
			fclose(fpstock);
			return FALSE;
		}
		if (check_item_dec(data->desc) == FALSE)
		{
			fprintf(stderr, "Save a line error:'%s'.\n", data->desc);
			fclose(fpstock);
			return FALSE;
		}

		/* save the data */
		if (fprintf(fpstock, "%s|%s|%s|%2d.%02d|%d\n", data->id, data->name,
				data->desc, data->price.dollars, data->price.cents,
				data->on_hand) < 0)
		{
			/* validate that the save was successful */
			fprintf(stderr, "Save a line error which is at line of '%s'.\n",
					data->id);
			fclose(fpstock);
			return FALSE;
		}
		current = current->next;
	}
	/* close the stockfile as we have successfully saved the file */
	fclose(fpstock);
	return TRUE;
}

/**
 * find a node by item id
 */
struct ppd_node * find_node_byId(const char * sId,
		const struct ppd_list* itemList)
{
	struct ppd_node *retValue;
	struct ppd_node * current;
	struct ppd_stock * stock;
	/*=================================================*/
	current = itemList->head;
	if (current == NULL || itemList->count == 0)
	{
		return NULL;
	}
	do
	{
		stock = current->data;
		if (strcmp(sId, stock->id) == 0)
		{
			retValue = current;
			break;
		}
		else
		{
			retValue = NULL;
		}
		current = current->next;
	} while (current != NULL);
	return retValue;
}

/**
 * find a stock by item id
 */
struct ppd_stock * find_stock_byId(const char * sId,
		const struct ppd_list* itemList)
{
	struct ppd_stock *retValue;
	struct ppd_node * current;
	struct ppd_stock * stock;
	current = itemList->head;
	if (current == NULL || itemList->count == 0)
	{
		return NULL;
	}
	do
	{
		stock = current->data;
		if (strcmp(sId, stock->id) == 0)
		{
			retValue = stock;
			break;
		}
		else
		{
			retValue = NULL;
		}
		current = current->next;

	} while (current != NULL);

	return retValue;
}

/**
 * find a stock by item's name
 */
struct ppd_stock * find_stock_byName(const char * sName,
		const struct ppd_list* itemList)
{
	struct ppd_stock *retValue;
	struct ppd_node * current;
	struct ppd_stock * stock;
	current = itemList->head;
	if (current == NULL || itemList->count == 0)
	{
		return NULL;
	}
	do
	{
		stock = current->data;
		if (strcmp(sName, stock->name) == 0)
		{
			retValue = stock;
			break;
		}
		else
		{
			retValue = NULL;
		}
		current = current->next;

	} while (current != NULL);

	return retValue;
}

/*
 * insert a new stock into itemList
 * ensure that newstock is inserted in the linked list in an orderly fashion
 * and sorted by their names
 * */
BOOLEAN insert_stock(struct ppd_stock * newstock, struct ppd_list * itemList)
{
	struct ppd_node * previousNode = NULL;
	struct ppd_node * currentNode = NULL;
	int i, stockCount;
	struct ppd_node * newNode = malloc(sizeof(struct ppd_node));
	if (!newNode)
	{
		perror("failed to allocate memory.\n");
		return FALSE;
	}
	else
	{
		newNode->data = newstock;
		newNode->next = NULL;
	}
	if (find_stock_byId(newstock->id, itemList) != NULL)
	{
		printf("Error: cannot insert into the ItemList.\n");
		return FALSE;
	}
	if (itemList->head == NULL || itemList->count == 0)
	{
		itemList->head = newNode;
		itemList->count = 1;
	}
	else
	{
		stockCount = itemList->count;
		currentNode = itemList->head;
		previousNode = itemList->head;
		for (i = 1; i <= stockCount; i++)
		{
			/* if smaller then insert */
			if (strcmp(newNode->data->name, currentNode->data->name) < 0)
			{
				if (currentNode == previousNode)
				{
					itemList->head = newNode;
				}
				else
				{
					previousNode->next = newNode;
				}
				newNode->next = currentNode;
				itemList->count = itemList->count + 1;
				break;
			}
			else
			{
				if (currentNode->next == NULL)
				{
					currentNode->next = newNode;
					itemList->count = itemList->count + 1;
					break;
				}
				else
				{
					previousNode = currentNode;
					currentNode = currentNode->next;
					continue;
				}
			}
		}
	}
	return TRUE;
}

/**
 * free stock list memory
 */
void free_stock_list(struct ppd_list* itemList)
{
	struct ppd_node *current = NULL;
	struct ppd_node *next = NULL;

	current = itemList->head;
	while (current)
	{
		next = current->next;
		if (current != NULL)
		{
			free_node(current);
		}
		current = next;
	}

}

/**
 * free a node memory
 */
void free_node(struct ppd_node* node)
{
	free_stock(node->data);
	free(node);
}

void free_stock(struct ppd_stock* stock)
{
	if (stock != NULL)
	{
		free(stock);
	}
}

/**
 * generate a new stock Id
 */
BOOLEAN generateStockId(struct ppd_system *ppd, char *newId)
{
	struct ppd_node *current;
	struct ppd_stock *my_stock;
	char ch_id[IDLEN];
	long l_temp_id = 0, l_max_id = 0;
	int i_new_id = 0;
	int i;
	char *pEnd;

	current = ppd->item_list->head;
	while (current != NULL)
	{
		my_stock = current->data;
		strcpy(ch_id, my_stock->id);
		for (i = 0; i < sizeof(ch_id) - 1; i++)
		{
			ch_id[i] = ch_id[i + 1];
		}
		ch_id[i] = '\0';
		l_temp_id = strtol(ch_id, &pEnd, 10);
		if (l_temp_id > l_max_id)
		{
			l_max_id = l_temp_id;
		}
		current = current->next;
	}
	i_new_id = (int) l_max_id + 1;
	if (i_new_id > 9999)
	{
		printf("Items number has exceeded max number.\n");
		return FALSE;
	}
	sprintf(newId, "I%04d", i_new_id);
	return TRUE;
}

/**
 * when add item,to check whether the item's name can be accepted
 */
BOOLEAN acceptStockName(struct ppd_system *ppd, char *newName)
{
	do
	{
		printf("Enter the item name:");
		fgets(newName, NAMELEN + EXTRA_CHARS, stdin);
		if (newName[0] == '\n')
		{
			return FALSE;
		}
		if (newName[strlen(newName) - 1] != '\n')
		{
			printf("Input item's name was too long.\n");
			read_rest_of_line();
		}
		else
		{
			newName[strlen(newName) - 1] = '\0';
		}
		if (check_item_name(newName) == FALSE)
		{
			continue;
		}
		if (find_stock_byName(newName, ppd->item_list) != NULL)
		{
			printf("This item has existed!,please change the name.\n");
			continue;
		}
		else
		{
			break;
		}

	} while (1 == 1);

	return TRUE;
}

/**
 * when add item,to check whether the item's description can be accepted
 */
BOOLEAN acceptStockDesc(struct ppd_system *ppd, char *newDesc)
{
	do
	{
		printf("Enter the item description:");
		fgets(newDesc, DESCLEN + EXTRA_CHARS, stdin);

		if (newDesc[strlen(newDesc) - 1] != '\n')
		{
			printf("Input item's name was too long.\n");
			read_rest_of_line();
		}
		else
		{
			newDesc[strlen(newDesc) - 1] = '\0';
		}
		if (check_item_dec(newDesc) == FALSE)
		{
			continue;
		}
		else
		{
			break;
		}

	} while (1 == 1);

	return TRUE;
}

/**
 * when add item,to check whether the item's price can be accepted
 */
BOOLEAN acceptStockPrice(struct ppd_system *ppd, struct price *newPrice)
{
	char myPrice[5];
	do
	{
		printf("Enter the item price:");
		fgets(myPrice, 5 + EXTRA_CHARS, stdin);

		if (myPrice[strlen(myPrice) - 1] != '\n')
		{
			printf("Input item's Price was too long.\n");
			read_rest_of_line();
		}
		else
		{
			myPrice[strlen(myPrice) - 1] = '\0';
		}
		if (check_item_price(myPrice, newPrice) == FALSE)
		{
			continue;
		}
		else
		{
			break;
		}

	} while (1 == 1);

	return TRUE;
}

/**
 * delete a stock through item id
 */
BOOLEAN deleteItemById(struct ppd_system *ppd, char * itemId)
{
	char * stockId;
	struct ppd_list * list;
	struct ppd_node * prev_node;
	struct ppd_node * curr_node;
	struct ppd_stock * stock;
	BOOLEAN retValue = FALSE;
	/*=================================================*/
	stockId = itemId;
	list = ppd->item_list;
	curr_node = list->head;
	if (curr_node == NULL || list->count == 0)
	{
		printf("The item list is NULL.\n");
		return FALSE;
	}
	else
	{
		prev_node = curr_node;
	}

	while (curr_node != NULL)
	{
		stock = curr_node->data;
		if (strcmp(stockId, stock->id) == 0)
		{
			if (curr_node == prev_node)
			{
				list->head = curr_node->next;
			}
			else
			{
				prev_node->next = curr_node->next;
			}
			printf("\"%s - %s - %s\" has been removed from the system.\n",
					stock->id, stock->name, stock->desc);
			free_node(curr_node);
			--list->count;
			retValue = TRUE;
			break;
		}

		prev_node = curr_node;
		curr_node = curr_node->next;
	}

	return retValue;
}

/**
 * reset items count value to default
 */
BOOLEAN resetOnHand(struct ppd_system *ppd)
{
	struct ppd_list * list;
	struct ppd_node * curr_node;
	struct ppd_stock * stock;
	/*=================================================*/
	list = ppd->item_list;
	curr_node = list->head;
	if (curr_node == NULL || list->count == 0)
	{
		printf("The item list is NULL.\n");
		return FALSE;
	}

	while (curr_node != NULL)
	{
		stock = curr_node->data;
		stock->on_hand = DEFAULT_STOCK_LEVEL;
		curr_node = curr_node->next;
	}

	printf("All stock has been reset to the default level of %d.\n",
	DEFAULT_STOCK_LEVEL);
	return TRUE;

}
