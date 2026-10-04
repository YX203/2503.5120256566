#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>
#include "Book.h"
#include "Student.h"
using namespace std;

// 容量上限常量
const int MAX_BOOKS = 100;     // 图书馆最多容纳图书数
const int MAX_STUDENTS = 50;   // 图书馆最多注册学生数

// 图书馆类（整体类 / 组合类）
// 组合关系：Library "has a" 多个Book对象 + 多个Student对象
// 即Book/Student作为Library的内嵌对象成员（对象数组形式）
class Library {
private:
    string name;                 // 图书馆名称
    Book books[MAX_BOOKS];       // 内嵌图书对象数组（组合：部分对象生命周期由整体控制）
    int bookCount;               // 当前图书数量
    Student students[MAX_STUDENTS]; // 内嵌学生对象数组（组合）
    int studentCount;            // 当前学生数量
public:
    // 构造函数：负责初始化本类基本成员，内嵌对象会先自动调用默认构造
    Library();
    Library(string libName);

    // 图书管理
    bool addBook(const Book& b);       // 向馆内添加图书（组合：拷贝到内嵌数组）
    bool removeBook(string barcode);   // 按条码删除图书
    Book* findBook(string barcode);    // 按条码查找图书（返回指针用于借阅）
    void showAllBooks();               // 列出所有图书

    // 学生管理
    bool addStudent(const Student& s);
    Student* findStudent(string id);
    void showAllStudents();

    // 借阅与归还（在图书馆层面协调学生与图书，调用依赖关系的borrowBook/returnBook）
    bool borrowBook(string stuId, string barcode);
    bool returnBook(string stuId, string barcode);

    // 显示图书馆整体信息
    void showInfo();

    // 获取接口
    string getName();
    int getBookCount();
    int getStudentCount();
};
#endif
