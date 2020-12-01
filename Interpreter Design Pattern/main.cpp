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
#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/spirit.hpp>

using namespace boost;

// https://stackoverflow.com/questions/15881524/how-do-i-set-up-code-coverage-in-c-googletest-project-in-visual-studio-2010
using std::cout;
using std::endl;

// Lexing
// Parsing
// Evaluating

struct Token{
  enum Type {integer, plus, minus, left, right} type;
  std::string text;

  Token(Type type, const std::string& text): type(type), text(text){}

  friend std::ostream& operator<<(std::ostream& out, const Token& token);
};

std::ostream& operator<<(std::ostream& out, const Token& token){
  out << " " << token.text << " ";
  return out;
}

// Lexing 

std::vector<Token> lex(const std::string& input) {
  std::vector<Token> result;

  for(int i=0; i < input.size();i++){
    switch(input[i]){
      case '+':
      result.push_back(Token{Token::plus, "+"});
      break;
      case '-':
      result.push_back(Token{Token::minus, "-"});
      break;
      case '(':
      result.push_back(Token{Token::left, "("});
      break;
      case ')':
      result.push_back(Token{Token::right, ")"});
      break;
      default:
      std::ostringstream buffer;
      buffer << input[i];
      for(int j = i+1; j < input.size();j++){
        if(isdigit(input[j])){
          buffer << input[j];
          i++;
        } else {
          result.push_back(Token{Token::integer, buffer.str()});
          break;
        }
      }
    }
  }

  return result;
}

// Parsing 
struct Element {

  virtual int eval() const = 0;

};

struct Integer : Element {

  int value;
  Integer(int value):value(value){}

  virtual int eval() const {
    return value;
  }

};

struct BinaryOperation : Element {

  enum Type {addition, subtraction} type;
  std::shared_ptr<Element> lhs, rhs;

  int eval () const override {
    auto right = rhs->eval();
    auto left = lhs->eval();
    if(type == addition){
      return left+right;
    } else return left - right;
  }

};

std::shared_ptr<Element> parse(const std::vector<Token>& tokens){
  auto result = std::make_unique<BinaryOperation>();
  bool have_lhs{false};
  for(int i = 0; i < tokens.size(); i++){
    auto& token = tokens[i];
    switch(token.type){
      case Token::integer:
        int value = boost::lexical_cast<int>(token.text);
        auto integer = std::make_shared<Integer>(value);
        if(!have_lhs){
            result->lhs = integer;
        } else result->rhs = integer;
        break;
      case Token::plus:
        result->type = BinaryOperation::addition;
        break;
      case Token::minus:
          result->type = BinaryOperation::subtraction;
          break;
      case Token::left:
        {
            int j = i;
            for(; j < tokens.size(); j++){
                if(tokens[j].type == Token::right)
                break;
            }
            // copying a subset of the original vector
            std::vector<Token> sub_expression(&tokens[i+1], &tokens[j]);

            // recursively call the parse function on this subset
            // record the evaluated value into the element variable
            auto element = parse(sub_expression);

            // the result will be an integer, set the appropriate value
            if(!have_lhs){
                result->lhs = element;
                have_lhs = true;
            } else {
                result->rhs = element;
            }

            // set value of i to j, since the subexpression between the indices i to j has been evaluated already
            i = j;
        }
        break;
    }
  }
}

int main () {

  std::string input{"(13-4)-(12+1)"};

  auto tokens = lex(input);

  for(auto& token : tokens)
    cout << token << " ";

  cout << endl; 


  return 0;
}