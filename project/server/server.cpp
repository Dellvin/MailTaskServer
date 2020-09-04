//
// Created by Daniil Tchyorny on 02.09.2020.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <unistd.h>
#include "server.h"


Server::Server(uint32_t Port) : port(Port) {
    defaultRoom=new Room(this);
    std::vector<Session*> enteredClients;
    chats.insert(std::pair<Room *, std::vector<Session *>>(defaultRoom, enteredClients));
}

int Server::start() {
    int32_t listenSocket;
    int32_t opt=1;//variable for saving port
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=INADDR_ANY;
    listenSocket=socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket==-1) {
        std::cerr<<"Listening error"<<std::endl;
        return 1;
    }
    if(setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){ // saving port after server fault
        std::cerr<<"Socket port error";
        return 2;
    }
    if(bind(listenSocket, (sockaddr *)&addr, sizeof(addr))){
        std::cerr<<"Binding error";
        return 3;
    }
    if(listen(listenSocket, SOMAXCONN)==-1){
        std::cerr<<"Listening error";
        return 4;
    }
    signal(SIGPIPE, SIG_IGN);// ignoring this for safety
    acceptor(listenSocket);
    return 0;
}

void Server::acceptor(int listenSocket) {
    std::cout<<"Waiting for connections..."<<std::endl;
    while (true){
        int clientSock=accept(listenSocket, 0, 0);
        if(clientSock==-1){
            std::cerr<<"User connection error";
            continue;
        }
        try{
            std::thread connection(&Server::clientHandler, this,  clientSock);
            muter.lock();
            users.insert(std::pair<int, std::thread>(clientSock, std::move(connection)));
            muter.unlock();
        }catch (const std::exception &ex){
            std::cerr<<ex.what()<<std::endl;
            std::string error="Connection error, sorry(";
            send(clientSock, error.c_str(), error.size(), 0);
            shutdown(clientSock, 2);
            close(clientSock);
        }
    }
}

void Server::clientHandler(int clientSocket) {
    std::cout<<"Client with socket number "<<clientSocket<<" has been connected!"<<std::endl;
    Session s(clientSocket, this);
    s.handling();
    muter.lock();
    users.at(clientSocket).detach();
    users.erase(clientSocket);
    muter.unlock();
    shutdown(clientSocket, 2);
    close(clientSocket);
}

void Server::stop() {
    for(auto &user:users){
        std::string byeMessage="Bye bye)";
        if(user.first){
            send(user.first, byeMessage.c_str(), byeMessage.size(), 0);
            shutdown(user.first, 2);
            close(user.first);
        }
        user.second.join();
    }
    users.clear();
}

std::unordered_map<Room *,std::vector<Session*>> *Server::getRooms() { return &chats; }



