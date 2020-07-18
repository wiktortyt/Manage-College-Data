#pragma once

#include "Employee.h"
#include "Student.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

struct CollegeHash {
	size_t operator()(const std::array<char,100>& arr) const {
		try {
			auto x = std::hash<char>{}(arr.at(0));
			auto y = std::hash<char>{}(arr.at(1));
			auto z = std::hash<char>{}(arr.at(2));
			return x ^ y ^ z;
		}
		catch (...) {
			std::cerr << "Something wrong with hashing array" << std::endl;
		}
	}
};

class College
{
	//additional category and info provided by user (map<category,info>, info = set<>), can't serialize std::string, todo
	//std::unordered_map<std::array<char,100>, std::unordered_set< std::array<char,100>,CollegeHash>,CollegeHash> _additionalCategoryAndData;
	//stored employees of college
	std::vector<Employee> _employees;
	//needed when reading
	unsigned int _NofEmployees;
	//stores sutdents of college
	std::vector<Student> _students;
	unsigned int _NofStudents;
	std::array<char,100> _name;

public:
	College(std::vector<Employee> emps, std::vector<Student> studs, std::array<char, 100> name)
		:_employees(emps), _students(studs), _name(name) {
		_NofEmployees = _employees.size();
		_NofStudents = _students.size();
	}
	College() = default;
	College(College&& c) = default;
	College(const College& c) = default;

	friend std::ofstream& operator<<(std::ofstream& ofs, const College college);
	friend std::ifstream& operator>>(std::ifstream& ifs, College& college);

	std::vector<Employee> getEmployees() const {
		return _employees;
	}

	std::vector<Student> getStudents() const noexcept{
		return _students;
	}

	unsigned int getNofEmployees() const noexcept {
		return _NofEmployees;
	}

	unsigned int getNofStudents() const noexcept {
		return _NofStudents;
	}

	std::array<char, 100> getName() const noexcept{
		return _name;
	}
};

