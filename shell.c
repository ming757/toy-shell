#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
 
#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	100
#define LEN_DIR   100

 
int main(void)
{

    char command[MAX_LEN_LINE];
    char *arg;
    char *arg1;
    char *arg2;
    char *arg3;
    char *args[] = {command, arg1, arg2, arg3, NULL};
    int ret, status;
    pid_t pid, cpid;

 

    char hostname[LEN_HOSTNAME + 1];
    memset(hostname, 0x00, sizeof(hostname));
    gethostname(hostname, LEN_HOSTNAME);

 
    while (true) {

        char *s;
        int len;
        
        char pwd[100];
        getcwd(pwd,100);

 

        printf("\033[35m%s\033[32m@%s:\033[34m%s\033[0m$ ", getpwuid(getuid())->pw_name, hostname,pwd);

        s = fgets(command, MAX_LEN_LINE, stdin);

        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);

        }

        len = strlen(command);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 

        }

        arg = strtok(command," ");
	    arg1 = strtok(NULL, " ");
	    arg2 = strtok(NULL, " ");
	    arg3 = strtok(NULL, " ");
	    args[1] = arg1;
	    args[2] = arg2;
	    args[3] = arg3;

        if (!strcmp(arg, "exit")){

              return 0;
            }
        else if (!strcmp(arg, "cd")){
		        if(chdir(arg1)<0){
			            printf("ERROR : There is no directory [%s].\n",arg);
		        }
		        else{
			            printf("successful!\n");
			    }
	    }
            
         printf("[%s]\n", command);

      

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);

        } 

        if (pid != 0) {  /* parent */
           cpid = waitpid(pid, &status, 0);
           if (cpid != pid) {
              fprintf(stderr, "waitpid failed\n");        

            }

            printf("Child process terminated\n");

            if (WIFEXITED(status)) {

                printf("Exit status is %d\n", WEXITSTATUS(status)); 

            }

        }

        else {  /* child */

             if (!strcmp(arg, "mkdir")){
		    args[0] = "/bin/mkdir";
	    }

	    else if (!strcmp(arg, "ls")){
		    args[0] = "/bin/ls";
	    }

	    else if (!strcmp(arg, "mv")){
		    args[0] = "/bin/mv";
        }
        
        else if (!strcmp(arg, "cp")){
		    args[0] = "/bin/cp";
	    }

	    else if (!strcmp(arg, "date")){
		    args[0] = "/bin/date";
	    }
	    
	
        ret = execve(args[0], args, NULL);

            if (ret < 0) {

                fprintf(stderr, "execve failed\n");   

                return 1;

            }

        } 

    }

    return 0;

}
