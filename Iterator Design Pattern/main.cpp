#include<iostream>
#include<vector>
#include<list>
#include<functional>
#include<algorithm>
#include<numeric>
#include<queue>
#include<map>
#include<memory.h>
#include <bits/stdc++.h>
#include<string>
#include<sstream>
#include<fstream>
#include<limits.h>

#include <experimental/coroutine>
#include <experimental/generator>

using std::cout;
using std::endl;

// Iterator design pattern
// An object which facilitates the traversal of a data structure

template<typename T> struct BinaryTree;

template<typename T>
struct Node {
  T value = T();
  Node<T>* left{nullptr};
  Node<T>* right{nullptr};
  Node<T>* parent{nullptr};

  BinaryTree<T>* tree{nullptr};

  Node(T value):value(value){}

  Node(T Value, Node<T>* left, Node<T>* right):value(value), left(left), right(right){
    this->left->tree = this->right->tree = tree;
    this->left->parent = this->right->parent = this;
  }

  void set_tree(BinaryTree<T>* t) {
    tree = t;
    if(left) left->set_tree(t);
    if(right) right->set_tree(t);
  }

  ~Node(){
    if(left) delete left;
    if(right) delete right;
  }

};

// Binary tree as a wrapper around the root element 
template<typename T>
struct BinaryTree {

  Node<T>* root{nullptr};

  BinaryTree(Node<T>* root):root(root), preorder(*this) {
    root->set_tree(this);
  }

  ~BinaryTree(){
    if(root) delete root;
  }

  template<typename U>
  struct preorderiterator{

    // node we are currently iterating over
    Node<U>* current;

    preorderiterator(Node<T>* current):current(current){}

    // comparing to other iterator
    bool operator!=(const preorderiterator<U>& other){
      return current != other.current;
    }

    // no recursion
    preorderiterator<U>& operator++(){

      // in-order successor 
      if(current->right){
        current = current->right;
        while(current->left){
          current = current->left;
        }
      } else {
        Node<U>* p = current->parent;
        while(p && current == p->right){
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    Node<U>& operator*(){
      return *current;
    }

  };

  typedef preorderiterator<T> iterator;
  iterator begin() {
    Node<T>* n = root;
    if(n){
      while(n->left){
        n=n->left;
      }
    }

    return iterator{n};
  }

  iterator end(){
    return iterator{nullptr};
  }

  class PreOrderTraversal {
    BinaryTree& tree;
  public:
    PreOrderTraversal(BinaryTree& tree):tree(tree){}
    iterator begin(){ return tree.begin();}
    iterator end() { return tree.end();}
  } preorder;

  // function called postorder
  // returns a generator, which is iterable 
  // it is recursive iterator

  std::experimental::generator<Node<T>*> postorder(){

    return postorderimpl(root);
  }

  private:
    std::experimental::generator<Node<T>*> postorderimpl(Node<T>* root){

      if(root){
        for(auto left : postorderimpl(root->left))
          co_yield left;
        for(auto right : postorderimpl(root->right))
          co_yield right;
        co_yield root;
      }
    }
};


int main() {

  std::vector<std::string> names{"Manali", "Purva", "Shreehari", "Madhavi"};
  std::vector<std::string>::iterator it = names.begin();

  cout << *it << endl;

  ++it;
  it->append(std::string(" Kulkarni"));
  cout << *it << endl;

  // pre-order iterator

  return 0;
}
