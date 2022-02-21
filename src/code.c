#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

int main(int argc, char **argv)
{
    // code de message d'erreur pour le nombre d'argument
    if (argc < 2)
    {
        perror("Forget programm name.\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    char *cmds[] = {argv[0], NULL};

    if (pid < 0)
    {
        perror("fork fail. \n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // child process
        printf("*************** Processus enfant **************** \n");
        printf("PID : %d ,   PPID : %d ,   GID : %d ", (int)getpid(), (int)getppid(), (int)getgid());

        //============== envoi de la requête PTRACE_TRACEME au parent===============
        if (ptrace(PTRACE_TRACEME, pid, NULL, NULL) < 0)
        {
            perror("ptrace fail.\n");
            exit(EXIT_FAILURE);
        }

        //=============== charger le fichier l'éxécutable de notre programme et l'exécuter

        execvp(cmds[0], cmds);
    }

    else
    {
        // parent process

        printf("**************Processus parent ******************\n");
        int status;
        int child_pid = waitpid(pid,&status,0);
        
        if(WIFSTOPPED(status))
        {
            printf("Terminated by receiving the %d signal." , (int) WSTOPSIG(status));
        }

        //printf("PID : %d  ,   PPID :  %d ,  GID : %d ", (int)getpid(), (int)getppid(), (int)getgid());
    }
    
    return 0;
}