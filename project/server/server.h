//
// Created by Daniil Tchyorny on 02.09.2020.
//

#ifndef ENTERINGTASK_SERVER_SERVER_H
#define ENTERINGTASK_SERVER_SERVER_H

#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "./session/session.h"
#include "./room/room.h"

class Session;
class Room;

class Server {
public:
    Server(uint32_t Port);

    int start();

    void stop();

    std::unordered_map<Room *, std::vector<Session *>> *getRooms();

    Room * getLobby(){ return defaultRoom; }

private:
    void acceptor(int listenSocket);

    void clientHandler(int clientSocket);

private:
    uint32_t port;
    std::unordered_map<int, std::thread> users;
    std::unordered_map<Room *, std::vector<Session *>> chats;
    Room *defaultRoom;
    std::mutex muter;
};


#endif //ENTERINGTASK_SERVER_SERVER_H
