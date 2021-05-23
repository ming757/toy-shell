#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30
#define LEN_DIR     100

int main(int argc, char *argv[], char *envp[])
{
    char command[MAX_LEN_LINE]; //array
    char hostname[LEN_HOSTNAME + 1];

    char *arg;
    char *arg1;
    char *arg2;
    char *arg3;
    char *args[] = {command, arg1, arg2, arg3, NULL};

    int ret, status;
    pid_t pid, cpid;
    int flag = 0;
    
    while (true) {
        char *s; // input sting storage
        int len; // lenth of command

        memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);

        char cwd[LEN_DIR + 1];
        getcwd(cwd, sizeof(cwd));
        printf("\033[36m%s\033[36m@%s:\033[35m%s\033[0m$ ", getpwuid(getuid())->pw_name, hostname,cwd);

        s = fgets(command, MAX_LEN_LINE, stdin); // function reading strings

        if (s == NULL) { // if not success -> raise error
            // fprintf(stderr, "fgets failed\n");
            exit(1);
        }

        len = strlen(command);
        if (command[len - 1] == '\n') { // end of command "\n" -> end of string "\0"
            command[len - 1] = '\0';
        }

        arg = strtok(command," ");
        arg1 = strtok(NULL, " ");
        arg2 = strtok(NULL, " ");
        arg3 = strtok(NULL, " ");
        args[1] = arg1;
        args[2] = arg2;
        args[3] = arg3;

        if (strcmp(command, "exit") == 0){
            return -1;
        }

        if (strcmp(command, "cd") == 0){
            if(chdir(args[1])<0){
                printf("bash: cd: %s: No such file or directory\n", args[1]);
            }
            flag = 1;
        }

        if (strcmp(arg, "clear") == 0){
            write (1, "\033[1;1H\033[2J",10);
            flag = 1;
        }
        
        // printf("[%s]\n", command); // print command

        pid = fork(); // fork fail

        if (pid < 0) { // fork fail
            // printf(stderr, "fork failed\n");
            exit(1);
        }

        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                // fprintf(stderr, "waitpid failed\n");        
            }
            // printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                // printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }

        else {  /* child */
        /*1 arg*/
            if (strcmp(arg, "ls") == 0){
				args[0] = "/bin/ls";
			}

			else if (strcmp(arg, "mkdir") == 0){
				args[0] = "/bin/mkdir";
			}

			else if (strcmp(arg, "rmdir") == 0){
				args[0] = "/bin/rmdir";
			}

			else if (strcmp(arg, "mv") == 0){
				args[0] = "/bin/mv";
			}

			else if (strcmp(arg, "rm") == 0){
				args[0] = "/bin/rm";
			}

            else if (strcmp(arg, "chmod") == 0){
				args[0] = "/bin/chmod";
			}
            else if (strcmp(arg, "cp") == 0){
				args[0] = "/bin/cp";
			}
            else if (strcmp(arg, "pwd") == 0){
				args[0] = "/bin/pwd";
			}
            else{
                if (!flag){
                    printf("%s: command not found\n", args[0]);
                }
                
            }

        /*execution part*/
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                // fprintf(stderr, "execve failed\n");   
                return 1;
            }
        }
        flag = 0;
    }
    return 0;
}

