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

// forward declaration of structures
struct HtmlELement;
struct HtmlBuilder;

// Builder design pattern
class HtmlELement{

  std::string name, text;
  std::vector<HtmlELement> elements;
  const size_t indent_size = 2;

public: 

  friend struct HtmlBuilder;
  // constructors
  HtmlELement(){}

  HtmlELement(const std::string& name, const std::string& text)
    : name(name), text(text){}

  // printing of the HtmlELement
  std::string str(int indent = 0) const{
    
    std::ostringstream oss;
    std::string i(indent_size*indent, ' ');
    oss << i << "<" << name << ">" << endl;
    if(text.size() > 0){
      oss << std::string(indent_size*(indent+1), ' ') << text << endl;
    }

    // recursive invocation for each element in the vector 
    for(const auto& e : elements)
      oss << e.str(indent+1);

    oss << i << "</" << name << ">" << endl;
    return oss.str();
  }

  /*static HtmlBuilder build(std::string root_name){
    return {root_name};
  }*/

};

// building the data structure of the HTML page piecewise 
struct HtmlBuilder{

  HtmlELement root;

  // constructor
  HtmlBuilder(std::string root_name){
    root.name = root_name;
  }

  // fluent interface - for method chaining
  HtmlBuilder& addChild(std::string name, std::string text){
    //HtmlELement element{name, text};
    root.elements.emplace_back(name, text);
    return *this;
  }

  std::string str() const{
    return root.str();
  }

  operator HtmlELement() const{
    return root;
  }

};

// Groovy-style builder
// Uniform initialization syntax 

struct Tag{
  std::string name;
  std::string text;
  std::vector<Tag> children;

  // key-value pair of attributes of the Tag 
  std::vector<std::pair<std::string, std::string>> attributes;

  // stream output operator
  friend std::ostream& operator<<(const std::ostream& out, const Tag& obj);

  protected:

  // constructor
  Tag(const std::string& name, const std::string& text):
          name(name), text(text){}
  
  Tag(const std::string& name, const std::vector<Tag>& children):
          name(name),children(children){}

};

std::ostream& operator<<(std::ostream& out, const Tag& obj){

  out << "<" << obj.name;
  for(const auto& att: obj.attributes)
    out << " " << att.first << "=\"" << att.second << "\"";

  if(obj.children.size() == 0 && obj.text.length()==0)
    out << "/>" << endl;
  else{
    out << ">" << endl;
    if(obj.text.length()){
      out << obj.text << endl;
    }

    // child will also be a Tag element, this will basically recurse
    for(const auto& child : obj.children)
      out << child;
    out << "</" << obj.name << ">" << endl;   
  }
  return out;
}

struct Paragraph : Tag{

Paragraph(const std::string& text):Tag("p", text){}

Paragraph(std::initializer_list<Tag> children)
        :Tag("p", children){}

};

struct Image : Tag{

  Image(const std::string& url):Tag("img",""){
    attributes.emplace_back(std::make_pair("src", url));
  }
};

// Builder Facets

// forward declaration of classes

class PersonBuilder;
class PersonAddressBuilder;
class PersonJobBuilder;

class Person{

  // address information
  std::string street_address, post_code, city;

  // employment information
  std::string company_name, position;
  int income{0};

  friend class PersonBuilder;
  friend class PersonJobBuilder;
  friend class PersonAddressBuilder;

  /*static PersonBuilder create(){
    
    return PersonBuilder();
  }*/

};

int main() {

  // adding HTML elements to the script
  auto text = "hello";
  std::string output;

  // adding a paragraph tag
  output += "<p>";
  output += text;
  output += "</p>";

  std::string words[] = {"hello", "world"};
  std::ostringstream oss;
  oss << "<ul>";
  for(auto& w : words)
    oss << "\n<li>" << w << "</li>";
  oss << "\n</ul>";

  cout << oss.str() << endl;

  cout << endl;

  HtmlBuilder builder{"ul"};
  builder.addChild("li", "hello").addChild("li", "world");

  cout << builder.str() << endl;

  // auto builder2 = HtmlELement::build("ul").addChild("li", "world");

  cout << "Groovy-Style HTML Builder\n" << endl;

  cout <<
          Paragraph{
            Image{"http://mathworks.com/"}
          } 
      << endl;

cout << "Builder Facet\n" << endl;




  return 0;
}
