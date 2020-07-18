#pragma once

#include <array>

class Person {
protected:
	std::array< char, 100> _name;
	std::array< char, 100> _surname;
	unsigned short _yearOfBirth;
public:
	Person(std::array< char, 100> name, std::array< char, 100> surname, unsigned short yearOfBirth) :
		_name(name), _surname(surname), _yearOfBirth(yearOfBirth) {}
	Person() {

	}

	std::array< char, 100> name() const {
		return _name;
	}

	std::array< char, 100> surname() const {
		return _surname;
	}

	unsigned short yearOfBirth() const {
		return _yearOfBirth;
	}
};