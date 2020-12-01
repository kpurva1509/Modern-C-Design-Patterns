#pragma once

#include <string>
#include <vector>

#include "Observer.hpp"

template <typename>
struct Observer;

template <typename T>
struct Observable
{

public:
    // vector is not a thread_safe class
    // STL in general is not thread_safe
    std::vector<Observer<T> *> observers;

public:
    void notify(T &source, const std::string &field_name)
    {
        // iterating through a collection
        for (auto observer : observers)
        {
            observer->field_changed(source, field_name);
        }
    }

    void subscribe(Observer<T> &observer)
    {
        observers.push_back(&observer);
    }

    void unsubscribe(Observer<T> &observer)
    {
        // using the classic erase-remove idiom
        // modifying the collection
        observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end());
    }
};