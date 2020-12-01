#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <memory>
#include <tuple>
#include <cstdint>
#include <cmath>

// #include <boost/signal.hpp>
// s#include <boost/signals2.hpp>

#include "Observer.hpp"
#include "Observable.hpp"
#include "Safer_Observable.hpp"

using std::cout;
using std::endl;

// using namespace boost;

// Observer Design Pattern
// Observable == Subject

// The observer design pattern avoids the tight coupling
// between the subject and the observers
// The observers are dynamically bound to the subject at runtime
// This is acheived by having an abstract Observer base class
// The actual observer classes inherit from this abstract base
// We maintain a member pointer to the observer class,
// which dereferences to the NON-ABSTRACT representation
// of the observer
// The observers can also dynamically subscribe to the subject using
// a method where they pass their "this" object
// The subject maintains a vectors of these objects passed
// Then it notifies all the elements in this vector of observers

// Similar to the condition variable with wait() and notify() signals
// Idea of event and subscriber
// Idea of signal and slot(Boost, QT, etc)

// CRTP
struct Person : Safer_Observable<Person> {

    int age;

    Person(int age) : age(age) {}

    int get_age() const
    {
        return age;
    }

    void set_age(int age)
    {

        auto old_can_vote = get_can_vote();

        // Person::age = age;
        if (this->age == age)
            return;
        this->age = age;
        notify(*this, "age");

        if(old_can_vote != get_can_vote()){
            notify(*this, "can_vote");
        }

    }

    bool get_can_vote() const {
        return age >= 16;
    }

};

// abstract base class observer
// it is an observer of the subject
struct ConsolePersonObserver : Observer<Person>
{ // Observer<Observable>
public:
    void field_changed(Person &source, const std::string &field_name) override
    {
        cout << "Person's " << field_name << " has changed to ";
        if (field_name == "age")
            cout << source.get_age();
        cout << "\n";

        if(field_name == "can_vote")
        cout << boolalpha << source.get_can_vote() << endl;
    }
};

// boost signal based observer design pattern
// using the terminology of signals and slots
// Signal == Subject
// Slot == Subcriber

/*template<typename T>
struct Observable2{
    boost::signal<void(T&, const std::string&)> field_changed;
};

class Person2: public Observable2<Person2> {
    int age;
public:
    int get_age() const {
        return age;
    }

    void set_age(int age) {
        if(this->age == age) return;
        this->age = age;
        field_changed(*this, "age");
    }
}; */

// deadlock condition
struct Traffic_Administration : Observer<Person> {

    void field_changed(Person &source, const std::string &field_name) override
    {
        if(field_name == "age"){
            if(source.get_age() < 17)
                cout << "Not old enough" << endl;
        } else {
            cout << "old enough" << endl;
            source.unsubscribe(*this);
        }
    }
};

int main()
{

    // Person 1
    Person person{10};
    ConsolePersonObserver cpo;
    person.subscribe(cpo);

    person.set_age(11);
    person.set_age(12);
    person.set_age(13);

    person.unsubscribe(cpo);

    person.set_age(15);

    // Person 2
    // Person2 person2;

    // create a connection to the subject(observable)
    // subcribe to the signal with a lambda function
    /*auto conn = person2.field_changed.connect(
        [](Person2& p, const std::string& field_name){
            cout << field_name << " has changed\n";
        }
    );

    person2.set_age(25);
    conn.disconnect(); */

    Person p2{12};
    ConsolePersonObserver cpo2;
    p2.subscribe(cpo2);

    p2.set_age(15);
    // p2.set_age(16);

    cout << endl;

    // deadlock condition
    Traffic_Administration ta;
    p2.subscribe(ta);

    p2.set_age(16);
    try{
    p2.set_age(17);
    } catch(const std::exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }

    return 0;
}