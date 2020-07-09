#pragma once

#include "Employee.h"
#include "Student.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class College
{
	//additional category and info provided by user
	std::unordered_map<std::string, std::unordered_set< std::string>> additionalCategoryAndData;
	//stored employees of college
	std::vector<Employee> employees;
	//needed when reading
	unsigned int NofEmployees;
	//stores sutdents of college
	std::vector<Student> students;
	unsigned int NofStudents;
	std::string name;
};

