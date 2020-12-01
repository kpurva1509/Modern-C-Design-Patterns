#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <memory>
#include <numeric>
#include <algorithm>
#include <functional>
#include <typeinfo>

using std::cout;
using std::endl;

// https://stackoverflow.com/questions/15881524/how-do-i-set-up-code-coverage-in-c-googletest-project-in-visual-studio-2010

// Chain of responsibility

// 1. Pointer chain 
// Similar to a singly linked list 

struct Creature {
  std::string name;
  int attack, defense;

  // constructor
  Creature(const std::string& name, int attack, int defense):
        name(name), attack(attack), defense(defense){}

  // stream output operator overloading
  friend std::ostream& operator<<(std::ostream& out, const Creature& obj);
};

std::ostream& operator<<(std::ostream& out, const Creature& obj){
  out << "Name of the creature is " << obj.name << "\nAttack : " << obj.attack << "\nDefense : "
   << obj.defense;
  return out;
}

class CreatureModifier {

  CreatureModifier* next{nullptr};
  
protected:
  Creature& creature;

public:

  CreatureModifier(Creature& creature):creature(creature){}

  void add(CreatureModifier* cm){
    if(next) next->add(cm);
    else next = cm;
  }

  virtual void handle() {
    if(next) next->handle();
  }

};

class DoubleAttack : public CreatureModifier {

public:

  // delegating constructors
  DoubleAttack(Creature& creature):CreatureModifier(creature){}

  void handle() override {

    creature.attack *= 2;

    // walk the chain of responsibility
    // checks the next pointer and traverses the chain of responsibility
    CreatureModifier::handle();
  }
};

class IncreasedDefense : public CreatureModifier {

public:
  IncreasedDefense(Creature& creature):CreatureModifier(creature) {}

  void handle() override {
    if(creature.attack <= 2) creature.defense++;
    CreatureModifier::handle();
  }

};

class NoBunuses : public CreatureModifier {

public:
  NoBunuses(Creature& creature):CreatureModifier(creature){}

  void handle() override {}

};

int main () {

  Creature goblin{"Goblin", 1,1};
  CreatureModifier root{goblin};
  DoubleAttack r1{goblin};
  DoubleAttack r2{goblin};
  IncreasedDefense r3{goblin};

  NoBunuses curse{goblin};
  root.add(&curse);

  root.add(&r1);
  root.add(&r2); // 4
  root.add(&r3); // no-op

  root.handle();

  cout << goblin << endl;

  return 0;
}
