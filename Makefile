slash.o : slash.c 
	gcc -Wall slash.c -o slash -lreadline -g

clean : 
	rm -rf slash
	rm -rf *.o




