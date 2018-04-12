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

void* read_from_server(char[], int*);
void* write_to_server(char[], char[], int*);

static const ssize_t BUFFER_SIZE = 1024;
static char sendmessage[BUFFER_SIZE];
static char recvmessage[BUFFER_SIZE];

int main(int argc, const char * argv[]) {
    
    pthread_t read_thread, write_thread;
    
    int server_socket_fd = 0;
    struct sockaddr_in server_address;
    
    // Converts localhost into 0.0.0.0
    struct hostent *host_ent = gethostbyname(SERVER_ADDRESS);
    unsigned long server_add_nbo = *(unsigned long*)(host_ent ->h_addr_list[0]);
    
    // Try to connect to server
    // 1. Create socket IPv4
    if (0 > (server_socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "Client failed to create socket\n");
        exit(1);
    } else {
        fprintf(stdout, "Client created socket...\n");
    }
    
    // 2. Configure socket address structure
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    //server_address.sin_addr.s_addr = server_add_nbo;
    
    inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));
    
    // 3. Connect client socket to server
    if (0 > connect(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address))) {
        fprintf(stderr, "Client failed to connect to serverat %s:%d\n", SERVER_ADDRESS, SERVER_PORT);
        close(server_socket_fd);
        exit(1);
    } else {
        fprintf(stdout, "Client connected to server at %s:%d\n", SERVER_ADDRESS, SERVER_PORT);
    }
    
    while (1) {
        bzero(sendmessage, BUFFER_SIZE);
        bzero(recvmessage, BUFFER_SIZE);
        
        fgets(sendmessage, BUFFER_SIZE, stdin);
        write(server_socket_fd, sendmessage, BUFFER_SIZE);
        read(server_socket_fd, recvmessage, BUFFER_SIZE);
        fprintf(stdout, "%s", recvmessage);
    }
    
//    // Initialize threads
//    if (0 > (pthread_create(&read_thread, NULL, read_from_server(recvmessage,
//                                                                 &server_socket_fd), NULL))) {
//        fprintf(stderr, "Cannot create new thread.");
//    }
//
//    if (0 > (pthread_create(&write_thread, NULL, write_to_server(sendmessage,
//                                                                 recvmessage,
//                                                                 &server_socket_fd), NULL))) {
//        fprintf(stderr, "Cannot create new thread.");
//    }
//
//    pthread_join(read_thread, NULL);
//    pthread_join(write_thread, NULL);
    close(server_socket_fd);
    return 0;
}

void* read_from_server(char recvmessage[BUFFER_SIZE],
                       int* server_socket_fd_p) {
    fprintf(stdout, "Created a read thread...\n");
    
    while(1) {
        bzero(recvmessage, BUFFER_SIZE);
        ssize_t num_bytes = read(*server_socket_fd_p, recvmessage, BUFFER_SIZE);

        if (num_bytes == 0) {
            fprintf(stderr, "Client cannot read from server\n");
        } else {
            fprintf(stdout, "Message from server %d: %s", *server_socket_fd_p, recvmessage);
        }
    }
    return NULL;
}

void* write_to_server(char sendmessage[BUFFER_SIZE],
                      char recvmessage[BUFFER_SIZE],
                      int* server_socket_fd_p) {
    fprintf(stdout, "Created a write thread...\n");
    
    while(1) {
        bzero(sendmessage, BUFFER_SIZE);
        bzero(recvmessage, BUFFER_SIZE);
        
        char* res = fgets(sendmessage, BUFFER_SIZE, stdin);
        fprintf(stdout, "Get message from stdin\n");

        if (res != NULL) {
            if (0 > write(*server_socket_fd_p, sendmessage, BUFFER_SIZE)) {
                fprintf(stderr, "Client cannot send message to server\n");
            } else {
                fprintf(stdout, "Send a message to server\n");
            }
        }
        
        read(*server_socket_fd_p, recvmessage, BUFFER_SIZE);
        fprintf(stdout, "%s", recvmessage);
    }
    
    return NULL;
}
