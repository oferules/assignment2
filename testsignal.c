#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

void user1Hanlder(int);
void user2Hanlder(int);

int flag = 1;

int
main(int argc, char *argv[])
{
    printf(1, "enter signal tests\n");
    
    /// test user handle signal
    sighandler_t handler = user1Hanlder;
    signal(1, handler);
    
    int pid=fork();
    if (pid==0){
        // new process handle Signal 1 with above function
        while(flag)  {}
        printf(1, "flag off\n");
        exit();
    }
    printf(1, "created process with pid: %d\n", pid);
    kill(pid, 1);
    wait();
    printf(1, "user handle test passed!\n");
    
    
    /// test signal kill
    /// create new child
    pid=fork();
    if (pid==0){
        // live forever
        while(1)  {}
        
        /// shouldnt reach here
        printf(1, "kill signal test FAILED!!\n");
        exit();
    }
    printf(1, "created process with pid: %d\n", pid);
    kill(pid, 9);
    wait();
    
    printf(1, "kill signal test passed!\n");
    
    
    /// test signals stop and cont - should not see print of parent and child collisions
    /// create new child
    pid=fork();
    if (pid==0){
        int i;
        for(i=0;i<200;i++){
            printf(1, "child action: %d\n", i);
        }
        exit();
    }
    
    int i;
    for(i=0;i<20;i++){
        printf(1, "now we fight!: %d\n", i);
    }

    /// stop the child
    kill(pid, 17);
    for(i=0;i<20;i++){
        printf(1, "I stopped my son haha: %d\n", i);
    }
    
    // continue the child
    kill(pid, 19);
    wait();
    
    printf(1, "stop and cont signals test passed!\n");
    
    printf(1, "all signal tests passed!\n");
    
    
    printf(1, "test cas\n");
    int inadd=0;
    int expected= 0;
    int newval=1;
    int bol= cas(&inadd,expected,newval);

    if(!bol || inadd!=1 ){
        printf(1, "cas test FAILED\n");
    }
    
    inadd=0;
    expected= 1;
    newval=2;
    bol= cas(&inadd,expected,newval);

    if(bol || inadd!=0 ){
        printf(1, "cas test FAILED\n");
    }
    printf(1, "cas test passed!\n");
    exit();
}

void
user1Hanlder(int signum){
    printf(1, "I handle first signal %x\n", signum);
    flag = !flag;
 
}
