#include <iostream>
#include <vector>
#include <map>
#include <numeric>
#include <functional>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cmath>
#include <memory>
#include <chrono>
#include <typeindex>

using std::cout;
using std::endl;

// Visitor design pattern
// Motivation -
// Need to define a new operation on an entire class hierarchy
// Need access to non-common aspects of classes in the hierarchy
// Creatwe an external component for handling
// Avoid type-checks(building on dynamic_cast<>())

// 1. Intrusive Visistor
struct Expression{

    // virtual destructor - abstract base
    virtual ~Expression() = default;
    // this print method violates OCP!!!
    virtual void print(std::ostringstream& oss) = 0;
};

struct DoubleExpression : Expression{
    double value;
    DoubleExpression(const double& value):value(value){}
    virtual void print(std::ostringstream& oss) override{
        oss << value;
    }
};

struct AdditionExpression : Expression {

    // std::shared_ptr<Expression> left;
    // std::shared_ptr<Expression> right;

    Expression* left;
    Expression* right;
    AdditionExpression(Expression* left, Expression* right):
                left(left), right(right){}
    
    ~AdditionExpression(){
        delete left;
        delete right;
    }
    virtual void print(std::ostringstream& oss) {
        oss << "(";
        left->print(oss);
        oss << "+";
        right->print(oss);
    }

    // friend std::ostream& operator<<(std::ostream& out, const AdditionExpression& expr);

};

// 2. Reflective Visitor
struct ExpressionPrinter{

    std::ostringstream oss;

    void print(Expression* expr){
        if(auto de = dynamic_cast<DoubleExpression*>(expr)){
            oss << de->value;
        } else if(auto ae = dynamic_cast<AdditionExpression*>(expr)){
            oss << "(";
            print(ae->left);
            oss <<"+";
            print(ae->right);
            oss << ")";
        }
    }

    std::string str() const {
        return oss.str();
    }
};

// 3. Double dispatch - compile time checking

struct DoubleExpression_DD;
struct AdditionExpression_DD;
struct SubtractionExpression_DD;

struct ExpressionVisitor_DD{
    
    // abstract visitor base class
    // overloaded visit methods for each visitable entity of hierarchy
    // visit methods are overloaded and overridden
    virtual void visit(DoubleExpression_DD* de) = 0;
    virtual void visit(AdditionExpression_DD* ae) = 0;
    virtual void visit(SubtractionExpression_DD* se) = 0;
};

struct ExpressionPrinter_DD : ExpressionVisitor_DD{
    std::ostringstream oss;
    /* virtual */ void visit(DoubleExpression_DD* de) override;
    /* virtual */ void visit(AdditionExpression_DD* ae) override;
    /* virtual */ void visit(SubtractionExpression_DD* se) override;

    std::string str() {
        return oss.str();
    }
};

struct Expression_DD{

    // abstract base class for the visitable class hierarchy
    // each element accepts a visitor and passes *this
    virtual void accept(ExpressionVisitor_DD* exprVisitor) = 0;
};

struct DoubleExpression_DD : Expression_DD {
    double value;
    DoubleExpression_DD(const double& value):value(value){}

    /* virtual */ void accept(ExpressionVisitor_DD* exprVisitor){
        exprVisitor->visit(this);
    }
};

struct AdditionExpression_DD : Expression_DD {
    Expression_DD* left;
    Expression_DD* right;

    AdditionExpression_DD(Expression_DD* left, Expression_DD* right):
            left(left), right(right){}

    ~AdditionExpression_DD(){
        delete left;
        delete right;
    }

    /* virtual */ void accept(ExpressionVisitor_DD *exprVisitor)
    {
        exprVisitor->visit(this);
    }
};

struct SubtractionExpression_DD : Expression_DD {
    Expression_DD* left;
    Expression_DD* right;

    SubtractionExpression_DD(Expression_DD* left, Expression_DD* right):
            left(left), right(right){}

    ~SubtractionExpression_DD(){
        delete left;
        delete right;
    }

    /* virtual */ void accept(ExpressionVisitor_DD* exprVisitor){
        exprVisitor->visit(this);
    } 

};

/* virtual */ void ExpressionPrinter_DD::visit(DoubleExpression_DD *de) {
    oss << de->value;
}
/* virtual */ void ExpressionPrinter_DD::visit(AdditionExpression_DD *ae) {

    bool needBraces = dynamic_cast<SubtractionExpression_DD*>(ae->right);

    // oss << "(";
    ae->left->accept(this);
    oss << "+";
    if(needBraces) oss << "(";
    ae->right->accept(this);
    if(needBraces) oss << ")";
}

/* virtual */ void ExpressionPrinter_DD::visit(SubtractionExpression_DD *se) {

    bool needBraces = dynamic_cast<SubtractionExpression_DD*>(se->right);
    se->left->accept(this);
    oss << "-";
    if(needBraces) oss << "(";
    se->right->accept(this);
    if(needBraces) oss << ")";

}

// 4. Acyclic Visitor - RTTI

template<typename Visitable>
struct Visitor{
    virtual void visit(Visitable& obj) = 0;
};

struct VisitorBase {
    // virtual D'tor to ensure V-tables are generated
    virtual ~VisitorBase() = default;
};

struct Expression_AV{
    virtual ~Expression_AV() = default;

    // accept a reference to VisitorBase
    virtual void accept(VisitorBase& visitorBaseObject){
        // accept a marker interface to VisitorBase
        // visitorBaseObject == Visitor<T>
        // T == Current Visitable
        using EV = Visitor<Expression_AV>;
        if(auto ev = dynamic_cast<EV*>(&visitorBaseObject)){
            ev->visit(*this);
        }
    }
};

struct DoubleExpression_AV : Expression_AV {
    double value;
    DoubleExpression_AV(double value):value(value) {}

    // visitorBaseObject is a Visitor<Visitable>
    // call "Visitable's" methods on the "Visitor" object
    // This is because "Vistor" is of the TYPE Visitable
    virtual void accept(VisitorBase& visitorBaseObject) {
        using DEV = Visitor<DoubleExpression_AV>;
        if(auto dev = dynamic_cast<DEV*>(&visitorBaseObject)) {
            dev->visit(*this);
        }
    }
};

struct AdditionExpression_AV : Expression_AV {
    Expression_AV* left;
    Expression_AV* right;

    AdditionExpression_AV(Expression_AV* left, Expression_AV* right):
            left(left), right(right) {}

    virtual void accept(VisitorBase& visitorBaseobject) {
        using AEV = Visitor<AdditionExpression_AV>;
        if(auto aev = dynamic_cast<AEV*>(&visitorBaseobject)) {
            aev->visit(*this);
        }
    }
};

struct ExpressionPrinter_AV : VisitorBase,
                              Visitor<DoubleExpression_AV>,  /* Visitor<Visitable> */
                              Visitor<AdditionExpression_AV> /* Visitor<Visitable> */ {

    /* virtual */ void visit(DoubleExpression_AV& doubleExpr) override {
        oss << doubleExpr.value;
    }

    /* virtual */ void visit(AdditionExpression_AV& additionExpr) override {
        oss << "(";
        additionExpr.left->accept(*this);
        oss << "+";
        additionExpr.right->accept(*this);
        oss << ")";
    }

    std::string str() {
        return oss.str();
    }

private:
    std::ostringstream oss;

};

// 5. Multimethods

struct GameObject;
void collide(GameObject& first, GameObject& second);

struct GameObject{

    // header required #include <typeindex>
    // wrapper for type_info
    virtual std::type_index type() const = 0;

    virtual void collide(GameObject& otherObject){
        // calling global collide function;
        ::collide(*this, otherObject);
    }
};

// CRTP
template<typename T>
struct GameobjectImpl : GameObject {
    /* virtual */ std::type_index type() const override {
        return typeid(T);
        friend T;
    }
    private:
    // to avoid incorrect CRTP inheritance,
    // make base C'tor private, and template parameter a friend
    GameobjectImpl(){}
};

struct Planet : GameobjectImpl<Planet> {
    Planet(){}
};

struct Asteroid : GameobjectImpl<Asteroid>
{
    Asteroid() {}
};

struct Spaceship : GameobjectImpl<Spaceship>
{
    Spaceship(){}
};

void spaceshipVSplanet(){
    cout << "Spaceship lands on planet" << endl;
}

void asteroidVSplanet(){
    cout << "Asteroid burns" << endl;
}

void asteroidVSspaceship() {
    cout << "Asteroid destroys spaceship" << endl;
}

std::map<std::pair<std::type_index, std::type_index>, void(*)()> outcomes {
    {{typeid(Spaceship), typeid(Planet)}, spaceshipVSplanet},
    {{typeid(Asteroid), typeid(Planet)}, asteroidVSplanet},
    {{typeid(Asteroid), typeid(Spaceship)}, asteroidVSspaceship}
};

// global method implementation
void collide(GameObject &first, GameObject &second) {
    auto it = outcomes.find({first.type(), second.type()});
    if(it == outcomes.end()){
        it = outcomes.find({second.type(), first.type()});
        if(it == outcomes.end()){
            cout << "objects pass harmlessly" << endl;
        }
    }
    it->second();
}

    int main()
{

    // 1. Driver code for intrusive visitor
    auto e = new AdditionExpression{
             new DoubleExpression{1},
             new AdditionExpression{
                 new DoubleExpression{2},
                 new DoubleExpression{3}
             }
    };

    std::ostringstream oss;
    e->print(oss);
    cout << oss.str() << endl;

    delete e;

    // 2. Driver for reflective visitor
    auto e2 = new AdditionExpression{
        new DoubleExpression{1},
        new AdditionExpression{
            new DoubleExpression{2},
            new DoubleExpression{3}
        }
    };

    ExpressionPrinter ep;
    ep.print(e2);
    cout << ep.str() << endl;

    delete e2;

    // 3. Driver for double dispatch
    auto e3 = new AdditionExpression_DD{
              new DoubleExpression_DD{1},
              new SubtractionExpression_DD{
                  new DoubleExpression_DD{5},
                  new DoubleExpression_DD{2}
              }
    };

    ExpressionPrinter_DD ep_DD;
    ep_DD.visit(e3);
    cout << ep.str() << endl;

    delete e3;

    // 4. Driver for acyclic visitor
    auto e4 = new AdditionExpression_AV{
              new DoubleExpression_AV{5},
              new AdditionExpression_AV{
                  new DoubleExpression_AV{2},
                  new DoubleExpression_AV{8}
              }
    };

    ExpressionPrinter_AV ep_AV;
    ep_AV.visit(*e4);

    cout << ep_AV.str() << endl;

    delete e4;

    // 5. Driver for Multimethods(Visitor with CRTP)
    Spaceship s;
    Asteroid a;
    Planet p;

}