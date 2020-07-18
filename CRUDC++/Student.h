#pragma once

#include "Person.h"

class Student : public Person
{
protected:
	unsigned short _yearOfLearning;
public:
	Student(unsigned short yearOfLearning, std::array< char, 100> name, std::array< char, 100> surname, unsigned short yearOfBirth)
		: Person(name, surname, yearOfBirth), _yearOfLearning(yearOfLearning) {}
	Student() = default;


	unsigned short yearOfLearning() const {
		return _yearOfLearning;
	}
};

