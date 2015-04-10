CC = cc -g

generator:  gent.o analizer.o manager.o
	$(CC) -o generator gent.o analizer.o manager.o 


.PHONY:	clean
clean:
	-rm *.o generator

