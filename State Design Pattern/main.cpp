#include <iostream>
#include<memory>
#include<string>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<functional>
#include<numeric>
#include<tuple>
#include<initializer_list>

using std::cout;
using std::endl;

class LightState;

class State{
  public:
  
  virtual void on(LightState* ls){
    cout << "Already ON" << endl;
  }

  virtual void off(LightState* ls){
    cout << "Already OFF" << endl;
  }
};

class OnState : public State {
  
  public:
  
  OnState() {
    cout << "Light is turned on" << endl;
  }

  // providing an interface to switch to another state 
  virtual void off(LightState* ls);
};

class OffState : public State {

  public:
  
  OffState() {
    cout << "Light is switched off" << endl;
  }

  // providing an interface to switch to another state 
  virtual void on(LightState* ls);
};

// Machine or the CONTEXT, whose states are to be changed 
class LightState {

  public:

  State* currentState;
  LightState(){
    currentState = new OffState();
    cout << endl;
  }

  void setState(State* state) {
    this->currentState = state;
  }

  // switching the states as per the inputs
  // we cannot just change states
  // we have to switch from different classes of the derived states
  void on() {
    // currentState holds a pointer to one of the derived states
    // we need to change the status of the current object to 
    // reflect the change 
    currentState->on(this);
  }

  void off() {
    currentState->off(this);
  }

};

void OnState::off(LightState* ls) {

  cout << "Switching from ON to OFF" << endl;
  ls->setState(new OffState());

  // delete the current state 
  // we delete the current OnState as we are switching to OffState
  delete this;

}
  
void OffState::on(LightState* ls) {
  cout << "Switching from OFF to ON" << endl;
  ls->setState(new OnState());

  // delete the current state 
  // we delete the current OnState as we are switching to OffState
  delete this;
}

int main() {

  LightState ls;
  ls.on();
  ls.off();
  ls.off();

}
