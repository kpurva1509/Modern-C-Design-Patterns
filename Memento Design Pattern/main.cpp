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

using std::cout;
using std::endl;

// Memento Design Pattern
// Roll back mechanism 
// Undo operation 

class Memento{

private:
  int balance;

public:
  Memento() {} 
  Memento(const int& balance):balance(balance){}

  int get_balance() const {
    return balance;
  }

  // violates OCP
  friend class BankAccount;
  friend class BankAccount2;

}; 

class BankAccount {

public:
  int balance{0};

public:

  BankAccount(int balance):balance(balance){}

  Memento deposit(int amount){
    balance +=amount;
    return {balance};
  }

  void restore(const Memento& memento){
    balance = memento.get_balance();
  }

  friend std::ostream& operator<<(std::ostream& out, const BankAccount& obj);

};

std::ostream& operator<<(std::ostream& out, const BankAccount& obj) {
  out << "Current balance in the account is " << obj.balance;
  return out;
}

class BankAccount2 {

private:

  int balance{0};
  // vector of changes applied to a BankAccount2
  // The state of the BankAccount2 is recorded at each stage
  // we have a shared_ptr since we are passing around the memento across methods
  std::vector<std::shared_ptr<Memento>> changes;
  int current; // index into the current memento (timeline)

public:

  // construct the bank account with an initial balance 
  BankAccount2(const int& balance):balance(balance){
    // save the initial state into the Memento vector 
    changes.emplace_back(std::make_shared<Memento>(balance));
    // current will be 0
    current = 0;
  }

  // deposit will also make an entry into the memento vector 
  std::shared_ptr<Memento> deposit(int amount){
    balance += amount;
    auto m = std::make_shared<Memento>(balance);
    current++;
    changes.push_back(m);
    return m;
  }

  void restore(const std::shared_ptr<Memento>& memento) {

    if(memento){
      balance = memento->get_balance();
      changes.push_back(memento);
      current = changes.size() - 1;
    }
  }

  std::shared_ptr<Memento> undo() {
    if(current > 0){
      --current;
      auto m = changes[current];
      balance = m->get_balance();
      return m;
    } else {
      return {};
    }
  }

  std::shared_ptr<Memento> redo() {
    if(current +1 < changes.size()){
      ++current;
      auto m = changes[current];
      balance = m->get_balance();
      return m;
    } else {
      return {};
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const BankAccount2& obj);

};

std::ostream& operator<<(std::ostream& out, const BankAccount2& obj) {
 out << "Current balance in the account is " << obj.balance;
  return out; 
}

int main() {

  cout << "Bank Account # 1 operations..." << endl;

  BankAccount ba1{1000};
  // m1 has a balance of 1200
  auto m1 = ba1.deposit(200);
  // m2 has a balance of 1500
  auto m2 = ba1.deposit(300);
  cout << ba1 << endl;

  // if we restore m1, it implies we go back to the state represented by m1 = 1200
  ba1.restore(m1);
  cout << ba1 << endl;
  
  // if we restore m2, it implies we go back to the state represented by m2 = 1500
  ba1.restore(m2);
  cout << ba1 << endl;

  // BankAccount2

  cout << "\nBank Account # 2 operations..." << endl;

  BankAccount2 ba2{2000};
  ba2.deposit(100);
  ba2.deposit(400);
  ba2.undo();

  cout << ba2 << endl;

  ba2.redo();
  cout << ba2 << endl;



  return 0;
}
