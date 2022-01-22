#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>



int main(){

pid_t pid;
int x = 30;	//sleep timer for the Zombie to be alive (sec)

    /* fork a child process */
    pid = fork();
  
    if (pid < 0) { /* error occured */
        
	fprintf(stderr, "Fork Failed");
        return 1;

    } else if (pid > 0 ) {/*parent process */
	system("echo In Parent Porcess!");
	sleep(x);
        // 	parent will wait for the child to complete 
        //	wait(NULL);  dont call wait to see if child process still exsist
        //	printf("Child Complete \n");
	//	printf("PID of parent ==> %d \n", getpid());

    } else { //child process
        system("echo In Child Process!");
	exit(0);

    }


    return 0;
    


}

