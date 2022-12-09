slash.o : slash.c
	gcc -c util.c prompt.c joker.c cmdext.c cmdint.c cdaux.c pwdaux.c slash.c -lreadline -g
	gcc util.o prompt.o joker.o cmdext.o cmdint.o cdaux.o pwdaux.o slash.o -o slash -lreadline -g

clean:
	rm -rf slash
	rm -rf *.o
