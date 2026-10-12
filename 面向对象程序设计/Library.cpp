#include "Library.h"
#include <iostream>

using namespace std;

Library::Library() : name("西南科技大学图书馆"), bookCount(0), studentCount(0) {}

Library::Library(string libName) : name(libName), bookCount(0), studentCount(0) {}

bool Library::addBook(const Book& b) {
    if (bookCount >= MAX_BOOKS) {
        cout << "添加失败：图书馆图书已达上限(" << MAX_BOOKS << "本)" << endl;
        return false;
    }
    books[bookCount] = b;
    bookCount++;
    return true;
}

bool Library::removeBook(string barcode) {
    int idx = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].getBarcode() == barcode) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "删除失败：未找到条码为 " << barcode << " 的图书" << endl;
        return false;
    }
    if (!books[idx].getAvailable()) {
        cout << "删除失败：图书《" << books[idx].getTitle() << "》正被借出，无法删除" << endl;
        return false;
    }

    for (int i = idx; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;
    cout << "删除成功：条码 " << barcode << " 的图书已从馆内移除" << endl;
    return true;
}

Book* Library::findBook(string barcode) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].getBarcode() == barcode) return &books[i];
    }
    return nullptr;
}

void Library::showAllBooks() const {
    cout << "===== 【" << name << "】馆藏图书(共" << bookCount << "本) =====" << endl;
    for (int i = 0; i < bookCount; i++) {
        cout << "[" << (i + 1) << "] ";
        books[i].show();
    }
}

bool Library::addStudent(const Student& s) {
    if (studentCount >= MAX_STUDENTS) {
        cout << "添加失败：注册学生数已达上限" << endl;
        return false;
    }
    students[studentCount] = s;
    studentCount++;
    return true;
}

Student* Library::findStudent(string id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].getId() == id) return &students[i];
    }
    return nullptr;
}

void Library::showAllStudents() const {
    cout << "===== 【" << name << "】注册学生(共" << studentCount << "人) =====" << endl;
    for (int i = 0; i < studentCount; i++) {
        cout << "[" << (i + 1) << "] ";
        students[i].showInfo();
        students[i].showBorrowedBooks();
    }
}

bool Library::borrowBook(string stuId, string barcode) {
    Student* s = findStudent(stuId);
    if (s == nullptr) {
        cout << "借书失败：未找到学号 " << stuId << " 的学生" << endl;
        return false;
    }
    Book* b = findBook(barcode);
    if (b == nullptr) {
        cout << "借书失败：未找到条码 " << barcode << " 的图书" << endl;
        return false;
    }
    return s->borrowBook(*b);
}

bool Library::returnBook(string stuId, string barcode) {
    Student* s = findStudent(stuId);
    if (s == nullptr) {
        cout << "还书失败：未找到学号 " << stuId << " 的学生" << endl;
        return false;
    }
    Book* b = findBook(barcode);
    if (b == nullptr) {
        cout << "还书失败：未找到条码 " << barcode << " 的图书" << endl;
        return false;
    }
    return s->returnBook(*b);
}

void Library::showInfo() const {
    cout << "===== 图书馆信息 =====" << endl;
    cout << "名称：" << name << endl;
    cout << "馆藏图书：" << bookCount << " 本" << endl;
    cout << "注册学生：" << studentCount << " 人" << endl;
}

string Library::getName() const { return name; }
int Library::getBookCount() const { return bookCount; }
int Library::getStudentCount() const { return studentCount; }