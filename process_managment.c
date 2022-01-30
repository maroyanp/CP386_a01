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
#define BUFFER_SIZE 1500
#define READ_END 0
#define WRITE_END 1

void writeOutput(char *command, char *output);

int main(int argc, char *argv[]){
    const char *name = "OS";
    const int SIZE = 4096;
    char *file_name = argv[1];

    int shm_fd;
    char *ptr;

    //dynamic array
    char *dArr[1];

    //creating pipe first
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    int fd[2];

    if (pipe(fd) == -1){
        printf("Pipe failed");
        exit(1);
    }


    pid_t pid;
    pid = fork();

    //process managment 
    if (pid < 0){
    
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
        fptr = fopen(file_name, "r");
        //create the char array to hold string
        char line[LINE_LEN] = "";

        while (!feof(fptr)){
        
        	if (fgets(line,LINE_LEN, fptr) != NULL){
        		
        		ptr += sprintf(ptr, "%s", line);
        	
        	}
        
        }
        
        //closes the file
        fclose(fptr);

        //NOW WE CLOSE S.M
        if(close(shm_fd) == -1){
            printf("close failed");
        }
        
        exit(0);

    } else {
        //in parent process
        wait(NULL);  
        //printf("BEFORE SHM_OOPEN \n");

        shm_fd = shm_open(name, O_RDONLY, 0666);

        ptr = (char *) mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        
        
        char *bytes = ptr;
        int i = 0;
        int j = 0; 
        char temp[100];

        while (bytes[i] != '\0'){

            if (bytes[i] == '\n'){

                dArr[j] = malloc(strlen(temp) * sizeof(char*));
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

		//removes the shared memory object
        shm_unlink(name);


        //**** FINALY DYNAMIC ARRY CREATED AND now we create another child process to execute each command
        pid = fork();

        if (pid < 0 ){
            printf("something went wrong \n");
            exit(1);
        }else if( pid == 0){
            //child process again..

            //close the unused end of the pipe;
            close(fd[READ_END]);
            
            for(int i = 0; i < j; i++){
                FILE *fp = popen((dArr[i]), "r+");
                char buffer = 0;
                //char *str;
                char temp[BUFFER_SIZE] = "";
        
                while (fread(&buffer, sizeof(buffer), 1, fp)){
                    strncat(temp,&buffer, 1);
                }
                pclose(fp);
                printf("%s\n", temp);
                write(fd[WRITE_END], temp, strlen(temp) + 1);
                sleep(1);

            }
            // fprintf(fp, "%s", write_msg);

            close(fd[WRITE_END]);
            exit(0);

        }else {
            //in parent 2 process;
            
            close(fd[WRITE_END]);

            for (int i = 0; i < j; i++){
                sleep(1);
                char readMes[BUFFER_SIZE];
                read(fd[READ_END], readMes, BUFFER_SIZE);
                //printf("%s\n",readMes);
                writeOutput(dArr[i], readMes);
            }

            close(fd[READ_END]);
            wait(NULL);

        }

    }
    return 0;
}



void writeOutput(char *command, char *output){
    FILE *fp;
    fp = fopen("output.txt", "a");
    fprintf(fp, "The output of: %s : is \n", command);
    fprintf(fp, ">>>>>>>>>>>\n%s<<<<<<<<<<<\n", output);
    fclose(fp);
}