#include "headers.hpp"
#include "Person.hpp"
#include "ChatRoom.hpp"

// Mediator -
/* A component that facilates the communication 
   between other components without them necessarily
   having to store a direct reference or being aware
   of each others' existence in the environment
*/

struct EventData{
    virtual void print() const = 0;
};

struct Player;

struct PlayerScored : EventData {
    std::string player_name;
    int goals_scored_so_far;

    PlayerScored(const std::string& player_name, const int& goals_scored_so_far):player_name(player_name), goals_scored_so_far(goals_scored_so_far) {}

    void print() const override {
        cout << player_name << " has scored their " << goals_scored_so_far << " goal" << endl;
    }
};

struct Game{
    signal<void(EventData*)> events;
};

struct Player {
    std::string name;
    int goals_scored{0};
    Game& game;

    Player(const std::string& name, Game& game): name(name), game(game){}

    void score() {
        goals_scored++;
        PlayerScored ps{name, goals_scored};
        game.events(&ps);
    }
};

struct Coach {
    Game& game;
    Coach(Game& game):game(game){
        game.events.connect([](EventData* e){
            PlayerScored* ps = dynamic_cast<PlayerScored*>(e);
            if(ps && ps->goals_scored_so_far < 3){
                cout << "Coach says well done " << ps->player_name << "!\n";
            }
        });
    }
};

int main()
{

    /*
    ChatRoom room;
    Person john{"John"};
    Person jane{"Jane"};
    room.join(&john);
    room.join(&jane);
    john.say("HI Room");
    jane.say("Oh, Hi John");

    Person simon{"Simon"};
    room.join(&simon);
    */

   // Event Broker






}