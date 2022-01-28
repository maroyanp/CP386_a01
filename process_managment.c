#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define LINE_LEN  80

int main(){
    const char *name = "OS";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;

    pid_t pid;
    pid = fork();

    //process managment 
    if (pid == -1){
    
        printf("Creating child process failed");
    
    }else if (pid == 0) { 
        // in child process
        //printf("hello world");
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

        ftruncate(shm_fd, SIZE);

        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        
        //now we open file 
        FILE *fptr;
        //open file
        fptr = fopen("./sample_in.txt", "r");
        //create the char array to hold string
        char line[LINE_LEN] = "";

        while (fgets(line, LINE_LEN, fptr)){
            //writes to shared mem
            strcpy(ptr, line);
            ptr += strlen(line) + 1;    
            //printf( "%s", line);
        }
        
        //closes the file
        fclose(fptr);

        //NOW WE CLOSE S.M
        if(close(shm_fd) == -1){
            printf("close failed");
        }
        

    } else {
        //in parent process
        wait(NULL);  
        printf("BEFORE SHM_OOPEN \n");

        shm_fd = shm_open(name, O_RDONLY, 0666);

        ptr = (char*) mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        //printf("%s", (char*)ptr);
		
		int i = 1;
		
		
		while (i <= 5){
			printf("%s \n",ptr );
			ptr += strlen(ptr) +1;
			i += 1;
		}
		
        //printf("%s", TEMP);


    }
    return 0;

}
