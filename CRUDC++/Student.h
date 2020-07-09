#pragma once

#include "Person.h"

class Student : public Person
{
protected:
	unsigned short _yearOfLearning;
public:
	Student(unsigned short yearOfLearning, std::string name, std::string surname, unsigned short yearOfBirth)
		: Person(name, surname, yearOfBirth), _yearOfLearning(yearOfLearning) {}
	Student() = default;
};

