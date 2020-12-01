#include "headers.hpp"
#include <boost/flyweight.hpp>
#include <boost/bimap.hpp>

using namespace boost;

// Flyweight design pattern
// Space optimization
// Avoid redundancy when storing data
// list of names, and pointers/references/indices to the common names
// Storing common data externally and then refer to them as needed

// e.g. MMORG

typedef uint32_t key;

struct User
{
    
    User(const std::string& first_name, const std::string& last_name):
           first_name{add(first_name)}, last_name{add(last_name)} {}

    const std::string& get_first_name(void) const {
        return names.left.find(first_name)->second;
    }

    const std::string& get_last_name(void) const {
        return names.right.find(last_name)->first;
    }

    friend std::ostream& operator<<(std::ostream& out, const User& user){
        out << "first_name: " << user.get_first_name() << " last_name: " 
                            << user.get_last_name() << endl;
        return out;
    }

    protected:
    key first_name, last_name; // uint32_t
    static bimap<key, std::string> names;
    static key seed;

    static key add(const std::string& s){
        auto it = names.right.find(s); 
        if(it != names.right.end()){
            return it->second;
        }
        else
        {
            key id = ++seed;
            names.insert({seed,s});
            return id;
        }
    }
};

// building a raw caching mechanism for the names
key User::seed{0};
bimap<key, std::string> User::names{};

// boost flyweight
struct User2{
    flyweight<std::string> first_name, last_name;

    // constructor
    User2(const std::string& first_name, const std::string& last_name):
                first_name(first_name),
                last_name(last_name){}


};



int main() {

    User u1{"Purva", "Kulkarni"};
    User u2{"Manali", "Kulkarni"};

    User2 u3{"Shreehari", "Kulkarni"};
    User2 u4{"Madhavi", "Kulkarni"};
    return 0;

}