CC = cc -g

generator:  gent.o analizer.o manager.o stats.o reader.o
	$(CC) -o generator gent.o analizer.o manager.o stats.o reader.o


.PHONY:	clean
clean:
	-rm *.o generator

