#include "types.h"
#include "stat.h"
#include "user.h"


void user1Hanlder(int);

int
main(int argc, char *argv[])
{
    printf(1, "enter signal tests\n");
    sighandler_t handler= user1Hanlder;
    signal(1,handler);
    
    int pid=fork();
    if (pid==0){
        // new process handle Signal 1 with above function

        while(1)  {} 
    }
    
    printf(1,"created process with pid: %d\n", pid);
    
    
    kill(pid,1);
    wait();
    
    

    
    
    
    printf(1, "all signal tests passed\n");
    exit();
}


void
user1Hanlder(int signum){
    printf(1, "I handle signal %d\n", signum);
}