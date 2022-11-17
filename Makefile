slash.o : slash.c 
	gcc -Wall slash.c -o slash -lreadline
	./slash

clean : 
	rm -rf *.o




