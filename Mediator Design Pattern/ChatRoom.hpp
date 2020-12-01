#pragma once
#include "headers.hpp"
#include "Person.hpp"

struct ChatRoom {
    
    std::vector<Person*> people;

    void broadcast(const std::string& origin, const std::string& message);

    void join(Person* p);

    void message(const std::string& origin, const std::string& who, const std::string& message);


};