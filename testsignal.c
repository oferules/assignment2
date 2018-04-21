#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"


int flag = 1;
int pid;


void
user1Hanlder(int signum){
    printf(1, "I handle first signal %x\n", signum);
    flag = !flag;
 
}

void userHandlerTest(){
    /// test user handle signal
    printf(1, "\nuser handler test- create a child with hanlder\n");
    sighandler_t handler = user1Hanlder;
    signal(1, handler);
    
    pid=fork();
    if (pid==0){
        // new process handle Signal 1 with above function
        while(flag)  {}
        exit();
    }
    kill(pid, 1);
    wait();
    printf(1, "user handler test PASSED!\n");
}

void killSignalTest(){
    /// test signal kill
    /// create new child
    printf(1, "\nkill signal test- create a child and kill him\n");
    pid=fork();
    if (pid==0){
        // live forever
        while(1)  { printf(1, "my father is going to kill me !\n");}
        /// shouldnt reach here
        printf(1, "kill signal test FAILED!!\n");
        exit();
    }

    kill(pid, 9);
    wait();
    
    printf(1, "kill signal test PASSED!\n");    
}

void stopAndContSignalTest(){
    /// test signals stop and cont - should not see print of parent and child collisions
    /// create new child
    printf(1, "\nstop and cont signal test- create a child, stop him for a while and let him continue\n");
    pid=fork();
    if (pid==0){
        int i;
        for(i=0;i<81;i++){
            printf(1, "child action: %d\n", i);
        }
        exit();
    }
    
    int i;
    for(i=0;i<21;i++){
        printf(1, "now we fight!: %d\n", i);
    }

    /// stop the child
    kill(pid, 17);
    for(i=0;i<11;i++){
        printf(1, "I stopped my son haha: %d\n", i);
    }
    
    // continue the child
    kill(pid, 19);
    wait();
    
    printf(1, "stop and cont signals test PASSED!\n");
}


void signalMaskTest(){
    /// test signal mask- change it to block all signals and send a signal
    /// create new child
    printf(1, "\nsignal mask test- block signals and send to child\n");
    printf(1, "father block all signals tries to kill child \n");
    sigprocmask(0);
    
    pid=fork();
    if (pid==0){
        sleep(20);
        printf(1, "child un-block all signals and need to die right after\n");
        sigprocmask(0xffffffff);
        printf(1, "signal mask test FAILED\n");
        exit();
        
    }
    // try to kill the child
    kill(pid, 9);
    wait();
    printf(1, "signal mask test PASSED!\n");
}

void sleepTest(){
    printf(1, "\ntest sleep\n");

    pid=fork();
    if (pid==0){
        printf(1, "child going to sleep for a while, meanwhile father wait!\n");
        sleep(400);
        printf(1, "child wake up!\n");
        exit();
    }
    wait();

    printf(1, "sleep test PASSED!\n");

    
}

void casTest(){
    printf(1, "\ntest cas\n");
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
    printf(1, "cas test PASSED!\n");
}

int
main(int argc, char *argv[])
{
    printf(1, "\nenter signal tests\n");
    
    userHandlerTest();
    killSignalTest();
    stopAndContSignalTest();
    signalMaskTest();
    sleepTest();
    casTest();
    
    printf(1, "\nALL TESTS PASSED!\n");
    exit();
}


