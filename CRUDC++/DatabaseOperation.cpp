#include "DatabaseOperation.h"
#include "Employee.h"
#include "Student.h"

#include <iostream>
#include <memory>

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
	int openModeForInStream = _openMode & (~std::ios::ate);
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
			for (size_t i = 0; i < size; i++)
			{
				_inFileStream.seekg(i);
				char c = static_cast<char>(_inFileStream.get());
				if (c == '&')
					posOfAmp = (int)_inFileStream.tellg() - 1 ;
			}
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
				addEmployee();
				endingChars += "e";
				break;
			case 1:
				addStudent();
				endingChars += "s";
				break;
			}
		} while (choice != -1);

		int a = _outFileStream.tellp();
		_outFileStream.flush();
		_outFileStream.write(endingChars.c_str(), endingChars.size());
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}

void DatabaseOperation::addEmployee()
{
	if (_outFileStream.good()) {
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
			return;
		}
		std::unique_ptr<Employee> employeePtr = std::make_unique<Employee>(Employee(experienceInTeaching, name, surname, yearOfBirth));

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

void DatabaseOperation::addStudent()
{
	if (_outFileStream.good()) {
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
			return;
		}
		std::unique_ptr<Student> studentPtr = std::make_unique<Student>(Student(yearOfLearning, name, surname, yearOfBirth));
		_outFileStream.write(reinterpret_cast<char*>(studentPtr.get()), sizeof(Student));
		if (_outFileStream.good())
			std::cout << "Employee written!" << std::endl;
		else
			std::cerr << "Something went bad with writing employee!" << std::endl;
	}
	else {
		std::cerr << "Bad file stream" << std::endl;
	}
}

void DatabaseOperation::readFromDB()
{
	if (_inFileStream.good()) {
		//at the end of file there is string of chars what is int the file
		_inFileStream.seekg(-1,std::ios::end);
		int size = _inFileStream.tellg();
		int pos = -1;
		//searching for begging of ending characters
		for (int i = 1; i <= size; i++)
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
		for (size_t i = pos; i <= size; i++)
		{
			char c = static_cast<char>(_inFileStream.get());
			endingChars += c;
		}

		//position of record which we read(in endingChars)
		int posOfData = 0;
		//reading data until end of employee objects
		for (int i = 0; i < size - endingChars.size() -1;)
		{
			_inFileStream.seekg(i, std::ios::beg);
			char typeOfRecord=255;

			if (endingChars.size() > 0)
				typeOfRecord = endingChars[posOfData++];
			else
				std::cerr << "No data to read" << std::endl;

			switch (typeOfRecord)
			{
			case 'e':
			{
				std::shared_ptr<Employee> employee = read<Employee>(i);
				i += sizeof(Employee);
				std::cout << "Experience in teaching: " << employee->experienceInTeaching() << std::endl;
				std::cout << "Name: " << employee->name().data() << std::endl;
				std::cout << "Surname: " << employee->surname().data() << std::endl;
				std::cout << "Year of birth: " << employee->yearOfBirth() << std::endl;
			}
				break;
			default:
				std::cerr << "Wrong type of record" << std::endl;
				break;
			}
		}
	}
	else {
		std::cerr << "Sometnig went wrong with input file" << std::endl;
	}
}

template<typename T>
std::shared_ptr<T> DatabaseOperation::read(int posOfData)
{
	if (_inFileStream.good() && posOfData >= 0) {
		_inFileStream.seekg(posOfData, std::ios::beg);
		std::shared_ptr<T> baseData = std::make_shared<T>(Employee());
		_inFileStream.read(reinterpret_cast<char*>(baseData.get()), sizeof(T));
		return baseData;
	}
	else {
		std::cerr << "Something went wrong" << std::endl;
		return nullptr;
	}
}

DatabaseOperation::~DatabaseOperation() {
	_outFileStream.close();
	_inFileStream.close();
}