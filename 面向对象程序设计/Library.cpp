#include "Library.h"
#include <iostream>
using namespace std;

// 默认构造：构造内嵌对象数组时，Book/Student的默认构造会被自动先调用
Library::Library() {
    name = "西南科技大学图书馆";
    bookCount = 0;
    studentCount = 0;
}

Library::Library(string libName) {
    name = libName;
    bookCount = 0;
    studentCount = 0;
}

// 向馆内添加图书：把外部Book拷贝到内嵌对象数组中
bool Library::addBook(const Book& b) {
    if (bookCount >= MAX_BOOKS) {
        cout << "添加失败：图书馆图书已达上限(" << MAX_BOOKS << "本)" << endl;
        return false;
    }
    // 组合：books[bookCount]是Library拥有的Book对象，赋值拷贝
    books[bookCount] = b;
    bookCount++;
    return true;
}

// 按条码删除图书
bool Library::removeBook(string barcode) {
    int idx = -1;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].getBarcode() == barcode) { idx = i; break; }
    }
    if (idx == -1) {
        cout << "删除失败：未找到条码为 " << barcode << " 的图书" << endl;
        return false;
    }
    if (!books[idx].getAvailable()) {
        cout << "删除失败：图书《" << books[idx].getTitle() << "》正被借出，无法删除" << endl;
        return false;
    }
    // 后面的图书往前覆盖
    for (int i = idx; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;
    cout << "删除成功：条码 " << barcode << " 的图书已从馆内移除" << endl;
    return true;
}

// 按条码查找图书，返回指针（供借阅/归还使用）
Book* Library::findBook(string barcode) {
    for (int i = 0; i < bookCount; i++) {
        if (books[i].getBarcode() == barcode) return &books[i];
    }
    return nullptr;
}

// 列出所有图书
void Library::showAllBooks() {
    cout << "===== 【" << name << "】馆藏图书(共" << bookCount << "本) =====" << endl;
    for (int i = 0; i < bookCount; i++) {
        cout << "[" << (i + 1) << "] ";
        books[i].show();
    }
}

// 添加学生
bool Library::addStudent(const Student& s) {
    if (studentCount >= MAX_STUDENTS) {
        cout << "添加失败：注册学生数已达上限" << endl;
        return false;
    }
    students[studentCount] = s;
    studentCount++;
    return true;
}

// 按学号查找学生
Student* Library::findStudent(string id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].getId() == id) return &students[i];
    }
    return nullptr;
}

// 列出所有学生
void Library::showAllStudents() {
    cout << "===== 【" << name << "】注册学生(共" << studentCount << "人) =====" << endl;
    for (int i = 0; i < studentCount; i++) {
        cout << "[" << (i + 1) << "] ";
        students[i].showInfo();
        students[i].showBorrowedBooks();
    }
}

// 图书馆层面的借书：查找学生和图书，然后调用Student::borrowBook(Book&)（依赖关系）
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
    return s->borrowBook(*b);  // 传引用，触发依赖关系
}

// 图书馆层面的还书
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

// 图书馆整体信息
void Library::showInfo() {
    cout << "===== 图书馆信息 =====" << endl;
    cout << "名称：" << name << endl;
    cout << "馆藏图书：" << bookCount << " 本" << endl;
    cout << "注册学生：" << studentCount << " 人" << endl;
}

string Library::getName() { return name; }
int Library::getBookCount() { return bookCount; }
int Library::getStudentCount() { return studentCount; }
