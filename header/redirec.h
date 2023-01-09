

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int cmd_with_redirection(char *cmd, char **args, int length,
                         int pos_redirection);

void go_back_to_standard(int *fd_std);

int handle_redirection(char *redirection, char *filename);


int contains_valid_redirection(char** args_extanded, int size);