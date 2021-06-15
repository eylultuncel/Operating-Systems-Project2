#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#define DATASIZE 68*14

// struct definition for node
typedef struct Node{
    char data[DATASIZE];
    struct Node *nextNode;
} Node;

Node *bottomStackPointer = NULL;
Node *topStackPointer = NULL;

char * FILENAME = ".txt";

//function declarations for queue
void printStack(void);
int push(char data[]);

//function for concatenating two strings
char* concat(const char *s1, const char *s2);

//the thread function
void *connection_handler(void *);

//main function
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

    //creating socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    //get the port number from arguments
    server.sin_port = htons( atoi(argv[2]) );

    //get filename from arguments and concatenate with ".txt"
    FILENAME = concat(argv[6],FILENAME);

    //bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //listen socket
    listen(socket_desc , 3);

    //accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}


//this function will handle connection for each client
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message ;
    FILE * fp,fp2;
    char * line = NULL;
    char * frame = "";
    size_t len = 0;
    int i = 0;
    ssize_t read;

    //open video chanels file
    fp = fopen(FILENAME, "a");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //put one more line ending to the end of file and close
    fputs("\n", fp);
    fclose(fp);

    //then open again for read operation
    fp= fopen(FILENAME,"r");

    while ((read = getline(&line, &len, fp)) != -1) {

        //get 14 lines and concat with frame
        frame = concat(frame,line);
        i++;

        //14 lines end up to one frame
        if (i == 14){
            //push the frame to the stack
            push(frame);
            frame = "";
            i=0;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    if (frame)
        free(frame);

    //there is a stack with all frames
    Node *TempPointer = bottomStackPointer;

    //in endless loop, write every frame to the socket
    //this way streaming never ends unless clients ends it
    while(TempPointer != NULL) {
        message = TempPointer->data;
        write(sock, message, strlen(message));
        TempPointer = TempPointer->nextNode;

        if(TempPointer == NULL){
            TempPointer = bottomStackPointer;
        }
    }


    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }


    return 0;
}

//function for concatenating two strings
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result < 0){
        printf("error when memory allocation.");
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void printStack(void){
    Node *TempPointer = bottomStackPointer;
    while(TempPointer != NULL){
        printf("%s\n",TempPointer->data);
        TempPointer = TempPointer->nextNode;
    };
    return;
}

//push a node to the stack
int push(char data[]) {
    Node *TempPointer = malloc(sizeof(Node));
    if (TempPointer == NULL) return 0;
    strcpy(TempPointer->data, data);
    TempPointer->nextNode = NULL;
    if (bottomStackPointer == NULL) TempPointer = bottomStackPointer = TempPointer;
    else topStackPointer->nextNode = TempPointer;
    topStackPointer = TempPointer;
    return 1;
}
