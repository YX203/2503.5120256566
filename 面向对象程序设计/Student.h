#pragma once
#ifndef STUDENT_H
#define STUDENT_H
#include <string>
#include "Book.h"
using namespace std;

// 最大可借图书数常量（用于学生借阅记录数组大小）
const int MAX_BORROW_RECORD = 20;

// 学生类：实验二依赖关系的"源类"——其成员函数以Book引用为参数
class Student {
private:
    string id;                    // 学号
    string name;                  // 姓名
    string department;            // 院系
    int maxBorrow;                // 最大借阅数量
    int borrowedCount;            // 当前已借数量
    Book* borrowedBooks[MAX_BORROW_RECORD]; // 已借图书指针数组（关联/聚合）
public:
    // 构造函数
    Student();
    Student(string sid, string sname, string dept, int max);

    // 核心功能：借书（依赖关系：传入Book对象引用，直接操作原对象）
    bool borrowBook(Book& book);
    // 还书功能
    bool returnBook(Book& book);

    // 显示学生信息（含已借图书列表）
    void showInfo();
    void showBorrowedBooks();

    // 获取接口
    string getId();
    string getName();
    string getDepartment();
    int getBorrowedCount();
    int getMaxBorrow();
};
#endif
