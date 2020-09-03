//
// Created by Daniil Tchyorny on 03.09.2020.
//

#ifndef ENTERINGTASK_SERVER_ROOM_H
#define ENTERINGTASK_SERVER_ROOM_H


#include "../session/session.h"
#include "../server.h"
#include <list>


constexpr uint16_t HISTORY_SIZE=128;
class Server;
class Session;
enum operation{
    NOT_COMMAND,
    MEMBER,
    CREATE,
    JOIN,
    LEAVE,
    DELETE,
};

class Room {
public:
    Room(Server *s) {master=s;};

    bool handler(const std::vector<std::string> &command, Session *member = nullptr, Room *r=nullptr);

    std::string nameGetter(){ return roomName; }

private:
    void addMember(Session *member);

    void deleteMe();

    void createMe(const std::vector<std::string> &name, Session *creator);

    void leaveRoom(Session *member);

    void historySizeCheck();

    void write(const std::vector<std::string> &message, Session *user);

    void write(const std::string &message);

    enum operation checkCommand(const std::string &message);

    static std::string wordsToSentence(const std::vector<std::string> &vec);

    void sendHistory(Session *newUser);

    void sendMembers(Session *asker);

private:
    std::string roomName;
    std::list<std::string> history;
    Server *master;
    std::mutex serverMuter, storyMuter;

};



#endif //ENTERINGTASK_SERVER_ROOM_H
