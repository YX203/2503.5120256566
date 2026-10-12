#include "Student.h"
#include "Resource.h"
#include <iostream>

using namespace std;

Student::Student()
    : id("20230001"), name("张三"), department("计算机学院"), maxBorrow(5), borrowedCount(0) {
    for (int i = 0; i < MAX_BORROW_RECORD; ++i) {
        borrowedBarcodes[i] = "";
        borrowedTitles[i] = "";
    }
}

Student::Student(string sid, string sname, string dept, int max)
    : id(sid), name(sname), department(dept), borrowedCount(0) {
    if (max < 0) max = 0;
    if (max > MAX_BORROW_RECORD) max = MAX_BORROW_RECORD;
    maxBorrow = max;
    for (int i = 0; i < MAX_BORROW_RECORD; ++i) {
        borrowedBarcodes[i] = "";
        borrowedTitles[i] = "";
    }
}

bool Student::borrowBook(Resource& res) {
    if (borrowedCount >= maxBorrow) {
        cout << "借阅失败：" << name << " 已达最大借阅数量(" << maxBorrow << "本)" << endl;
        return false;
    }
    if (borrowedCount >= MAX_BORROW_RECORD) {
        cout << "借阅失败：借阅记录数组已满" << endl;
        return false;
    }
    if (!res.getAvailable()) {
        cout << "借阅失败：资源《" << res.getTitle() << "》已被借出" << endl;
        return false;
    }

    res.setAvailable(false);
    borrowedBarcodes[borrowedCount] = res.getBarcode();
    borrowedTitles[borrowedCount] = res.getTitle();
    borrowedCount++;

    cout << "借阅成功：" << name << " 借到了《" << res.getTitle() << "》" << endl;
    return true;
}

bool Student::returnBook(Resource& res) {
    if (res.getAvailable()) {
        cout << "归还失败：资源《" << res.getTitle() << "》本就在馆" << endl;
        return false;
    }

    int idx = -1;
    for (int i = 0; i < borrowedCount; ++i) {
        if (borrowedBarcodes[i] == res.getBarcode()) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "归还失败：" << name << " 未借过《" << res.getTitle() << "》" << endl;
        return false;
    }

    for (int i = idx; i < borrowedCount - 1; ++i) {
        borrowedBarcodes[i] = borrowedBarcodes[i + 1];
        borrowedTitles[i] = borrowedTitles[i + 1];
    }
    borrowedCount--;
    borrowedBarcodes[borrowedCount] = "";
    borrowedTitles[borrowedCount] = "";

    res.setAvailable(true);
    cout << "归还成功：" << name << " 归还了《" << res.getTitle() << "》" << endl;
    return true;
}

void Student::showInfo() const {
    cout << "学号：" << id << " | 姓名：" << name
        << " | 院系：" << department
        << " | 已借/最大：" << borrowedCount << "/" << maxBorrow << endl;
}

void Student::showBorrowedBooks() const {
    cout << name << " 已借资源：" << endl;
    if (borrowedCount == 0) {
        cout << "  （无）" << endl;
        return;
    }
    for (int i = 0; i < borrowedCount; ++i) {
        cout << "  " << (i + 1) << ". 条码：" << borrowedBarcodes[i]
            << " | 名称：" << borrowedTitles[i] << endl;
    }
}

string Student::getId() const { return id; }
string Student::getName() const { return name; }
string Student::getDepartment() const { return department; }
int Student::getBorrowedCount() const { return borrowedCount; }
int Student::getMaxBorrow() const { return maxBorrow; }