CC = cc -g

generator:  gent.o analizer.o manager.o stats.o
	$(CC) -o generator gent.o analizer.o manager.o stats.o


.PHONY:	clean
clean:
	-rm *.o generator

