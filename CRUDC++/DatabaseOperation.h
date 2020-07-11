#pragma once

#include <fstream>
#include <filesystem>

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

	void addEmployee();

	void addStudent();

	void readFromDB();

	template<typename T>
	std::shared_ptr<T> read(int posOfData);

	~DatabaseOperation();
};