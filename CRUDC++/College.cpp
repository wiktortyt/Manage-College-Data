#include "College.h"
#include "Employee.h"
#include "Student.h"

#include <fstream>

std::ofstream& operator<<(std::ofstream& ofs, const College college) {

	//writing employees
	size_t sizeOfEmployees = college._employees.size();
	ofs.write(reinterpret_cast<const char*>(&sizeOfEmployees), sizeof(size_t));


	for (size_t i = 0; i < college._employees.size(); i++)
	{
		unsigned short experienceInTeaching= college._employees[i].experienceInTeaching();
		ofs.write(reinterpret_cast<const char*>(&experienceInTeaching), sizeof(unsigned short));

		size_t sizeOfName = college._employees[i].name().size();
		ofs.write(reinterpret_cast<const char*>(&sizeOfName), sizeof(size_t));

		for (size_t j = 0; j < college._employees[i].name().size(); j++) {
			char c = college._employees[i].name()[j];
			ofs.write(reinterpret_cast<const char*>(&c), sizeof(char));
		}

		size_t surnameSize=college._employees[i].surname().size();
		ofs.write(reinterpret_cast<const char*>(&surnameSize), sizeof(size_t));

		for (size_t j = 0; j < college._employees[i].surname().size(); j++) {
			char c = college._employees[i].surname()[j];
			ofs.write(reinterpret_cast<const char*>(&c), sizeof(char));

		}

		unsigned short yearOfBirth=college._employees[i].yearOfBirth();
		ofs.write(reinterpret_cast<const char*>(&yearOfBirth), sizeof(yearOfBirth));
	}

	//writing students

	size_t sizeOfStudents = college._employees.size();
	ofs.write(reinterpret_cast<const char*>(&sizeOfStudents), sizeof(size_t));

	for (size_t i = 0; i < college._students.size(); i++)
	{
		unsigned short yearOfLearning = college._students[i].yearOfLearning();
		ofs.write(reinterpret_cast<const char*>(&yearOfLearning), sizeof(unsigned short));

		size_t sizeOfName = college._students[i].name().size();
		ofs.write(reinterpret_cast<const char*>(&sizeOfName), sizeof(size_t));

		for (size_t j = 0; j < college._students[i].name().size(); j++) {
			char c = college._students[i].name()[j];
			ofs.write(reinterpret_cast<const char*>(&c), sizeof(char));
		}

		size_t surnameSize = college._students[i].surname().size();
		ofs.write(reinterpret_cast<const char*>(&surnameSize), sizeof(size_t));

		for (size_t j = 0; j < college._students[i].surname().size(); j++) {
			char c = college._students[i].surname()[j];
			ofs.write(reinterpret_cast<const char*>(&c), sizeof(char));

		}

		unsigned short yearOfBirth = college._students[i].yearOfBirth();
		ofs.write(reinterpret_cast<const char*>(&yearOfBirth), sizeof(yearOfBirth));
	}


	//writing college name
	for (size_t i = 0; i < college._name.size(); i++){
		char c = college._name[i];
		ofs.write(reinterpret_cast<const char*>(&c), sizeof(char));
	}

	return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, College& college)
{

	size_t sizeOfEmps;
	ifs.read(reinterpret_cast<char*>(&sizeOfEmps), sizeof(size_t));

	//reading employees

	std::vector<Employee> emps;

	for (size_t i = 0; i < sizeOfEmps; i++)
	{
		unsigned short experienceInTeaching;
		size_t sizeOfName;

		ifs.read(reinterpret_cast<char*>(&experienceInTeaching), sizeof(unsigned short));

		ifs.read(reinterpret_cast<char*>(&sizeOfName), sizeof(size_t));

		std::array<char, 100> nameOfEmployee;

		for (size_t i = 0; i < nameOfEmployee.size(); i++) {
			char c;
			ifs.read(reinterpret_cast<char*>(&c), sizeof(char));
			nameOfEmployee[i] = c;
		}

		size_t sizeOfSurname;

		ifs.read(reinterpret_cast<char*>(&sizeOfSurname), sizeof(size_t));

		std::array<char, 100> surnameOfEmployee;

		for (size_t i = 0; i < surnameOfEmployee.size(); i++) {
			char c;
			ifs.read(reinterpret_cast<char*>(&c), sizeof(char));
			surnameOfEmployee[i] = c;
		}

		unsigned short yearOfBirth;
		ifs.read(reinterpret_cast<char*>(&yearOfBirth), sizeof(unsigned short));

		Employee emp(experienceInTeaching, nameOfEmployee, surnameOfEmployee, yearOfBirth);

		emps.push_back(emp);

	}

	// reading students

	size_t sizeOfStuds;
	ifs.read(reinterpret_cast<char*>(&sizeOfStuds), sizeof(size_t));

	std::vector<Student> studs;

	for (size_t i = 0; i < sizeOfStuds; i++)
	{
		unsigned short yearOfLearning;
		size_t sizeOfName;

		ifs.read(reinterpret_cast<char*>(&yearOfLearning), sizeof(unsigned short));

		ifs.read(reinterpret_cast<char*>(&sizeOfName), sizeof(size_t));

		std::array<char, 100> nameOfStudent;

		for (size_t i = 0; i < nameOfStudent.size(); i++) {
			char c;
			ifs.read(reinterpret_cast<char*>(&c), sizeof(char));
			nameOfStudent[i] = c;
		}

		size_t sizeOfSurname;

		ifs.read(reinterpret_cast<char*>(&sizeOfSurname), sizeof(size_t));

		std::array<char, 100> surnameOfStudent;

		for (size_t i = 0; i < surnameOfStudent.size(); i++) {
			char c;
			ifs.read(reinterpret_cast<char*>(&c), sizeof(char));
			surnameOfStudent[i] = c;
		}

		unsigned short yearOfBirth;
		ifs.read(reinterpret_cast<char*>(&yearOfBirth), sizeof(unsigned short));

		Student stud(yearOfLearning, nameOfStudent, surnameOfStudent, yearOfBirth);

		studs.push_back(stud);
	}


	std::array<char, 100> nameOfCollege;

	for (size_t i = 0; i < nameOfCollege.size(); i++) {
		char c;
		ifs.read(reinterpret_cast<char*>(&c), sizeof(char));
		nameOfCollege[i] = c;
	}

	college._employees = emps;
	college._students = studs;
	college._name = nameOfCollege;
	college._NofEmployees = sizeOfEmps;
	college._NofStudents = sizeOfStuds;

	return ifs;
}
