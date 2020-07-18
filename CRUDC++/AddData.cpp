#include "DatabaseOperation.h"

#include "Employee.h"
#include "Student.h"
#include "College.h"

#include <iostream>
#include <memory>
#include <array>

inline Employee addEmployee() {
	std::array< char, 100> name, surname;
	unsigned short yearOfBirth, experienceInTeaching;
	std::cout << "Name: ";
	std::cin >> name.data();
	std::cout << "Surname: ";
	std::cin >> surname.data();
	std::cout << "Year of birth: ";
	std::cin >> yearOfBirth;
	std::cout << "Experience in teaching(years): ";
	std::cin >> experienceInTeaching;
	if (std::cin.fail()) {
		std::cerr << "You entered something wrong" << std::endl;
		return Employee(-1, std::array<char, 100>(), std::array<char, 100>(), -1);
	}

	return Employee(experienceInTeaching, std::move(name), std::move(surname), yearOfBirth);
}

void DatabaseOperation::addEmployeeToDB()
{
	if (_outFileStream.good()) {
		
		std::unique_ptr<Employee> employeePtr = std::make_unique<Employee>(addEmployee());

		int a = _outFileStream.tellp();
		_outFileStream.write(reinterpret_cast<char*>(employeePtr.get()), sizeof(Employee));
		if (_outFileStream.good())
			std::cout << "Employee written!" << std::endl;
		else
			std::cerr << "Something went bad with writing employee!" << std::endl;
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}

inline Student addStudent() {
	std::array< char, 100> name, surname;
	unsigned short yearOfBirth, yearOfLearning;
	std::cout << "Name: ";
	std::cin >> name.data();
	std::cout << "Surname: ";
	std::cin >> surname.data();
	std::cout << "Year of birth: ";
	std::cin >> yearOfBirth;
	std::cout << "Year of learning: ";
	std::cin >> yearOfLearning;
	if (std::cin.fail()) {
		std::cerr << "You entered something wrong" << std::endl;
		return Student();
	}
	return Student(yearOfLearning, std::move(name), std::move(surname), yearOfBirth);
}

void DatabaseOperation::addStudentToDB()
{
	if (_outFileStream.good()) {
		std::unique_ptr<Student> studentPtr = std::make_unique<Student>(addStudent());
		_outFileStream.write(reinterpret_cast<char*>(studentPtr.get()), sizeof(Student));
		if (_outFileStream.good())
			std::cout << "Student written!" << std::endl;
		else
			std::cerr << "Something went bad with writing student!" << std::endl;
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}


inline College addCollege() {
	int NofEmployees = -1;
	int NofStudents = -1;
	std::array<char, 100> name;

	std::cout << "How many employees does college have?: ";
	std::cin >> NofEmployees;
	std::cout << "How many students does college have?: ";
	std::cin >> NofStudents;
	std::cout << "Name of college: ";
	std::cin >> name.data();

	if (std::cin.fail()) {
		std::cerr << "Wrong number!" << std::endl;
	}

	std::vector<Employee> emps;
	std::vector<Student> studs;

	//not checing if employee is valid because checking was made in function addEmployee()
	//it will just add empty employee object
	for (size_t i = 0; i < NofEmployees; i++)
		emps.push_back(addEmployee());

	for (size_t i = 0; i < NofStudents; i++)
		studs.push_back(addStudent());

	return College(std::move(emps), std::move(studs), std::move(name));
}

void DatabaseOperation::addCollegeToDB() {
	if (_outFileStream.good()) {
		std::unique_ptr<College> collegePtr = std::make_unique<College>(addCollege());

		//_outFileStream.write(reinterpret_cast<char*>(collegePtr.get()), sizeof(*collegePtr));
		_outFileStream << *collegePtr;
		if (_outFileStream.good())
			std::cout << "College written!" << std::endl;
		else
			std::cerr << "Something went bad with writing college!" << std::endl;
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}