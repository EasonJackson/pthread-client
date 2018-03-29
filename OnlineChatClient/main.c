//
//  main.c
//  OnlineChat
//
//  Created by EasonJackson on 3/14/18.
//  Copyright © 2018 EasonJackson. All rights reserved.
//

# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <pthread.h>
# include <stdio.h>
# include <netdb.h>

# define SERVER_ADDRESS "localhost"
# define SERVER_PORT 8080
# define CLIENT_PORT 8000

void* read_from_server(char[], int*, int*);
void* write_to_server(char[], int*, int*);

static const ssize_t BUFFER_SIZE = 1024;
static char sendmessage[BUFFER_SIZE];
static char recvmessage[BUFFER_SIZE];

int main(int argc, const char * argv[]) {
    
    pthread_t read_thread, write_thread;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    // Initialize threads
    
    
    if (0 > (pthread_create(&read_thread, NULL, read_from_server, NULL))) {
        fprintf(stderr, "Cannot create new thread.");
    }
    
    if (0 > (pthread_create(&write_thread, NULL, write_to_server, NULL))) {
        fprintf(stderr, "Cannot create new thread.");
    }
    
    int sock, valread;
    struct hostent *host_ent = gethostbyname(SERVER_ADDRESS);
    char lh = host_ent -> h_name;
    
    // Try to connect to server
    
    
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);
    return 0;
}

void* read_from_server() {
    return NULL;
}

void* write_to_server() {
    return NULL;
}
