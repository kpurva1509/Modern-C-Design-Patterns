#include "headers.hpp"

using std::cout;
using std::endl;

/* Singleton Design Pattern
For some components of the system, it really makes sense to only have one instance
eg - Database Repository
eg - object Factory
Situations where the constructor call is expensive
Provide all the consumers with an API to the one and only object created
Also, need to maintain thread-safety of access
*/

class SingletonDatabase{

private:
    SingletonDatabase(){
        cout << "Initializing the database" << endl;
        std::ifstream ifs("Capitals.txt");
        std::string s, s2;

       while(getline(ifs, s)){
           getline(ifs, s2);
           capitals.insert(std::make_pair(s, std::stoi(s2)));
       }

    }
    std::map<std::string, int> capitals;
    
public:

    // deleting the copy constructor and copy assignment to avoid replication
    SingletonDatabase(const SingletonDatabase& rhs) = delete;
    SingletonDatabase& operator=(const SingletonDatabase& rhs) = delete;

    // return a static instance of the Singleton
    // the method is static because the constructor of the class in private
    // as the constructor is private, we cannot instantiate the class object
    // therefore, we need to access the method WITHOUT an object
    // the body of the methos has static member, since the static object
    // implies that there is onlt a one-time instatiation of the class object
    // we return a reference of the static object because returning a copy
    // will error out since copy constructor and assignment are deleted, 
    // also, copy will defeat the purpose of SINGLETON
    static SingletonDatabase& get(){
        static SingletonDatabase db;
        return db;
    }

    int get_population(const std::string& city){
        if(capitals.find(city)!=capitals.end()){
            return capitals[city];
        } else
        {
            cout << "Entry does not exist in the database" << endl;
            return 0;
        }
        
    }

};

struct SingletonRecordFinder{

    int total_population(const std::vector<std::string>& names){
        int result{0};
        for(auto& name: names){
            result += SingletonDatabase::get().get_population(name);
        }

        return result;
    }

};

TEST(RecordFinderTest, SingletonRecordPopulationTest){
    SingletonRecordFinder srf;
    std::vector<std::string> names{"Mumbai", "Delhi"};
    int tp = srf.total_population(names);
    EXPECT_EQ(1200, tp);

}

int main(int ac, char* av[]) {

    // int population = SingletonDatabase::get().get_population("Mumbai");
    if (int population = SingletonDatabase::get().get_population("Hyderabad"))
    {
        cout << "Population : " << population << endl;
    }

    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();

    return 0;
}