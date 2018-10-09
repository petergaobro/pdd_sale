/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Peng	Gao
 * Student Number   : s3457558
 * Course Code      : COSC1076
 * Program Code     : BP162
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include "ppd_coin.h"
#include "ppd_main.h"

/**
 * @file ppd_coin.c implement functions here for managing coins in the
 * "cash register" contained in the @ref ppd_system struct.
 **/

BOOLEAN check_denomination(const int *cents)
{
	int money_cents = *cents;
	/*{
	 FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS,
	 ONE_DOLLAR, TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
	 };*/
	if (money_cents == 5 || money_cents == 10 || money_cents == 20
			|| money_cents == 50 || money_cents == 100 || money_cents == 200
			|| money_cents == 500 || money_cents == 1000)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOLEAN check_denomination_count(const int *deno_count)
{
	int money_count = *deno_count;
	int temp_max_moeny_count = pow(10, DOLLAR_MAX_NUM_LEN);
	if (money_count < 0)
	{
		printf("Error: The count of money in the coin file "
				"cannot be negative number.\n");
		return FALSE;
	}

	if (money_count >= temp_max_moeny_count)
	{
		printf("Error: The count of money in the coin file(%d) "
				"has exceeded the max denomination count(%d).\n", money_count,
				temp_max_moeny_count);

		return FALSE;
	}

	return TRUE;
}

struct coin * create_coin(enum denomination denom, int count)
{
	struct coin * new_coin = NULL;
	/* allocate space for the a new stock */
	new_coin =(struct coin *) malloc(sizeof(struct coin));
	if (new_coin == NULL)
	{
		perror("Failed to allocate memory to coin.\n");
		return NULL;
	}
	/* copy the values into the coin */
	new_coin->denom = denom;
	new_coin->count = count;

	return new_coin;

}

struct coin * line_to_coin(char *line_char)
{
	char *token, *end;
	int money = 0, money_count = 0;
	long l_value = 0;
	/* check the denomination*/
	token = strtok(line_char, COIN_DELIM);
	if (token == NULL)
	{
		printf("A line in coin file is not valid. \n");
		return NULL;
	}
	l_value = strtol(token, &end, 10);
	if (*end != '\0')
	{
		fprintf(stderr, "Error: the data line is not valid.\n");
		return NULL;
	}
	if (l_value > INT_MAX || l_value < INT_MIN)
	{
		fprintf(stderr, "Error: %ld is out of range for a denomination.\n",
				l_value);
		return NULL;
	}
	money = (int) l_value;
	if (check_denomination(&money) == FALSE)
	{
		printf("Error:The denomination of '%d' which is in the coin file"
				" is not valid.\n", money);
		return NULL;
	}
	money = getDenoType(money);

	/* check count of the denomination*/
	token = strtok(NULL, COIN_DELIM);
	if (token == NULL)
	{
		fprintf(stderr, "Error: A line in coin file is not valid.\n");
		return NULL;
	}
	l_value = strtol(token, &end, 10);
	if (*end != '\0')
	{
		fprintf(stderr, "Error: A line in coin file is not valid.\n");
		return NULL;
	}
	if (l_value > INT_MAX || l_value < INT_MIN)
	{
		fprintf(stderr, "Error: %ld is out of range for a denomination.\n",
				l_value);
		return NULL;
	}
	money_count = (int) l_value;
	if (check_denomination_count(&money_count) == FALSE)
	{
		return NULL;
	}
	token = strtok(NULL, COIN_DELIM);
	if (token != NULL)
	{
		fprintf(stderr, "Error: A line in coin file is not valid.\n");
		return NULL;
	}
	/*--------------------------------------------------------*/
	return create_coin(money, money_count);
}

BOOLEAN insert_coin(struct ppd_system* ppd, struct coin* strCoin,
		unsigned index)
{
	int i = 0;
	/*char * myDenom=NULL;*/
	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (ppd->cash_register[i].denom == strCoin->denom)
		{
			printf("The denomination is not valid or it has existed.\n");
			return FALSE;
		}
	}

	ppd->cash_register[index] = *strCoin;

	return TRUE;
}

/**
 * save coins
 */
BOOLEAN save_coins(struct ppd_system *ppd)
{
	int i = 0;

	int coin_count = 0;
	FILE* fp;
	if ((fp = fopen(ppd->coin_file_name, "w")) == NULL)
	{
		printf("\n fopen failed - could not open file : %s\n",
				ppd->coin_file_name);
		return FALSE;
	}
	/* denomName = malloc(sizeof(char) * MONEY_MAX_LINE_LEN); */
	for (i = 0; i < NUM_DENOMS; i++)
	{
		char *denomName = getDenoCharFromDeno(ppd->cash_register[i].denom);
		coin_count = ppd->cash_register[i].count;

		/* save the data */
		if (fprintf(fp, "%s,%d\n", denomName, coin_count) < 0)
		{
			/* validate that the save was successful */
			fprintf(stderr, "Save a coin line error,"
					"which is at line of '%s'.\n", denomName);
			fclose(fp);
			return FALSE;
		}
	}
	/* close the stockfile as we have successfully saved the file */
	fclose(fp);

	return TRUE;

}

enum denomination getDenoType(unsigned money)
{
	/*
	 enum denomination
	 {
	 FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS,
	 ONE_DOLLAR, TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
	 };
	 */
	enum denomination retValue;
	switch (money)
	{
	case 5:
		retValue = FIVE_CENTS;
		break;
	case 10:
		retValue = TEN_CENTS;
		break;
	case 20:
		retValue = TWENTY_CENTS;
		break;
	case 50:
		retValue = FIFTY_CENTS;
		break;
	case 100:
		retValue = ONE_DOLLAR;
		break;
	case 200:
		retValue = TWO_DOLLARS;
		break;
	case 500:
		retValue = FIVE_DOLLARS;
		break;
	case 1000:
		retValue = TEN_DOLLARS;
		break;
	default:
		break;
	}

	return retValue;

}

char * getDenoCharFromDeno(enum denomination deno)
{
	char * pDeno;
	switch (deno)
	{
	case FIVE_CENTS:
		pDeno = "5";
		break;
	case TEN_CENTS:
		pDeno = "10";
		break;
	case TWENTY_CENTS:
		pDeno = "20";
		break;
	case FIFTY_CENTS:
		pDeno = "50";
		break;
	case ONE_DOLLAR:
		pDeno = "100";
		break;
	case TWO_DOLLARS:
		pDeno = "200";
		break;
	case FIVE_DOLLARS:
		pDeno = "500";
		break;
	case TEN_DOLLARS:
		pDeno = "1000";
		break;
	default:
		pDeno = "";
		break;
	}

	return pDeno;
}

char * getDenominationFullchar(enum denomination deno)
{
	char *fullName;
	switch (deno)
	{
	case FIVE_CENTS:
		fullName = "5 cents";
		break;
	case TEN_CENTS:
		fullName = "10 cents";
		break;
	case TWENTY_CENTS:
		fullName = "20 cents";
		break;
	case FIFTY_CENTS:
		fullName = "50 cents";
		break;
	case ONE_DOLLAR:
		fullName = "1 dollar";
		break;
	case TWO_DOLLARS:
		fullName = "2 dollar";
		break;
	case FIVE_DOLLARS:
		fullName = "5 dollar";
		break;
	case TEN_DOLLARS:
		fullName = "10 dollar";
		break;
	default:
		fullName = "";
		break;
	}

	return fullName;

}
/**
 * Finding the coins denomnation which is in the coin List
 * return the position of the denomination in the coin array
 */
int find_index_of_denomination(const struct coin *cash_register,
		enum denomination denom)
{
	int i = 0;
	for (i = 0; i < NUM_DENOMS; i++)
	{
		struct coin tempCoin = cash_register[i];
		if ((tempCoin.denom == denom) && tempCoin.count > 0)
		{
			return i;
		}
		else
		{
			continue;
		}
	}

	return -1;
}

BOOLEAN add_denomination(struct ppd_system *ppd, struct coin *pArray_coin)
{
	int i = 0, j = 0;
	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (pArray_coin[i].count > 0)
		{
			for (j = 0; j < NUM_DENOMS; j++)
			{
				if (ppd->cash_register[j].denom == pArray_coin[i].denom)
				{
					ppd->cash_register[j].count = ppd->cash_register[j].count
							+ pArray_coin[i].count;
				}
			}
		}
	}

	return TRUE;
}

BOOLEAN del_denomination(struct ppd_system *ppd, struct coin *pArray_coin)
{
	int i = 0, j = 0;
	for (i = 0; i < NUM_DENOMS; i++)
	{
		if (pArray_coin[i].count > 0)
		{
			for (j = 0; j < NUM_DENOMS; j++)
			{
				if (ppd->cash_register[j].denom == pArray_coin[i].denom)
				{
					ppd->cash_register[j].count = ppd->cash_register[j].count
							- pArray_coin[i].count;
				}
			}
		}
	}

	return TRUE;
}

/**
 * to calculte how many denominations can be withdraw
 * @arg_cents is input cents,which will decrease
 * @arg_coin  is a struct of coin, not a array of strunct coin
 * @max_money is a kind of cents
 * @cash_register is the coin array in the system
 */
BOOLEAN calculte_denominations(int *arg_cents, struct coin *arg_coin,
		const int max_money, const struct coin *cash_register)
{
	int index_cash = 0;
	int denom_count = 0;
	enum denomination denom;
	denom = getDenoType(max_money);
	index_cash = find_index_of_denomination(cash_register, denom);
	denom_count = cash_register[index_cash].count;
	arg_coin->denom = denom;

	while (*arg_cents >= max_money)
	{
		if (denom_count == 0)
		{
			break;
		}
		else
		{
			denom_count = denom_count - 1;
			arg_coin->count = arg_coin->count + 1;
			*arg_cents = *arg_cents - max_money;
		}
	}

	if (arg_coin->count > 0)
		return TRUE;
	else
		return FALSE;
}

/**
 * for change struct coin *arg_coin
 * convert money to array of struct coin
 */
BOOLEAN calculte_coins(int *arg_cents, struct coin *arg_coin,
		const struct coin *cash_register)
{
	int cents = *arg_cents;
	struct coin new_coin;
	while (cents > 0)
	{
		if (cents >= 1000)
		{
			new_coin.denom = TEN_DOLLARS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 1000, cash_register);
			arg_coin[0] = new_coin;
		}
		else if (cents >= 500)
		{
			new_coin.denom = FIVE_DOLLARS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 500, cash_register);
			arg_coin[1] = new_coin;
		}
		else if (cents >= 200)
		{
			new_coin.denom = TWO_DOLLARS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 200, cash_register);
			arg_coin[2] = new_coin;
		}
		else if (cents >= 100)
		{
			new_coin.denom = ONE_DOLLAR;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 100, cash_register);
			arg_coin[3] = new_coin;
		}
		else if (cents >= 50)
		{
			new_coin.denom = FIFTY_CENTS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 50, cash_register);
			arg_coin[4] = new_coin;
		}
		else if (cents >= 20)
		{
			new_coin.denom = TWENTY_CENTS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 20, cash_register);
			arg_coin[5] = new_coin;
		}
		else if (cents >= 10)
		{
			new_coin.denom = TEN_CENTS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 10, cash_register);
			arg_coin[6] = new_coin;
		}
		else if (cents >= 5)
		{
			new_coin.denom = FIVE_CENTS;
			new_coin.count = 0;
			calculte_denominations(&cents, &new_coin, 5, cash_register);
			arg_coin[7] = new_coin;
		}
		else
		{
			break;
		}

	}

	return TRUE;
}

BOOLEAN reset_coin_list(struct ppd_system *ppd)
{
	int i = 0;
	for (i = 0; i < NUM_DENOMS; i++)
	{
		ppd->cash_register[i].count = DEFAULT_COIN_COUNT;
	}
	printf("All coins have been reset to the default level of %d.\n",
	DEFAULT_COIN_COUNT);
	return TRUE;
}

/*
void free_coin(struct coin *my_coin)
{
	free(my_coin);
}
*/

/*
void free_coin_list(struct ppd_system *ppd)
{
	struct coin *coins = ppd->cash_register;
	int i=0;
	for(i=0;i<NUM_DENOMS;i++)
	{
		free_coin(&coins[i]);
	}
}
*/
