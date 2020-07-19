#include "DatabaseOperation.h"
#include "Employee.h"
#include "Student.h"
#include "College.h"

#include <iostream>
#include <memory>

template<>
std::shared_ptr<College> DatabaseOperation::read(int posOfData) {
	if (_inFileStream.good() && posOfData >= 0) {
		std::shared_ptr<College> college=std::make_shared<College>(College());
		_inFileStream.seekg(posOfData,std::ios::beg);
		_inFileStream >> *college;
		return college;
	}
	else {
		std::cerr << "Something went wrong" << std::endl;
		return nullptr;
	}
}

template<typename T>
std::shared_ptr<T> DatabaseOperation::read(int posOfData)
{
	if (_inFileStream.good() && posOfData >= 0) {
		_inFileStream.seekg(posOfData, std::ios::beg);
		std::shared_ptr<T> baseData = std::make_shared<T>(T());
		_inFileStream.read(reinterpret_cast<char*>(baseData.get()), sizeof(T));
		return baseData;
	}
	else {
		std::cerr << "Something went wrong" << std::endl;
		return nullptr;
	}
}

DatabaseOperation::DatabaseOperation(DatabaseOperation&& dbOp) noexcept
	: _DBFileName(std::move(dbOp._DBFileName)),
	_outFileStream(std::move(dbOp._outFileStream)),
	_inFileStream(std::move(dbOp._inFileStream)),
	_openMode(dbOp._openMode){

}

DatabaseOperation::DatabaseOperation(std::filesystem::path DBFileName,std::ios_base::openmode openMode)
	: _DBFileName(DBFileName),_openMode(openMode) {
	//using ios::in to not erase file
	_outFileStream.open(DBFileName,openMode | std::ios::in | std::ios::out);
	//erasing ios::ate to not erase file
	std::ios_base::openmode openModeForInStream = _openMode & (~std::ios::ate);
	_inFileStream.open(DBFileName, openModeForInStream);
}

DatabaseOperation& DatabaseOperation::operator=(DatabaseOperation&& dbOp) noexcept {
	if (&dbOp == this)
		return *this;
	_DBFileName = dbOp._DBFileName;
	_outFileStream = std::move(dbOp._outFileStream);
	_inFileStream = std::move(dbOp._inFileStream);
	dbOp._DBFileName = "";
	_openMode = dbOp._openMode;
	return *this;
}

//writing data to DB
void DatabaseOperation::writeToDB() {
	if (_outFileStream.good()) {
		//char at the end of file which signifies what was written to file
		//helps at reading file
		std::string endingChars="&";
		int posOfAmp = -1;
		//if appending to file
		if ((_openMode & std::ios::ate) == std::ios::ate) {
			_inFileStream.seekg(0,std::ios::end);
			int size = _inFileStream.tellg();
			//get position of meaningful apersand
			for (size_t i = 0; i < size; i++)
			{
				_inFileStream.seekg(i);
				char c = static_cast<char>(_inFileStream.get());
				if (c == '&')
					posOfAmp = (int)_inFileStream.tellg() - 1 ;
			}
			//if there is no ampersand you are writing first record
			if (posOfAmp > -1) {
				endingChars = "&";
				std::filebuf* temp = _inFileStream.rdbuf();
				temp->pubseekpos(posOfAmp, std::ios::in);
				char endingChar = '\0';
				while ((endingChar = (char)temp->snextc() )!= std::char_traits<char>::eof()) {
					endingChars += endingChar;
				};
				
				_outFileStream.seekp(posOfAmp);
				int a = _outFileStream.tellp();
			}
		}
		int choice = -1;
		do {
			std::cout << "Select what you want to write:\n"
				"Employee(0)\n"
				"Student(1)\n"
				"College(2)\n"
				"Exit(-1)" << std::endl;
			std::cin >> choice;
			if (std::cin.fail()) {
				std::cerr << "Please enter a number. Exiting" << std::endl;
				return;
			}
			//switching your choice
			switch (choice) {
			case 0:
				addEmployeeToDB();
				endingChars += "e";
				break;
			case 1:
				addStudentToDB();
				endingChars += "s";
				break;
			case 2:
				addCollegeToDB();
				endingChars += "c";
				break;
			}
		} while (choice != -1);

		_outFileStream.flush();
		_outFileStream.write(endingChars.c_str(), endingChars.size());
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}

//shows college record data
void DatabaseOperation::ShowCollege(std::shared_ptr<College> collegePtr) {
	std::cout << "Printing college " << collegePtr->getName().data() << std::endl;
	std::cout << "Number of employees: " << collegePtr->getNofEmployees() << std::endl;
	std::cout << "Employees: " << std::endl;
	std::vector<Employee> emps = collegePtr->getEmployees();

	for (size_t i = 0; i < collegePtr->getNofEmployees(); i++)
	{
		std::cout << "Name: " << emps[i].name().data() << std::endl;
		std::cout << "Surname: " << emps[i].surname().data() << std::endl;
		std::cout << "Year of birth: " << emps[i].yearOfBirth() << std::endl;
		std::cout << "Experience in teaching: " << emps[i].experienceInTeaching() << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Number of students: " << collegePtr->getNofStudents() << std::endl;
	std::cout << "Students: " << std::endl;
	
	std::vector<Student> studs = collegePtr->getStudents();

	for (size_t i = 0; i < collegePtr->getNofStudents(); i++)
	{
		std::cout << "Name: " << studs[i].name().data() << std::endl;
		std::cout << "Surname: " << studs[i].surname().data() << std::endl;
		std::cout << "Year of birth: " << studs[i].yearOfBirth() << std::endl;
		std::cout << "Year of learning: " << studs[i].yearOfLearning() << std::endl;
	}

	std::cout << std::endl;
}


void DatabaseOperation::readFromDB()
{
	if (_inFileStream.good()) {
		//at the end of file there is string of chars what is in the file
		_inFileStream.seekg(0,std::ios::end);
		int size = _inFileStream.tellg();
		int pos = -1;
		//searching for begining of ending characters
		for (int i = 1; i < size; i++)
		{
			_inFileStream.seekg(-i, std::ios::end);
			char c = static_cast<char>(_inFileStream.get());
			if (c == '&') {
				pos = _inFileStream.tellg();
				break;
			}
		}
		_inFileStream.seekg(pos, std::ios::beg);
		//reading ending characters
		std::string endingChars = "";
		for (size_t i = pos; i < size; i++)
		{
			char c = static_cast<char>(_inFileStream.get());
			endingChars += c;
		}

		//position of record which we read(in endingChars)
		int posOfData = 0;
		//reading data until end of objects
		for (int i = 0; i < size - endingChars.size() -1;)
		{
			_inFileStream.seekg(i, std::ios::beg);
			char typeOfRecord=255;

			if (endingChars.size() > 0)
				typeOfRecord = endingChars[posOfData++];
			else {
				std::cerr << "No data to read" << std::endl;
				return;
			}
			switch (typeOfRecord)
			{
			case 'e':
			{
				std::shared_ptr<Employee> employee = read<Employee>(i);
				i += sizeof(Employee);
				std::cout << "Experience in teaching: " << employee->experienceInTeaching() << std::endl;
				std::cout << "Name: " << employee->name().data() << std::endl;
				std::cout << "Surname: " << employee->surname().data() << std::endl;
				std::cout << "Year of birth: " << employee->yearOfBirth() << std::endl << std::endl;;
			}
				break;
			case 's':
			{
				std::shared_ptr<Student> student = read<Student>(i);
				i += sizeof(Student);
				std::cout << "Year of learning: " << student->yearOfLearning() << std::endl;
				std::cout << "Name: " << student->name().data() << std::endl;
				std::cout << "Surname: " << student->surname().data() << std::endl;
				std::cout << "Year of birth: " << student->yearOfBirth() << std::endl << std::endl;;
			}
				break;
			case 'c':
			{
				std::shared_ptr<College> college = read<College>(i);
				DatabaseOperation::ShowCollege(college);

				i += college->getNofEmployees() * sizeof(Employee);
				i += college->getNofStudents() * sizeof(Student);
				i += college->getName().size();
				//stores sizes of vectors of employees and students
				i += 2 * sizeof(size_t);
				//sizes of name and surname of students and employees
				i += 2 * sizeof(size_t) * college->getNofEmployees();
				i += 2 * sizeof(size_t) * college->getNofStudents();
				break;
			}
			default:
				std::cerr << "Wrong type of record in file" << std::endl;
				break;
			}
		}
	}
	else {
		std::cerr << "Somethig went wrong with input file" << std::endl;
	}
}


DatabaseOperation::~DatabaseOperation() {
	_outFileStream.close();
	_inFileStream.close();
}