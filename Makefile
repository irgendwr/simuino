RM = rm -f
CC = g++
CFLAGS = -Wpedantic -Wall -Wextra -O3
INCLUDES =
LFLAGS =
LIBS = -lncurses -lm

.PHONY: all
all:
	$(CC) $(CFLAGS) $(INCLUDES) $(LFLAGS) $(LIBS) -o simuino simuino.c
	touch temp.txt error.txt servuino/serv.error copy.error

.PHONY: clean
clean:
	# $(RM) *.o
	$(RM) simuino
	$(RM) servuino/servuino
	$(RM) temp.txt
	$(RM) settings.txt
	$(RM) conf_list.txt
	$(RM) error.txt
	$(RM) copy.error
	$(RM) servuino/serv.error
	$(RM) servuino/g++.result
	$(RM) servuino/sketch.ino
	$(RM) servuino/data.register
	$(RM) servuino/data.scen
	$(RM) servuino/data.scenario
	$(RM) servuino/ino.debug
	$(RM) servuino/serv.anaval
	$(RM) servuino/serv.cust
	$(RM) servuino/serv.digval
	$(RM) servuino/serv.event
	$(RM) servuino/serv.pinmod
	$(RM) servuino/serv.pinrw
	$(RM) servuino/serv.serial
	$(RM) servuino/serv.time
