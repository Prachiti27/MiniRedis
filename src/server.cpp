#include "server.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

Server::Server(int port){
    this->port = port;
}

void Server::start(){
    //creating socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
        perror("Socket creation failed");
        return;
    }

    //define server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); //htons = Host TO Network Short
    server_addr.sin_addr.s_addr = INADDR_ANY; //Listen on ALL network interfaces

    //Bind socket to IP and port
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        return;
    }

    //listening
    if(listen(server_fd,1)<0){
        perror("Listen failed");
        return;
    }

    cout<<"Server listening to port "<<port<<endl;

    //accept in client side
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr,&client_len);
    if(client_fd < 0){
        perror("Accept failed");
        return;
    }

    cout<<"Client connected"<<endl;

    while(true){
        sleep(1);
    }
}