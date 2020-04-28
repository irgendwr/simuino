.PHONY: all
all:
	g++ -o simuino simuino.c -lncurses
	touch temp.txt error.txt servuino/serv.error copy.error

.PHONY: clean
clean:
	# -rm *.o
	-rm simuino
	-rm servuino/servuino
	-rm temp.txt
	-rm settings.txt
	-rm conf_list.txt
	-rm error.txt
	-rm copy.error
	-rm servuino/serv.error
	-rm servuino/g++.result
	-rm servuino/sketch.ino
	-rm servuino/data.register
	-rm servuino/data.scen
	-rm servuino/data.scenario
	-rm servuino/ino.debug
	-rm servuino/serv.anaval
	-rm servuino/serv.cust
	-rm servuino/serv.digval
	-rm servuino/serv.event
	-rm servuino/serv.pinmod
	-rm servuino/serv.pinrw
	-rm servuino/serv.serial
	-rm servuino/serv.time
