#pragma once
#include "headers.hpp"
#include "ChatRoom.hpp"

struct ChatRoom;

struct Person {

    // This will have a reference to the chatroom(mediator)
    // The mediator will be instatiated in the constructor call of each 
    // participant of the chat room
    // The MEDIATOR can also be instantiated using a Singleton
    // The MEDIATOR is usually a global static variable, or is passed
    // in as a variable in the constructor

    std::string name;
    std::vector<std::string> chat_log;
    ChatRoom* room{nullptr};

    Person(const std::string& name);

    void say(const std::string& message) const;
    
    void pm(const std::string& who, const std::string& message) const;

    void receive(const std::string& origin, const std::string& message);

    bool operator==(const Person& rhs) const;

    bool operator!=(const Person& rhs) const;

private: 
     
    // making copy assignment and copy constructor private
    // does not make sense to have copy in Person
    // One person in the chat room cannot be a copy of other 

    Person(const Person& rhs);
    Person& operator=(const Person& rhs);


};