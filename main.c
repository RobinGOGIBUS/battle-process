#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "struct_members.h"

const char* currentMember;

const struct Members members = {"PERE", "FILS"};

int gameInProgress;

int points;

int pid;


void displayAction(int sig)
{
    switch (sig)
    {
        case SIGTERM:
            points -= 1;
            if(points < 1){
                kill(pid, SIGUSR2);
            }else{
                kill(pid, SIGUSR1);
            }        
            break;
        case SIGUSR1:
            if(currentMember == members.father){
                printf("%s[%d] : Aïes, je suis touché \n", members.child, pid);
            }else{
                printf("%s[%d] : Aïes, je suis touché \n", members.father, pid);
            }
            break;
        case SIGUSR2:
            if(currentMember == members.father){
                printf("%s[%d] : Arghhh !! je meurs ! \n", members.child, pid);
            }else{
                printf("%s[%d] : Arghhh !! je meurs ! \n", members.father, pid);
            }
            gameInProgress = 0;
            break;
        default:
            break;
    }
}



int main(int argc, char** argv)
{
    srand(time(NULL));
    int fatherPoints = (rand() % 5) + 5;
    int childPoints = (rand() % 5) + 5;
    gameInProgress = 1;
    signal(SIGTERM, displayAction);
    signal(SIGUSR1, displayAction);
    signal(SIGUSR2, displayAction);
    pid = fork();
    if(pid != 0){
        currentMember = members.father;
        points = fatherPoints;
        usleep(10);
        printf("%s[%d] : Nombre de points initial : %d \n", currentMember, getpid(), points);
    }else{
        currentMember = members.child;
        points = childPoints;
        pid = getppid();
        printf("%s[%d] : Nombre de points initial : %d \n", currentMember,  getpid(), points);
    }
    while(gameInProgress){
        if(points < 1){
            kill(pid, SIGUSR2);
            return 0;
        }
        printf("%s[%d] : Pan ! \n", currentMember, getpid());
        kill(pid, SIGTERM);
        pause();    
        
    }
    if(points >= 1){
        printf("%s[%d] : J'ai gagné !, nombre de points : %d \n", currentMember, getpid(), points);
    }
    return 0;
}