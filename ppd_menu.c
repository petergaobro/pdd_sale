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

#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu(struct menu_item* menu)
{
	unsigned i = 0;

	char * menuName[NUM_MENU_ITEMS] =
	{ "Display Items", "Purchase Item", "Save and Exit", "Add Item",
			"Remove Item", "Display Coins", "Reset Stock", "Reset Coins",
			"Abort Program" };

	menu_function menuFunctions[NUM_MENU_ITEMS] =
	{ display_items, purchase_item, save_system, add_item, remove_item,
			display_coins, reset_stock, reset_coins, abort_system };

	for (i = 0; i < NUM_MENU_ITEMS; i++)
	{
		strcpy(menu[i].name, menuName[i]);
		menu[i].function = menuFunctions[i];
	}

}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item * menu)
{

	char prompt[50];
	int menu_index;
	BOOLEAN flag = FALSE;

	setbuf(stdout, NULL);

	sprintf(prompt, "Please enter your choice (%d - %d): ", 1, 9);
	do
	{
		flag = getInteger(&menu_index, 1, prompt, 1, 9);
		if (flag == TRUE)
		{
			break;
		}
	} while (flag == FALSE);

	/*printf("\n");*/

	return menu[menu_index - 1].function;
}

/**
 * display system menu
 */
void display_system_menu(struct menu_item * menu)
{
	unsigned i;
	printf("----------\n");
	printf("Main Menu:\n");
	for (i = 0; i < NUM_MENU_ITEMS; i++)
	{
		printf("%d.%s\n", i + 1, menu[i].name);
		if (i == 3)
		{
			printf("Administrator-Only Menu:\n");
		}
	}
}
