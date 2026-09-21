#pragma once
#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "Book.h"
using namespace std;

class Student {
private:
    string id;          // 学号
    string name;        // 姓名
    string department;  // 院系
    int maxBorrow;      // 最大借阅数量
    int borrowedCount;  // 当前已借数量

public:
    // 构造函数
    Student();
    Student(string sid, string sname, string dept, int max);

    // 核心功能：借书（依赖关系：传入Book对象引用，直接操作原对象）
    bool borrowBook(Book& book);
    // 还书功能
    bool returnBook(Book& book);
    // 显示学生信息
    void showInfo();
};

#endif