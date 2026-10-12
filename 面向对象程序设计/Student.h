#pragma once
#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Resource; // 前向声明

const int MAX_BORROW_RECORD = 20;

class Student {
private:
    std::string id;
    std::string name;
    std::string department;
    int maxBorrow;
    int borrowedCount;
    std::string borrowedBarcodes[MAX_BORROW_RECORD];
    std::string borrowedTitles[MAX_BORROW_RECORD];

public:
    Student();
    Student(std::string sid, std::string sname, std::string dept, int max);

    // 依赖关系：参数为 Resource 引用
    bool borrowBook(Resource& res);
    bool returnBook(Resource& res);

    void showInfo() const;
    void showBorrowedBooks() const;

    std::string getId() const;
    std::string getName() const;
    std::string getDepartment() const;
    int getBorrowedCount() const;
    int getMaxBorrow() const;
};

#endif