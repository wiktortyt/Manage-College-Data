#pragma once

#include "Person.h"

#include <string>

class Employee : public Person
{
protected:
	unsigned short _experienceInTeaching;
public:
	Employee(unsigned short experienceInTeaching, std::string name, std::string surname, unsigned short yearOfBirth) 
		: Person(name,  surname, yearOfBirth), _experienceInTeaching(experienceInTeaching) {}
	Employee() = default;
	Employee(Employee&& e) = default;

	Employee(const Employee& e) = default;

	unsigned short experienceInTeaching() {
		return _experienceInTeaching;
	}
};

