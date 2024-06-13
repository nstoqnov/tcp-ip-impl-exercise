//
//  main.c
//  IP protocol in C
//
//  Created by Nikolay Stoyanov on 12.06.24.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>

int server(void* unused) {
    // insert code here...
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    
    //Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Error while createing a socket \n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    //Set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //bind to the set port and IP
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        printf("Couldn't bind to the port \n");
        return -1;
    }
    printf("Done with bindng\n");
    
    //listen for clients:
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening \n");
        return -1;
    }
    printf("\nListening for incoming connections...\n");
    
    //Accept of incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        printf("Can't accept\n");
        return -1;
        }
    printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // Closing the socket:
//    close(client_sock);
//    close(socket_desc);
    return 0;
}

int client(void* unused){
    
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[100], client_message[100];
    
    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    
    // Get input from the user:
    //printf("Enter message: ");
    //scanf("%s", client_message);
    
    // Send the message to server:
    if(send(socket_desc, "Message from client", strlen("Message from client"), 0) < 0){
        printf("Unable to send message\n");
        return -1;
    }
    
    // Receive the server's response:
    if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }
    
    printf("Server's response: %s\n",server_message);
    
    // Close the socket:
    //close(socket_desc);
    
    return 0;
}

int main(int argx, const char * argv[]){
    pthread_t serv, cl;
    
    pthread_create(&serv, NULL, server, NULL);
    pthread_create(&cl, NULL, client, NULL);
    
    void* unused;
    pthread_join(serv, &unused);
    pthread_join(cl, &unused);
    return  0;
}

