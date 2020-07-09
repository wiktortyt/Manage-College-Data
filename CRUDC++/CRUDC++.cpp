// CRUDC++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include "Employee.h"
#include "DatabaseOperation.h"

constexpr int READ = 0;
constexpr int WRITE = 1;
constexpr int CREATEFILE = 2;
constexpr int APPENDFILE = 3;

int main()
{

    namespace fs = std::filesystem;
    fs::path currentDir = fs::current_path();
    std::string_view fileName{ "fileDB.bin" };
    currentDir /= fileName;
    
    int choice = -1;


    do {
        std::cout << "Do you want to read(0) from DB or write(1) to DB? Type number" << std::endl;
        std::cin >> choice;
    } while (choice != WRITE && choice != READ);

    if (choice == WRITE) {
        try {
            std::cout << "Create new file("<< CREATEFILE << ") or append to file("<<APPENDFILE<<")? " << std::endl;
            int choiceCrOp = -1;
            std::cin >> choiceCrOp;
            if (choiceCrOp == CREATEFILE && std::cin.good()) {
                std::unique_ptr<DatabaseOperation> dbOpPtr = std::make_unique<DatabaseOperation>(DatabaseOperation(currentDir, std::ios::binary));
                dbOpPtr->writeToDB();
            }else if(choiceCrOp== APPENDFILE && std::cin.good()){
                std::unique_ptr<DatabaseOperation> dbOpPtr = std::make_unique<DatabaseOperation>(DatabaseOperation(currentDir, std::ios::binary|std::ios::app));
                dbOpPtr->writeToDB();
            }
            else {
                std::cerr << "Wrong choice for creating file" << std::endl;
            }
        }
        catch (std::exception& ex) {
            std::cerr << "Error opening file to write: " << ex.what() << std::endl;
        }
    }
    else if (choice == READ) {
        try {
            //ios::app to not delete data before reading
            std::unique_ptr<DatabaseOperation> dbOpPtr = std::make_unique<DatabaseOperation>(DatabaseOperation(currentDir, std::ios::binary|std::ios::app));
            dbOpPtr->readFromDB();
        }
        catch (std::exception& ex) {
            std::cerr << "Error opening file to read: " << ex.what() << std::endl;
        }
    }

    std::cout << "Worked on: " << currentDir << std::endl;


}