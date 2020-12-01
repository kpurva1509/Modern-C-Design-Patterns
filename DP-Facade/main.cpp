#include "headeres.hpp"

// Facade design pattern
// Structural design pattern
// Seperates the interface from the implementation details
// Single class that represents an entire subsystem
// Balance the complexity with the presentation and usability
// Easy-to-use API Design
// Facade - simple interface over a large and sophisticated
// body of code

class Customer{

    std::string name;
    Customer(){}

public:

    // default Customer class initialization is avoided
    Customer(const std::string& name):name(name){}

    const std::string& get_name(void) const {
        return name;
    }

};

class Bank{

public:
    bool has_sufficient_savings(const Customer& customer, const int& amount){
        cout << "Check bank for " << customer.get_name() << endl;
        return true;
    }

};

class Credit{

    public:
    bool has_good_credit(const Customer& customer, const int& amount){
        cout << "Check credit for " << customer.get_name() << endl;
    }
};

class Loan
{

public:
    bool has_good_credit(const Customer &customer, const int &amount)
    {
        cout << "Check loan for " << customer.get_name() << endl;
    }
};

class Mortgage{

    Bank bank;
    Credit credit;
    Loan loan;

public:
    bool is_eligible(const Customer& customer, const int& amount){
        bool eligible{false};

        eligible = bank.has_sufficient_savings(customer, amount);
        if(eligible) eligible = loan.has_good_credit(customer, amount);
        if(eligible) eligible = credit.has_good_credit(customer, amount);

        return eligible;
    }
};

int main() {

    Mortgage mortgage;
    Customer customer("Purva Kulkarni");

    bool eligible = mortgage.is_eligible(customer, 45000);

    cout << "\n" << customer.get_name() << " has been " << (eligible ? "approved "
                    : "declined ") << "the loan application" << endl;
 
    return 0;
}