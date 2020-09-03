//
// Created by Daniil Tchyorny on 03.09.2020.
//

#include "room.h"
#include "constexp.h"


bool Room::handler(const std::vector<std::string> &command, Session *member, Room *r) {
    operation c = checkCommand(command[0]);
    if (c == NOT_COMMAND) write(command, member);
    else if (c == CREATE) createMe(command, member);
    else if (c == JOIN) addMember(member);
    else if (c == LEAVE) leaveRoom(member);
    else if (c == MEMBER) sendMembers(member);
    else if (c == DELETE) deleteMe();
    return false;
}

enum operation Room::checkCommand(const std::string &message) {
    if (message == "#create") return CREATE;
    else if (message == "#join") return JOIN;
    else if (message == "#leave") return LEAVE;
    else if (message == "#delete") return DELETE;
    else if (message == "#member") return MEMBER;
    else return NOT_COMMAND;
}

void Room::write(const std::vector<std::string> &message, Session *user) {
    historySizeCheck();
    std::string finalMessage(user->nicknameGetter() + ": " + wordsToSentence(message) + '\n');
    serverMuter.lock();
    std::unordered_map<Room *, std::vector<Session *>> *rooms=master->getRooms();
    for(const auto &member: rooms->at(this)){
        member->sender(finalMessage+'\n');
    }
    serverMuter.unlock();
    storyMuter.lock();
    history.push_front(finalMessage);
    storyMuter.unlock();
}

void Room::createMe(const std::vector<std::string> &name, Session *creator) {
    std::vector<Session *> members;
    members.push_back(creator);
    serverMuter.lock();
    master->getRooms()->insert(std::pair<Room *, std::vector<Session *>>(this,members));
    roomName=name[1];
    serverMuter.lock();
    write("The room '"+roomName+"' has been created!\n");
}

void Room::addMember(Session *member) {
    serverMuter.lock();
    try{
        master->getRooms()->at(this).push_back(member);
    }catch (std::exception &ex){
        std::cerr<<ex.what()<<std::endl;
        serverMuter.unlock();
        return;
    }
    serverMuter.unlock();
    write("The member '"+member->nicknameGetter()+"' has been joined!\n");
    sendHistory(member);
}

void Room::deleteMe() {
    serverMuter.lock();
    for(const auto &member: master->getRooms()->at(this)){
        member->sender(DELETE_ROOM);
        member->resetRoom();
        member->setState(LOBBY);
    }
    master->getRooms()->erase(this);
    serverMuter.unlock();
}

void Room::leaveRoom(Session *member) {
    uint16_t index=0;
    serverMuter.lock();
    for(const auto user:master->getRooms()->at(this)){
        if(user==member)break;
        index++;
    }
    master->getRooms()->at(this).erase(master->getRooms()->at(this).begin()+index);
    serverMuter.unlock();
    write("The member '"+member->nicknameGetter()+"' jeft(\n");
    member->setState(LOBBY);
    member->resetRoom();
}

void Room::historySizeCheck() {
    storyMuter.lock();
    if(history.size()>HISTORY_SIZE) history.pop_back();
    storyMuter.unlock();
}

std::string Room::wordsToSentence(const std::vector<std::string> &vec) {
    std::string s;
    for(const auto &el:vec)
        s+=el+" ";
    return s;
}

void Room::write(const std::string &message) {
    serverMuter.lock();
    std::unordered_map<Room *, std::vector<Session *>> *rooms=master->getRooms();
    for(const auto &member: rooms->at(this)){
        member->sender(message);
    }
    serverMuter.unlock();
    storyMuter.lock();
    history.push_front(message);
    storyMuter.unlock();
}

void Room::sendHistory(Session *newUser) {
    storyMuter.lock();
    auto it=history.end();
    for(;it!=history.begin();it--){
        newUser->sender(*it);
    }
    storyMuter.unlock();
}

void Room::sendMembers(Session *asker) {
    std::string membersList(BREAK_LINE);
    serverMuter.lock();
    for(const auto &member: master->getRooms()->at(this)){
        membersList+=member->nicknameGetter()+'\n';
    }
    serverMuter.unlock();
    membersList+=BREAK_LINE;
    asker->sender(membersList);
}
