#pragma once

#include "Person.h"

#include <string>
#include <array>

class Employee : public Person
{
protected:
	unsigned short _experienceInTeaching;
public:
	Employee(unsigned short experienceInTeaching, std::array< char, 100> name, std::array< char, 100> surname, unsigned short yearOfBirth)
		: Person(name,  surname, yearOfBirth), _experienceInTeaching(experienceInTeaching) {}
	Employee() = default;
	//Employee(Employee&& e) = default;

	//Employee(const Employee& e) = default;

	unsigned short experienceInTeaching() const {
		return _experienceInTeaching;
	}
};

