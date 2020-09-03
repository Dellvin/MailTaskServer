//
// Created by Daniil Tchyorny on 02.09.2020.
//

#include <unistd.h>
#include <cstring>
#include "session.h"
#include <iostream>
#include "constexp.h"





void Session::handling() {
    sender(HELLO_MESSAGE);
    uint16_t bytesCount;
    while(true){
        if(flag==EXIT) break;
        bytesCount=read(sock, buff+lenBuf, buffSize-lenBuf);
        if(bytesCount<=0) break;
        lenBuf+=bytesCount;
        checkMessage();
        if(lenBuf>=buffSize)lenBuf=0;
    }
}

void Session::sender(const std::string &message) {
    write(sock, message.c_str(), message.size());
}

void Session::checkMessage() {
    while(true){
        int16_t lfPos=-1;
        for(int16_t i=0;i<lenBuf;++i){
            if(buff[i]=='\n'){
                lfPos=i;
                break;
            }
        }
        if(lfPos==-1)break;
        std::string message;
        for(uint8_t i=0;i<lfPos;++i){
            message+=buff[i];
        }
        message[lfPos]='\0';
        if (lfPos&&message[lfPos-1]=='\r')
            message[lfPos-1]='\0';
        moveBuff(lfPos);
        parsing(message);
        execute();
    }
}


void Session::moveBuff(uint8_t distance) {
    uint16_t j=0;
    for(uint16_t i=distance+1;i<lenBuf;++i){
        buff[j]=buff[i];
        j++;
    }
    lenBuf-=distance+1;
}

void Session::parsing(const std::string &message) {
    uint8_t wordCount=0;
    for(uint8_t beginPos=0; beginPos < message.size(); ++beginPos){
        uint8_t endPos=skipBreakers(message, beginPos);
        std::string word;
        for(;beginPos<endPos;++beginPos){
            if(message[beginPos]!='\0')
                word+=message[beginPos];
        }
        if(!word.empty()){
            request.push_back(word);
            wordCount++;
        }
    }
}

uint8_t Session::skipBreakers(const std::string &rowMessage, uint8_t beginPos) {
    uint8_t pos=beginPos;
    for(;pos<rowMessage.size();++pos){
        if(rowMessage[pos]==' ' || rowMessage[pos]=='\t'){
            pos=pos;
            break;
        }
    }
    return pos;
}

void Session::execute() {
    if(flag==REGISTRATION) login();
    else if(flag==LOBBY){
        if(!correctLobbyCommand(request[0])) {
            sender(UNDEFINE_MESSAGE);
        }
        else if(request[0]=="#help")sender(HELP_MESSAGE);
        else if(request[0]=="#exit"){
            sender(EXIT_MESSAGE);
            flag=EXIT;
        }
        else if(request[0]=="#create"){
            currentRoom=new Room(master);
            currentRoom->handler(request, this);
            flag=IN_ROOM;
        }
        else if(request[0]=="#join"){
            if(findRoomByName(request[1])){
                currentRoom->handler(request, this);
                flag=IN_ROOM;
            }
        }
        else if(request[0]=="#show") sendRooms();
    }
    else if(flag==IN_ROOM){
        if(request[0]=="#delete"){
            if(checkDeleteAbility()){
                currentRoom->handler(request,this);
                delete currentRoom;
            }
        }
        else
            currentRoom->handler(request,this);
    }
    request.clear();
}

bool Session::correctLobbyCommand(const std::string &command){
    return command.find('#') == 0;
}

void Session::login() {
    if(request.empty())return;
    if(request.size()>1){
        sender(TOO_MUCH_LOGIN);
        sender(LOGIN_TRY);
        return;
    }
    if(request[0].empty()){
        sender(TOO_MUCH_LOGIN);
        sender(EMPTY_LOGIN);
        return;
    }
    serverMuter.lock();
    std::unordered_map<Room *,std::vector<Session*>> *rooms=master->getRooms();
    for(const auto &room: *rooms){
        for(const auto &user: room.second){
            if(user->nickname == request[0]) {
                sender(USED_LOGIN);
                serverMuter.unlock();
                sender(LOGIN_TRY);
                return;
            }
        }
    }
    serverMuter.unlock();
    for(const auto &letter: request[0]){
        if((letter>'9'|| letter<'0')&&(letter>'Z'||letter<'A')&&(letter>'z'||letter<'a')&&letter!='_'){
            sender(WRONG_LOGIN);
            sender(LOGIN_TRY);
            return;
        }
    }
    serverMuter.lock();
    nickname=request[0];
    master->getRooms()->at(master->getLobby()).push_back(this);
    serverMuter.unlock();
    flag=LOBBY;
    request.clear();
    sender(SIGN_IN_OK);
}

bool Session::findRoomByName(const std::string &name) {
    serverMuter.lock();
    for(const auto &room: *(master->getRooms())){
        if(room.first->nameGetter()==name){
            currentRoom=room.first;
            serverMuter.unlock();
            return true;
        }
    }
    serverMuter.unlock();
    sender(JOIN_ERROR);
    return false;
}

void Session::sendRooms() {
    std::string roomsList(BREAK_LINE);
    serverMuter.lock();
    for(const auto &room: *(master->getRooms())){
        roomsList+=room.first->nameGetter()+'\n';
    }
    serverMuter.unlock();
    roomsList+='\n'+BREAK_LINE;
    sender(roomsList);
}

bool Session::checkDeleteAbility() {
    serverMuter.lock();
    roomMuter.lock();
    if(master->getRooms()->at(currentRoom).at(0)==this) {
        roomMuter.unlock();
        serverMuter.unlock();
        return true;
    }
    roomMuter.unlock();
    serverMuter.unlock();
    sender(DELETE_ERROR);
    return false;
}

Session::~Session() {
    if(currentRoom!=nullptr) delete currentRoom;
}
