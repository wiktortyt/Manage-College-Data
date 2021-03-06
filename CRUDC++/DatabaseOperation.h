#pragma once

#include <fstream>
#include <filesystem>
#include "College.h"

class DatabaseOperation {
	std::filesystem::path _DBFileName;
	std::ofstream _outFileStream;
	std::ifstream _inFileStream;
	std::ios_base::openmode _openMode;
public:
	DatabaseOperation(std::filesystem::path DBFileName, std::ios_base::openmode openMode);

	DatabaseOperation(const DatabaseOperation& dbOp) = delete;
	
	DatabaseOperation(DatabaseOperation&& dbOp) noexcept;

	//we don't want to copy file streams
	DatabaseOperation& operator=(const DatabaseOperation& dbOp) = delete;

	DatabaseOperation& operator=(DatabaseOperation&& dbOp) noexcept;

	void writeToDB();

	void addEmployeeToDB();

	void addStudentToDB();

	void addCollegeToDB();

	void readFromDB();

	void ShowCollege(std::shared_ptr<College> collegePtr);

	template<typename T>
	std::shared_ptr<T> read(int posOfData);

	~DatabaseOperation();
};