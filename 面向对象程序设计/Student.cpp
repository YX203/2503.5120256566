#include "Student.h"
#include <iostream>
using namespace std;

Student::Student() {
    id = "20230001";
    name = "张三";
    department = "计算机学院";
    maxBorrow = 5;
    borrowedCount = 0;
}

Student::Student(string sid, string sname, string dept, int max) {
    id = sid;
    name = sname;
    department = dept;
    maxBorrow = max;
    borrowedCount = 0;
}

// 依赖关系的核心实现：Student类的函数操作Book类对象
bool Student::borrowBook(Book& book) {
    // 校验1：学生是否达到借阅上限
    if (borrowedCount >= maxBorrow) {
        cout << "借阅失败：" << name << " 已达最大借阅数量" << endl;
        return false;
    }
    // 校验2：图书是否在馆可借
    if (!book.getAvailable()) {
        cout << "借阅失败：图书《" << book.getTitle() << "》已被借出" << endl;
        return false;
    }
    // 借阅成功，同步修改双方状态
    book.setAvailable(false);
    borrowedCount++;
    cout << "借阅成功：" << name << " 借到了《" << book.getTitle() << "》" << endl;
    return true;
}

bool Student::returnBook(Book& book) {
    if (book.getAvailable()) {
        cout << "归还失败：图书《" << book.getTitle() << "》本就在馆" << endl;
        return false;
    }
    book.setAvailable(true);
    borrowedCount--;
    cout << "归还成功：" << name << " 归还了《" << book.getTitle() << "》" << endl;
    return true;
}

void Student::showInfo() {
    cout << "学号：" << id << " | 姓名：" << name
        << " | 院系：" << department
        << " | 已借/最大：" << borrowedCount << "/" << maxBorrow << endl;
}