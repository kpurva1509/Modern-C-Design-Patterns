#include "ChatRoom.hpp"

void ChatRoom::broadcast(const std::string &origin, const std::string &message){

    for(auto& p : people){
        if(p->name != origin){
            p->receive(origin, message);
        }
    }
}

void ChatRoom::join(Person *p) {

    std::string join_msg = p->name + "joins the chat";
    broadcast("room", join_msg);
    p->room = this;
    people.push_back(p);

}

void ChatRoom::message(const std::string &origin, const std::string &who, const std::string &message) {

    auto target = std::find_if(people.begin(), people.end(), 
    [&](const Person *p){
        return p->name == who;
    });

    if(target != people.end()){
        (*target)->receive(origin, message);
    }


}