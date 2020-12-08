#include<iostream>
#include<vector>
#include<cstdio>
#include<sstream>
#include<string>
#include<fstream>
#include<memory>
#include<cmath>

#define _USE_MATH_DEFINES_

using std::cout;
using std::endl;

//#include BOOST_USER_CONFIG

#include</Users/purvakulkarni/Desktop/boost/config/user.hpp>
#include</Users/purvakulkarni/Desktop/boost/config.hpp>
#include</Users/purvakulkarni/Desktop/boost/config.hpp>
#include</Users/purvakulkarni/Desktop/boost/serialization/serialization.hpp>

#include</Users/purvakulkarni/Desktop/boost/archive/text_iarchive.hpp>
#include</Users/purvakulkarni/Desktop/boost/archive/text_oarchive.hpp>

using namespace boost;

struct Address{

	std::string street, city;
	int suite;

	Address(){}

	// constructor to initialize the address
	Address(const std::string& street, const std::string& city, const int& suite): street(street), city(city), suite(suite) {}

	// copy constructor for address
	Address(const Address& other) {

		street = other.street;
		city = other.city;
		suite = other.suite;

	}

	// copy assignment operator for address
	Address& operator=(const Address& other) {

		street = other.street;
		city = other.city;
		suite = other.suite;

		return *this;

	}

	friend class serialization::access;

	template<class archive>
	void serialize(archive& ar, const unsigned version){

		ar & street;
		ar & city;
		ar & suite;

	}

	friend std::ostream& operator<<(std::ostream& oss, const Address& address);
};

std::ostream& operator<<(std::ostream& oss, const Address& address) {

	oss << address.street << " " << address.city << " " << address.suite;
	return oss;
}

struct Contact {

	std::string name;
	Address* address;

	//Contact(const std::string& name, const Address& address): name(name), address(address){}

	Contact(){}

	Contact(const std::string& name, Address* address): name(name), address(address){}

	// explicit implementation of copy constructor is required for deep copy
	Contact(const Contact& other) {

		name = other.name;
		address = new Address{other.address->street, other.address->city, other.address->suite};

		// implementing copy constructor for address
		// other.address is a REFERENCE, so while copying it, we still have to dereference it
		// address = new Address{*other.address};

	}

	// explicit implementation of assignment operator
	Contact& operator=(const Contact& other) {

		// delete the currently existing information in address
		// it is referring to the old address location in heap
		delete address;
		address = nullptr;

		name = other.name;

		// create a new address location
		address = new Address{other.address->street, other.address->city, other.address->suite};

		return *this;

	}

	// PROTOTYPE VIA SERIALIZATION

private:

	friend class serialization::access;

	template<class archive>
	void serialize(archive& ar, const unsigned version){

		ar & name;
		ar & address;

	}

	~Contact() {

		delete address;
		address = nullptr;

	}

	friend std::ostream& operator<<(std::ostream& oss, const Contact& contact);

};

std::ostream& operator<<(std::ostream& oss, const Contact& contact){

	// contact.address is a pointer so we have to dereference it
	oss << contact.name << " \n" << *contact.address;
	return oss;
}

// General prototype for people to work with
// Contact prototype for main office employees
// Can be used with a copy constructor or copy assignment
//Contact main{"", new Address{"123 East Dr", "London", 0}};

// Create an EMPLOYEE FACTORY

struct EmployeeFactory {

private:

	static std::unique_ptr<Contact> newEmployee(const std::string& name, const int suite, Contact& prototype) {

		auto result = std::make_unique<Contact>(prototype);
		result->name = name;
		result->address->suite = suite;
		return result;

	}

public:

	// public factory method to create new employees from
	// the factory method CREATES new employees by calling the NEWEMPLOYEE method

	static std::unique_ptr<Contact> newMainOfficeEmployee(const std::string& name, const int suite){

		static Contact main{"", new Address{"123 East Dr", "London", 0}};

		return newEmployee(name, suite, main);

	}
};

int main() {

	Contact john{"John Doe", new Address{"123 East Dr", "London", 123}};
	//Contact jane{"Jane Smith", new Address{"123 East Dr", "London", 543}};
	//Contact paul{"Paul Richard", new Address{"123 East Dr", "London", 248}};

	Contact jane{john};
	jane.name = "Jane Smith";
	jane.address->suite = 543;

	auto purva = EmployeeFactory::newMainOfficeEmployee("Purva Kulkarni", 347);

	// a pointer is created so we have to dereference it before call to print
	cout << *purva << endl;

	auto clone = [](const Contact& c){

		std::ostringstream oss;
		archive::text_oarchive oa(oss);
		oa << c;
		std::string s = oss.str();

		cout << s << endl;

		std::istringstream iss;
		archive::text_iarchive ia(iss);
		Contact result;
		ia >> result;

		return result;

	};

	auto jack = EmployeeFactory::newMainOfficeEmployee("Jack", 201);
	auto fred = clone(*jack);


	return 0;
}
