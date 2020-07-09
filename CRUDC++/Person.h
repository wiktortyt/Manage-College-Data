#pragma once
class Person {
protected:
	std::string _name, _surname;
	unsigned short _yearOfBirth;
public:
	Person(std::string name, std::string surname, unsigned short yearOfBirth) :
		_name(name), _surname(surname), _yearOfBirth(yearOfBirth) {}
	Person() = default;

	std::string name() {
		return _name;
	}
};