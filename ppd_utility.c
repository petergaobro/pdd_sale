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

#include "ppd_utility.h"
#include <assert.h>

/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
	int ch;
	/* keep retrieving characters from stdin until we
	 * are at the end of the buffer
	 */
	while (ch = getc(stdin), ch != '\n' && ch != EOF)
		;
	/* reset error flags on stdin */
	clearerr(stdin);
}

/**
 * get a integer value that user input
 */
BOOLEAN getInteger(int* integer, const unsigned length, const char* prompt,
		const int min, const int max)
{
	BOOLEAN finished = FALSE;
	char* input;
	long long_result;
	int int_result = 0;

	/* a pointer to the next character after what has been successfully processed */
	char* end;

	/* Allocate temporary memory. */
	if ((input = malloc(sizeof(char) * (length + EXTRA_CHARS))) == NULL)
	{
		/* displays this message followed by a system defined error message */
		perror("failed to allocate memory in getInteger");
		return FALSE;
	}

	/* Continue to interact with the user until the input is valid. */
	do
	{
		/* Provide a custom prompt. */
		printf("%s", prompt);

		/* Accept input. "EXTRACHARS" is for the \n and \0
		 * characters. */
		fgets(input, length + EXTRA_CHARS, stdin);

		/* A string that doesn't have a newline character is too
		 * long (has leftover input in the buffer). */
		if (input[strlen(input) - 1] != '\n')
		{
			printf("Input was too long.\n");
			read_rest_of_line();
		}
		else
		{
			/* Overwrite the \n character with \0 as we now
			 * know we have gotten all the data from the buffer
			 */
			input[strlen(input) - 1] = '\0';
			/* Convert string to an integer. */
			long_result = strtol(input, &end, 10);
			/* Validate integer result. */
			if (*end != '\0')
			{
				/* there were leftover characters that were
				 * no processed so it was not a valid
				 * integer
				 */
				fprintf(stderr, "Input was not numeric.\n");
				free(input);
				return FALSE;
				/* continue; */

			}
			/* validate the range of the integer */
			if (long_result < min || long_result > max)
			{
				printf("Input was not within range %d - %d.\n", min, max);
				free(input);
				return FALSE;
			}
			else
			{
				/* all good so assign the value to its
				 * integer variable
				 */
				int_result = long_result;
				finished = TRUE;
			}
		}
	} while (finished == FALSE);

	/* Make the result integer available to calling function. */
	*integer = int_result;

	/* Deallocate temporary memory. */
	free(input);

	return TRUE;
}

/**
 * get the length of Integer
 */
unsigned getLengthofInt(unsigned v)
{
	unsigned count = 1;
	unsigned n = v;
	while (n >= 10)
	{
		count++;
		n = n / 10;
	}
	return count;
}

/**
 * print a line with a char on the screen
 */
void print_line(unsigned lineNumber, char c)
{
	int i = 0;
	for (i = 0; i < lineNumber; i++)
	{
		printf("%c", c);
	}
	printf("\n");
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
	BOOLEAN retValue = FALSE;

	/* whether the system pointer is valid */
	assert(system != NULL);

	/* initialize system memery, set system memery is 0*/
	memset(system, 0, sizeof(struct ppd_system));

	/* initialize coin list */
	system->coin_from_file = FALSE;
	init_coin_list(system->cash_register);

	/* initialize item_list */
	if (init_stock_list(&system->item_list) == FALSE)
	{
		retValue = FALSE;
	}
	else
	{
		retValue = TRUE;
	}

	return retValue;
}

/**
 * loads the stock file's data into the system. This needs to be 
 * implemented as part of requirement 2 and 19 of the assignment specification.
 **/
BOOLEAN load_stock(struct ppd_system * system, const char * filename)
{
	FILE *fpStockRead = NULL;
	char stockLine[STOCK_MAX_LINE + EXTRA_CHARS];
	struct ppd_stock* newstock;
	/* Validating whether the file exists */
	fpStockRead = fopen(filename, "r");
	if (!fpStockRead)
	{
		printf("Error:Stock file open failed: %s, it is probably not exist.\n",
				filename);
		return FALSE;
	}
	else
	{
		/* Requirement #19 Validating the input Files */
		while (fgets(stockLine, STOCK_MAX_LINE + EXTRA_CHARS, fpStockRead)
				!= NULL)
		{
			if (strlen(stockLine) < 1)
			{
				fprintf(stderr, "Error: the file is empty.\n");
				fclose(fpStockRead);
				return FALSE;
			}
			/*printf("\n%s", stockLine);*/
			if (stockLine[strlen(stockLine) - 1] != '\n')
			{
				fprintf(stderr, "Error: line is too long in loading stock.\n");
				fclose(fpStockRead);
				return FALSE;
			}

			/* remove the newline */
			stockLine[strlen(stockLine) - 1] = '\0';
			newstock = line_to_stockItem(stockLine);
			if (newstock == NULL)
			{
				fclose(fpStockRead);
				return FALSE;
			}
			/* check the newstock is unique using checking the item */
			if (find_stock_byId(newstock->id, system->item_list) != NULL)
			{
				printf("Loading stock error:Stock item is not unique.\n");
				fclose(fpStockRead);
				return FALSE;
			}
			/* insert the stock into the list */
			if (insert_stock(newstock, system->item_list) == FALSE)
			{
				printf("\nLoading stock error:Insert item into list fail.\n");
				fclose(fpStockRead);
				return FALSE;
			}
		}
	}
	fclose(fpStockRead);
	system->stock_file_name = filename;
	return TRUE;
}

/**
 * loads the contents of the coins file into the system. This needs to
 * be implemented as part 1 of requirement 18.
 **/
BOOLEAN load_coins(struct ppd_system * system, const char * filename)
{
	FILE *fpRead = NULL;
	char moneyLine[MONEY_MAX_LINE_LEN + EXTRA_CHARS];
	int i = 0;
	fpRead = fopen(filename, "r");
	if (!fpRead)
	{
		printf("Error:Coin file open failed: %s,"
				"it is probably not exist.\n", filename);
		return FALSE;
	}
	else
	{
		/* Requirement #18 Validating the input coin Files */
		while (fgets(moneyLine, MONEY_MAX_LINE_LEN + EXTRA_CHARS, fpRead))
		{
			BOOLEAN insert_ok = FALSE;
			struct coin *tempCoin = NULL;

			if (moneyLine[strlen(moneyLine) - 1] != '\n')
			{
				fprintf(stderr, "Error: line that is in the coin file"
						" is too long in loading coin.\n");
				fclose(fpRead);
				return FALSE;
			}
			/* remove the newline */
			moneyLine[strlen(moneyLine) - 1] = '\0';

			tempCoin = line_to_coin(moneyLine);
			if (tempCoin == NULL)
			{
				fclose(fpRead);
				return FALSE;
			}
			insert_ok = insert_coin(system, tempCoin, i);
			free(tempCoin);
			if (insert_ok== FALSE)
			{
				fclose(fpRead);
				return FALSE;
			}
			i++;
		}
	}
	system->coin_from_file = TRUE;
	system->coin_file_name = filename;
	fclose(fpRead);
	if (i != NUM_DENOMS)
	{
		printf("Error: the number of denomination is not 8.\n"
				"system has exited!\n");
		return FALSE;
	}
	return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{

	/*free_coin_list(system);*/

	/* free the stock list */
	free_stock_list(system->item_list);
	free(system->item_list);



}

