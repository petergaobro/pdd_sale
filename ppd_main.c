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
#include "ppd_menu.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    /* uncomment this menu structure when you are ready to work on the
     * menu system with function pointers
     * */
    struct menu_item menu[NUM_MENU_ITEMS];
    BOOLEAN run_flag;
    BOOLEAN initSystem_flag, loadStock_flag, loadCoin_flag;
    struct ppd_system system;
    
    /* validate command line arguments */
    run_flag = check_command_args(argc);
    if (run_flag == FALSE)
    {
        return EXIT_FAILURE;
    }
    /* init the system */
    initSystem_flag = system_init(&system);
    
    /* load stock data */
    if (initSystem_flag == TRUE)
    {
        loadStock_flag = load_stock(&system, argv[1]);
        /* load coin data */
        if (argc == 3)
        {
            loadCoin_flag = load_coins(&system, argv[2]);
        }
        else
        {
            loadCoin_flag = TRUE;
        }
    }
    
    /* test if everything has been initialised correctly */
    if (initSystem_flag == FALSE || loadStock_flag == FALSE
        || loadCoin_flag == FALSE)
    {
        return EXIT_FAILURE;
    }
    
    /* initialise the menu system */
    init_menu(menu);
    
    /* set windows console display */
    setbuf(stdout, NULL);
    
    /* loop, asking for options from the menu */
    while (run_flag)
    {
        menu_function selectMenu;
        display_system_menu(menu);
        
        /* print the menu and get the menu selected by user */
        selectMenu = get_menu_choice(menu);
        
        /* run each option selected
         ** user exit will be implemented in the exit menu
         */
        selectMenu(&system);
    };
    
    return EXIT_SUCCESS;
}

/**
 * to check the count of input auguments
 */
BOOLEAN check_command_args(int argc)
{
    BOOLEAN run_flag = FALSE;
    
    switch (argc)
    {
        case 1:
            printf("You must run with one or two arguments.\n");
            run_flag = FALSE;
            break;
        case 2:
            run_flag = TRUE;
            break;
        case 3:
            run_flag = TRUE;
            break;
        default:
            printf("The number of arguments are incorrect,"
                   "run the application again\n");
            run_flag = FALSE;
            break;
    }
    return run_flag;
}
