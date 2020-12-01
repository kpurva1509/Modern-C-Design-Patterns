#pragma once
#include <string>
using namespace std;

// T is the type of object we are observing(observable, or the subject)

template <typename T>
struct Observer
{

    virtual void field_changed(T &source, const std::string &field_name) = 0;
};