#include "server.h"
#include "parser.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

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

    char buffer[1024]; 

    while(true){
        memset(buffer,0,sizeof(buffer)); //clear buffer to remove grabage data if any

        int bytes_read = read(client_fd, buffer, sizeof(buffer));

        if(bytes_read<0){
            perror("Error reading the input");
            break;
        }

        if(bytes_read == 0){
            cout<<"Client Disconnected"<<endl;
            break;
        }

        string input(buffer, bytes_read);

        Command cmd = Parser::parse(input);

        for(int i=0;i<cmd.args.size();i++){
            if(cmd.name == "SET"){
                if(cmd.args.size()<2){
                    cout<<"(error) wrong number of arguments for SET"<<endl;
                }
                else{
                    db.set(cmd.args[0],cmd.args[1]);
                    cout<<"OK"<<endl;
                }
            }
            else if(cmd.name=="GET"){
                if(cmd.args.size()<1){
                    cout<<"(error) wring number of arguments for GET"<<endl;
                }
                else{
                    auto val = db.get(cmd.args[0]);
                    if(val.has_value()) cout<<val.value()<<endl;
                    else cout<<"(nil)"<<endl;
                }
            }
            else if(cmd.name == "DEL"){
                if(cmd.args.size()<1){
                    cout<<"(error) wrong number of arguments for DEL"<<endl;
                }
                else{
                    bool removed = db.del(cmd.args[0]);
                    cout<<(removed ? 1 : 0)<<endl;
                }
            }
            else if(cmd.name == "INCR"){
                if(cmd.args.size()<1){
                    cout<<"(error) wring number of arguments for INCR"<<endl;
                }
                else{
                    auto num = db.incr(cmd.args[0]);
                    if(num.has_value()) cout<<num.value()<<endl;
                    else cout<<"(error) value is not an integer"<<endl;
                }
            }
            else if(cmd.name=="EXIT\r\n"){
                cout<<"Client requested EXIT. Closing connection."<<endl;
                close(client_fd);
                break;
            }
            else{
                cout<<"(error) unkonwn command "<<cmd.name<<endl; 
            }
        }
    }
}