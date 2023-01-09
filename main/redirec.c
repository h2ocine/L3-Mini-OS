#include "redirection.h"


/**
 * @brief check if the arg list contains a valid redirection (redirection symbol
 * and a filename destination)
 *
 * @param args_extanded argument array
 * @param size size of the array , number of args
 * @return return the position of the redirection symbol in array if finded
 * returns -1 if the redirection is invalid
 * returns -2 if there is no redirection
 */

void exec_externe(char **tab, int taille, int *last_exit, char *fic, char *typeredirection)
{
    if (verif_executable(tab, taille) == 1)
    {

        // printf(" ca rentre\n");
        int t;
        char **sep = explode(tab[0], "/", &t);

        char *pwdEnv = getenv("PWD");
        size_t size = strlen(pwdEnv) + 1;
        char *cpyPwd = malloc(size + 1);
        snprintf(cpyPwd, size + 1, "%s", pwdEnv);
        cpyPwd[size - 1] = '/';
        cpyPwd[size] = '\0';

        for (int i = 0; i < t - 1; i++)
        {
            size += strlen(sep[i]) + 1;
            cpyPwd = realloc(cpyPwd, size + 1);
            strncat(cpyPwd, sep[i], strlen(sep[i]));
            cpyPwd[size - 1] = '/';
            cpyPwd[size] = '\0';
        }

        if (isIn(cpyPwd, sep[t - 1]) == 1)
        {
            size += strlen(sep[t - 1]);
            cpyPwd = realloc(cpyPwd, size + 1);
            strncat(cpyPwd, sep[t - 1], strlen(sep[t - 1]));

            char **arguments_exec = malloc((taille + 1) * sizeof(char *));
            // arguments_exec[0] = cpyPwd;
            arguments_exec[0] = malloc(strlen(cpyPwd) + 1);
            strncpy(arguments_exec[0], cpyPwd, strlen(cpyPwd) + 1);
            for (int i = 1; i < taille; i++)
            {
                arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
            }
            arguments_exec[taille] = NULL;
            execCMD(cpyPwd, arguments_exec, last_exit);
            //redirect_cmd_externe(cpyPwd, arguments_exec, last_exit, fic, typeredirection);
            free_StingArrayArray(arguments_exec, taille);
        }
        else
        {
            // *last_exit = 127;
            // printf("bash: ./%s: Aucun fichier ou dossier de ce type \n",sep[0]);
        }

        free(cpyPwd);

        free_StingArrayArray(sep, t);
    }
    else
    {

        //  On recupere la variable d'environnement path et on lui cree une copie (car si on la modifie la var d'env se modifie)

        char *envPath = getenv("PATH");
        size_t taille_envPath = strlen(envPath);

        // Creation de la copie
        char *path = malloc(taille_envPath + 1);
        snprintf(path, taille_envPath + 1, "%s", envPath);
        path[taille_envPath] = '\0';

        // On decoupe la variable path avec le separateur ":"
        int taillarrpath;
        char **pathDecoupe = explode(path, ":", &taillarrpath);
        free(path);

        // on parcout chaque élément de la variable PATH
        for (int i = 0; i < taillarrpath; i++)
        {
            // On verifie si chaque chemin du tabl eau pathDecoup a un fichier de nom la commande ecrite par l'utilisateur (tab[0])
            // Si non on essaye avec un autre chemin
            if (isIn(pathDecoupe[i], tab[0]) == 0)
            {
                // printf("Encore \n");
                continue;
            }
            else
            {

                size_t taille_elem = strlen(pathDecoupe[i]);

                // On cree un tableau de chaine de caratere pour la fonction exec qui doit ressembler à ça pour l'exemple "ls -l path" -> [/usr/bin/ls, -l, path, NULL]
                // Taille + 1 car le dernier element doit etre NULL
                char **arguments_exec = malloc((taille + 1) * sizeof(char *));

                char *cmd = malloc(taille_elem + 1 + /* taille du '/' */ +strlen(tab[0]) + 1);
                if (cmd == NULL)
                    perror("malloc");
                snprintf(cmd, taille_elem + 1, "%s", pathDecoupe[i]);
                cmd[taille_elem] = '\0';

                // equivalent de strcat(arg0,tab[0]) :
                cmd[taille_elem] = '/';
                snprintf(&cmd[taille_elem + 1], strlen(tab[0]) + 1, "%s", tab[0]);
                int t = taille_elem + 1 + strlen(tab[0]);
                cmd[t] = '\0';

                // printf("cmd  = %s \n", cmd);

                // On complete le premier argument du tableau avec la commande
                arguments_exec[0] = malloc(strlen(cmd) + 1); //---------------------->fuite mem
                snprintf(arguments_exec[0], strlen(cmd) + 1, "%s", cmd);

                // On remplit le tableau pour les options
                for (int i = 1; i < taille; i++)
                {

                    arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                    snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
                    // printf("args[%d] = %s \n", i, arguments_exec[i]);
                }

                arguments_exec[taille] = NULL;

                // Fait la redirection qui correspond au "typeredirection"
                //redirect_cmd_externe(cmd, arguments_exec, last_exit, fic, typeredirection);
                execCMD(cpyPwd, arguments_exec,last_exit);

                free(cmd);
                free_StingArrayArray(arguments_exec, taille);
                free_StingArrayArray(pathDecoupe, taillarrpath);

                return;
            }
        }
        //*last_exit = 127;
        free_StingArrayArray(pathDecoupe, taillarrpath);
        exit(0);
    }
}


int contains_valid_redirection(char** args_extanded, int size) {

    for(int i = size-1; i>0; i--){
      char* string = args_extanded[i];
      if (strcmp(string, ">") == 0 || strcmp(string, "<") == 0 ||
          strcmp(string, ">>") == 0 || strcmp(string, ">|") == 0 ||
          strcmp(string, "2>") == 0 || strcmp(string, "2>>") == 0 ||
          strcmp(string, "2>|") == 0 || strcmp(string, "2>") == 0) {
          //here args_extanded[i+1] is the redirection field and it can't be a pipe 
          if (i<size-1 && !prefix("|",args_extanded[i+1],NULL)) return i;  // redirection symbol position
          else {
            return -2; //invalid redirection
          }
      } 
    }
  
  return -1;  // means no redirection
}

/**
 * @brief handle a command redirection
 *
 * @param redirection string that contains the redirection sign
 * @param filename the name of the destination file
 * @return return an array that contains the default descriptors of stdin dtdout
 * and stderr
 *
 */
int handle_redirection(char* redirection, char* filename) {
  int fd = 0;
  int result;
  if (strcmp(redirection, "<") ==0) {  // redirecting stdin of the command to the file filename

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
      perror("opening file error");
      return 1;
    }

    result = dup2(fd, STDIN_FILENO);
    if (result == -1) {
      perror("error redirecting stdin");
      return 1;
    }
    close(fd);
  } else {
    if (strcmp(redirection, ">") == 0) {
      fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0666);
      if (fd == -1) {
        perror("opening file error");
        return 1;
      }

      result = dup2(fd, STDOUT_FILENO);
      if (result == -1) {
        perror("error redirecting stdout");
        return 1;
      }
      close(fd);

    } else {
      if (strcmp(redirection, ">|") == 0) {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
          perror("opening file error");
          return 1;
        }

        result = dup2(fd, STDOUT_FILENO);
        if (result == -1) {
          perror("error redirecting stdout");
          return 1;
        }
        close(fd);

      } else {
        if (strcmp(redirection, ">>") == 0) {
          fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
          if (fd == -1) {
            perror("opening file error");
            return 1;
          }

          result = dup2(fd, STDOUT_FILENO);
          if (result == -1) {
            perror("error redirecting stdout");
            return 1;
          }
          close(fd);

        } else {
          if (strcmp(redirection, "2>") == 0 ||
              strcmp(redirection, "2>|") == 0 ||
              strcmp(redirection, "2>>") == 0) {
            if (strcmp(redirection, "2>") == 0) {
              fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0664);

            } else {
              if (strcmp(redirection, "2>|") == 0) {
                fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

              } else {
                fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
              }
            }

            if (fd == -1) {
              perror("opening file error");
              return 1;
            }

            result = dup2(fd, STDERR_FILENO);
            if (result == -1) {
              perror("error redirecting stderr");
              return 1;
            }
            close(fd);
          } else
            return 1;
        }
      }
    }
  }
  return 0;
}



// go back to standard descriptors for stdin stdout and stderr, after a
// redirection
void go_back_to_standard(int* fd_std) {
  dup2(fd_std[0], STDIN_FILENO);
  close(fd_std[0]);

  dup2(fd_std[1], STDOUT_FILENO);
  close(fd_std[1]);

  dup2(fd_std[2], STDERR_FILENO);
  close(fd_std[2]);
}

// execute cmd with a redirection
int cmd_with_redirection(char *cmd, char **args, int length,
                         int pos_redirection) {
  static int fd_standard[3];
  fd_standard[0] = dup(STDIN_FILENO);
  fd_standard[1] = dup(STDOUT_FILENO);
  fd_standard[2] = dup(STDERR_FILENO);
  int return_value = 0;
  char * sv=args[pos_redirection];
  
handle_rd:
  length = length - 2;
  //Met les bons descripteurs etc renvoie 0 si ça s'est bien passé args[length+1] c'est fic 
  return_value = handle_redirection(args[length], args[length + 1]);
  if (return_value == 0) {
    //il y a une redirection 
    if (contains_valid_redirection(args, length) > 0) {
      goto handle_rd;
    }
    sv = args[length]; // save the adress of the argument
    args[length] = NULL; // put the pointer to argumnt = null so that the commands will stop at null 
    if (strcmp(cmd, "exit") == 0) {
      return_value = my_exit(args + 1, length - 1);
    } else {
      if (strcmp(cmd, "cd") == 0) {
        return_value = my_cd(args + 1, length - 1);
      } else {
        if (strcmp(cmd, "pwd") == 0) {
          return_value = my_pwd(args + 1, length - 1);
        } else {
          // execute external command with the new extanded array of args
          return_value = extern_command(cmd, args);
        }
      }
    }
  }
  args[length]=sv;
  go_back_to_standard(fd_standard);
  return (return_value);
}

