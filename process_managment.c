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
    char *ptr;

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

        while (!feof(fptr)){
        
        	if (fgets(line,LINE_LEN, fptr) != NULL){
        		
        		sprintf(ptr, "%s", line);
        		ptr += strlen(line);
        	
        	}
        
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

        ptr = (char *) mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        
        char *dArr[1];
        char *bytes = ptr;
        int i = 0;
        int j = 0; 
        char temp[100];

        while (bytes[i] != '\0'){

            if (bytes[i] == '\n'){

                dArr[j]= malloc(strlen(temp) + 1 * sizeof(char *));
                strcpy(dArr[j], temp);
                //dArr[j] = temp;
                //printf("%s\n", dArr[j]);
                temp[0] = '\0';
                j += 1;
    			
            }else {
                strncat(temp, &bytes[i], 1);
            }
                i += 1;
            }

        j = 0;
        //display(dArr, ptr, 64);
        printf("%s \n", dArr[1]);
		



		//removes the shared memory object
        shm_unlink(name);

        //printf("%s", (char*)ptr);


    }
    return 0;
}


void display(char **array, char *bytes, int n){
    //printf("display: sfwf::\n");
    //printf("\n");

    int i = 0;
    int j = 0; 
    char temp[100];
    while (bytes[i] != '\0'){

        if (bytes[i] == '\n'){

            array[j] = malloc(strlen(temp) + 1 * sizeof(char));
            array[j] = temp;
            //printf("%s", temp);
            temp[0] = '\0';
            j += 1;

        }else {

            strncat(temp, &bytes[i], 1);
        }

        i += 1;

    }


}
