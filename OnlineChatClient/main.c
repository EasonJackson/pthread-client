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
# define SERVER_PORT 8000
# define NUMBER_OF_THREADS 2

int* read_from_server() {
    return 0;
}

void* write_to_server() {

    return NULL;
}


int main(int argc, const char * argv[]) {
    
    pthread_t thread_pool[NUMBER_OF_THREADS];
    int thread_args[NUMBER_OF_THREADS];
    int result_code;
    
    // Initialize threads
    
    thread_args[0] = 0;
    result_code = pthread_create(&thread_pool[0], NULL, read_from_server, &thread_args[0]);
    thread_args[1] = 1;
    result_code = pthread_create(&thread_pool[1], NULL, write_to_server, &thread_args[1]);
    
    struct sockaddr_in server_address;
    int sock, valread;
    struct hostent *host_ent = gethostbyname(SERVER_ADDRESS);
    char lh = host_ent -> h_name;
    
    // Try to connect to server
    
    
    while (1) {
        
    }
    return 0;
}
