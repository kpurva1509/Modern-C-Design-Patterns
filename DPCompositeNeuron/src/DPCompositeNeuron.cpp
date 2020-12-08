/}
/============================================================================
// Name        : DPCompositeNeuron.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>
#include <cstdio>

using std::cout;
using std::endl;

// Object of a type
// Object of a collection of the type
// Scalar and composition of objects in a uniform manner
// Designing Composite Neural Networks

// CRTP - Curiously Recurring Template Pattern
template<typename Self>
struct SomeNeurons{

  template <typename T>
  void connectTo(T& other){

    // connect to a vector of pointers to a vector of pointer
    // Self will point to the class of the calling object 
    // other will be the class of the object passed as a parameter
    // we want to connect each neuron of the calling object vector to
    // each neuron of the object passed as a parameter
    for(Neuron& from : *static_cast<Self*>(this)){
      for(Neuron& to : other){
        from.outgoing.push_back(&to);
        to.incoming.push_back(&from);
      }
    }

};


struct Neuron : SomeNeurons<Neuron> {

  unsigned int id;
  std::vector<Neuron*> incoming, outgoing;

  Neuron(){
    static int id = 1;
    this->id = id++;
  }

  /*void connectTo(Neuron& other){

    // the new neuron to your outgoing connections
    // push reference to other object in your "outgoing" 
    // add yourself to the neurons's incoming connections

    outgoing.push_back(&other);
    other.incoming.push_back(this);

  }*/

  // Overloading the iterator for range based for loop
  Neuron* begin(){
    return this;
  }

  Neuron* end(){
    return this+1;
  }

  friend std::ostream& operator<<(std::ostream& out, const Neuron& neuron);

};

std::ostream& operator<<(std::ostream& out, const Neuron& neuron){
  
  // print the incoming and the outgoing nodes
  // sinc eincoming and outgoing are a vector of pointers
  // we need to take in each element into a pointer as below 

  out << "Neuron id  : " << neuron.id << endl;

  for(Neuron* n : neuron.incoming){
    out << n->id << "\t-->\t[" << neuron.id << "]" << endl;
  }

  for(Neuron* n : neuron.outgoing){
    out << "[" << neuron.id << "]\t-->\t" << n->id << endl;
  }

  return out;
}

struct NeuronLayer : std::vector<Neuron>, SomeNeurons<NeuronLayer> {

  NeuronLayer(int count){
    while(count-- > 0){
      emplace_back(Neuron{});
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const NeuronLayer& neuronLayer);

};

std::ostream& operator<<(std::ostream& out, const NeuronLayer& neuronLayer){

  for(auto& n : neuronLayer) out << n;

  return out;
}

int main() {

  Neuron n1, n2;
  //n1.connectTo(n2);
  cout << n1 << n2 << endl;

  NeuronLayer nLayer(3);
  cout << nLayer << endl;

}
