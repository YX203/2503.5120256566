#include "Student.h"
#include <iostream>
using namespace std;

// 默认构造
Student::Student() {
    id = "20230001";
    name = "张三";
    department = "计算机学院";
    maxBorrow = 5;
    borrowedCount = 0;
    for (int i = 0; i < MAX_BORROW_RECORD; i++) {
        borrowedBooks[i] = nullptr;
    }
}

// 带参构造
Student::Student(string sid, string sname, string dept, int max) {
    id = sid;
    name = sname;
    department = dept;
    maxBorrow = max;
    borrowedCount = 0;
    for (int i = 0; i < MAX_BORROW_RECORD; i++) {
        borrowedBooks[i] = nullptr;
    }
}

// 依赖关系核心：Student的成员函数borrowBook以Book&为参数，直接修改Book状态
bool Student::borrowBook(Book& book) {
    // 校验1：学生是否达到借阅上限
    if (borrowedCount >= maxBorrow) {
        cout << "借阅失败：" << name << " 已达最大借阅数量(" << maxBorrow << "本)" << endl;
        return false;
    }
    // 校验2：图书是否在馆可借
    if (!book.getAvailable()) {
        cout << "借阅失败：图书《" << book.getTitle() << "》已被借出" << endl;
        return false;
    }
    // 借阅成功：同步修改双方状态
    book.setAvailable(false);
    borrowedBooks[borrowedCount] = &book;  // 记录借阅的图书指针
    borrowedCount++;
    cout << "借阅成功：" << name << " 借到了《" << book.getTitle() << "》" << endl;
    return true;
}

// 还书：通过引用操作原Book对象
bool Student::returnBook(Book& book) {
    if (book.getAvailable()) {
        cout << "归还失败：图书《" << book.getTitle() << "》本就在馆" << endl;
        return false;
    }
    // 在借阅记录中查找该书并移除
    int idx = -1;
    for (int i = 0; i < borrowedCount; i++) {
        if (borrowedBooks[i] == &book) { idx = i; break; }
    }
    if (idx == -1) {
        cout << "归还失败：" << name << " 未借过《" << book.getTitle() << "》" << endl;
        return false;
    }
    // 从记录数组中移除（后面的往前挪）
    for (int i = idx; i < borrowedCount - 1; i++) {
        borrowedBooks[i] = borrowedBooks[i + 1];
    }
    borrowedBooks[borrowedCount - 1] = nullptr;
    borrowedCount--;

    book.setAvailable(true);
    cout << "归还成功：" << name << " 归还了《" << book.getTitle() << "》" << endl;
    return true;
}

// 显示学生基本信息
void Student::showInfo() {
    cout << "学号：" << id << " | 姓名：" << name
         << " | 院系：" << department
         << " | 已借/最大：" << borrowedCount << "/" << maxBorrow << endl;
}

// 显示已借图书列表
void Student::showBorrowedBooks() {
    cout << name << " 已借图书：" << endl;
    if (borrowedCount == 0) {
        cout << "  （无）" << endl;
        return;
    }
    for (int i = 0; i < borrowedCount; i++) {
        cout << "  " << (i + 1) << ". ";
        borrowedBooks[i]->show();
    }
}

// getter
string Student::getId() { return id; }
string Student::getName() { return name; }
string Student::getDepartment() { return department; }
int Student::getBorrowedCount() { return borrowedCount; }
int Student::getMaxBorrow() { return maxBorrow; }
