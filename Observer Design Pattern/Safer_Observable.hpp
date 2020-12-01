#pragma once

#include <string>
#include <vector>
#include <mutex>

#include "Observer.hpp"

template <typename>
struct Observer;

// thread safety and reentrancy

template<typename T>
class Safer_Observable {

public:
    // vector is not a thread_safe class
    // STL in general is not thread_safe
    std::vector<Observer<T> *> observers;
    std::mutex mutex_t;
    // mtx;

public:
    void notify(T &source, const std::string &field_name)
    {
        std::scoped_lock<std::mutex> lock{mutex_t};
        // iterating through a collection
        for (auto observer : observers)
        {
            observer->field_changed(source, field_name);
        }
    }

    void subscribe(Observer<T> &observer)
    {
        std::scoped_lock<std::mutex> lock{mutex_t};
        observers.push_back(&observer);
    }

    void unsubscribe(Observer<T> &observer)
    {
        std::scoped_lock<std::mutex> lock{mutex_t};
        // using the classic erase-remove idiom
        // modifying the collection
        observers.erase(
            remove(observers.begin(), observers.end(), &observer),
            observers.end());
    }
};
