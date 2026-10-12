#pragma once
#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Book; // 前向声明

const int MAX_BORROW_RECORD = 20;

class Student {
private:
    std::string id;                    // 学号
    std::string name;                  // 姓名
    std::string department;            // 院系
    int maxBorrow;                     // 最大借阅数量
    int borrowedCount;                 // 当前已借数量

    // 用条码和书名副本记录借阅信息，避免保存 Book* 导致数组移动后指针失效
    std::string borrowedBarcodes[MAX_BORROW_RECORD];
    std::string borrowedTitles[MAX_BORROW_RECORD];

public:
    Student();
    Student(std::string sid, std::string sname, std::string dept, int max);

    // 依赖关系：参数为 Book 引用，直接操作原 Book 对象
    bool borrowBook(Book& book);
    bool returnBook(Book& book);

    void showInfo() const;
    void showBorrowedBooks() const;

    std::string getId() const;
    std::string getName() const;
    std::string getDepartment() const;
    int getBorrowedCount() const;
    int getMaxBorrow() const;
};

#endif