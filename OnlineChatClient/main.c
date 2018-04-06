//
//  main.c
//  OnlineChat
//
//  Created by EasonJackson on 3/14/18.
//  Copyright © 2018 EasonJackson. All rights reserved.
//

# include <stdio.h>
# include <pthread.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <ctype.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h>

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
    int server_socket_fd = 0, client_socket_fd = 0;
    int set_reuse_addr = 1;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    // Initialize threads
    if (0 > (pthread_create(&read_thread, NULL, read_from_server(recvmessage,
                                                                 &server_socket_fd,
                                                                 &client_socket_fd), NULL))) {
        fprintf(stderr, "Cannot create new thread.");
    }
    
    if (0 > (pthread_create(&write_thread, NULL, write_to_server(sendmessage,
                                                                 &server_socket_fd,
                                                                 &client_socket_fd), NULL))) {
        fprintf(stderr, "Cannot create new thread.");
    }
    
    int sock, valread;
    // Converts localhost into 0.0.0.0
    struct hostent *host_ent = gethostbyname(SERVER_ADDRESS);
    unsigned long server_add_nbo = *(unsigned long*)(host_ent ->h_addr_list[0]);
    
    // Try to connect to server
    // 1. Create socket IPv4
    if (0 > (client_socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "Client failed to create socket\n");
        exit(1);
    }
    
    // 2. Configure socket address structure
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    
    
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);
    return 0;
}

void* read_from_server(char recvmessage[BUFFER_SIZE],
                       int* server_socket_fd_p,
                       int* client_socket_fd_p) {
    while(1) {
        bzero(recvmessage, BUFFER_SIZE);
        ssize_t num_bytes = read(*server_socket_fd_p, recvmessage, BUFFER_SIZE);
        
        if (num_bytes == 0) {
            fprintf(stderr, "Client cannot read from server");
        } else {
            fprintf(stdout, "Message from server %d: %s", *server_socket_fd_p, recvmessage);
        }
    }
    return NULL;
}

void* write_to_server(char sendmessage[BUFFER_SIZE],
                      int* server_socket_fd_p,
                      int* client_socket_fd_p) {
    while(1) {
        bzero(sendmessage, BUFFER_SIZE);
        
        fgets(sendmessage, BUFFER_SIZE, stdin);
        if (0 > write(*client_socket_fd_p, sendmessage, BUFFER_SIZE)) {
            fprintf(stderr, "Client cannot send message to server");
        }
    }
    return NULL;
}
