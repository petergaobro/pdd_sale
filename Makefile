
USER=saaaa
SOURCES=ppd_main.c ppd_menu.c ppd_options.c ppd_utility.c ppd_stock.c \
ppd_coin.c
HEADERS=ppd_main.h ppd_menu.h ppd_options.h ppd_utility.h ppd_stock.h \
ppd_coin.h ppd_shared.h
README=ppd_readme
REQUIREMENT13=requirement13.txt
MAKEFILE=Makefile
###  $^ variable means "all dependencies of this target"
###  $@ means "the name of this target"
#######################################################################
CC		= gcc
CFLAGS	= -ansi -pedantic -Wall
PROG 	= ppd
OBJS	= ppd_main.o ppd_menu.o ppd_options.o ppd_utility.o \
 ppd_stock.o ppd_coin.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

ppd_main.o: ppd_main.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_main.c
	
ppd_menu.o: ppd_menu.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_menu.c
	
ppd_options.o: ppd_options.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_options.c
	
ppd_utility.o: ppd_utility.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_utility.c

ppd_stock.o: ppd_stock.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_stock.c

ppd_coin.o: ppd_coin.c $(HEADERS)
	$(CC) $(CFLAGS) -c ppd_coin.c
#######################################################################
	
clean:
	@echo "cleaning ..."
	rm -f *.o ppd 

########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Do not submit your .dat files, or directories. 
# We only want the files that are part of your implementation.
########################################################################
archive:
	zip $(USER)a2.zip $(SOURCES) $(HEADERS) $(README) $(REQUIREMENT13) $(MAKEFILE)
