//
// Created by Daniil Tchyorny on 02.09.2020.
//

#ifndef ENTERINGTASK_SERVER_SESSION_H
#define ENTERINGTASK_SERVER_SESSION_H

#include <string>
#include <vector>
#include "../server.h"
#include "../room/room.h"

class Server;

class Room;

constexpr uint16_t buffSize = 1024;

enum state {
    REGISTRATION,
    LOBBY,
    IN_ROOM,
    EXIT,
};

class Session {
public:
    Session(int socket, Server *boss) : sock(socket) { master = boss; }

    void handling();

    void sender(const std::string &message);

    std::string nicknameGetter() { return nickname; }

    void setState(state currentState) { flag = currentState; }

    void resetRoom() { currentRoom = nullptr; }

private:
    void checkMessage();

    void moveBuff(uint8_t distance);

    void parsing(const std::string &rowMessage);

    static uint8_t skipBreakers(const std::string &rowMessage, uint8_t beginPos);

    void execute();

    void login();

    static bool correctLobbyCommand(const std::string &command);

    bool findRoomByName(const std::string &name);

    void sendRooms();

    bool checkDeleteAbility();

private:
    std::vector<std::string> request;
    int sock;
    char buff[buffSize];
    uint16_t lenBuf = 0;
    state flag = REGISTRATION;
    std::string nickname;
    Server *master;
    Room *currentRoom;
    std::mutex serverMuter, roomMuter;
};


#endif //ENTERINGTASK_SERVER_SESSION_H
