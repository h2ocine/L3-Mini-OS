slash.o : slash.c
	gcc -c main/util.c main/prompt.c main/redirections.c main/joker.c main/cmdext.c main/cmdint.c main/cdaux.c main/pwdaux.c slash.c -lreadline -g
	gcc util.o prompt.o redirections.o joker.o cmdext.o cmdint.o cdaux.o pwdaux.o slash.o -o slash -lreadline -g

clean:
	rm -rf slash
	rm -rf *.o
