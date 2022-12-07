slash.o : slash.c
	gcc -c util.c prompt.c joker.c cmdext.c cmdint.c cdaux.c pwdaux.c slash.c -lreadline 
	gcc util.o prompt.o joker.o cmdext.o cmdint.o cdaux.o pwdaux.o slash.o -o slash -lreadline

clean:
	rm -rf slash
	rm -rf *.o
