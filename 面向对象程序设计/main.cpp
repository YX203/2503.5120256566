#include "Book.h"
#include "Student.h"
#include "Library.h"
#include <iostream>
using namespace std;

int main() {
    // ========== 一、实验一：类与对象 ==========
    cout << "========== 实验一：类与对象 ==========" << endl;
    // 1. 使用默认构造函数创建图书对象
    Book book1;
    cout << "--- 默认构造的图书 ---" << endl;
    book1.show();
    cout << "ISBN合法性校验：" << (book1.checkIsbnValid() ? "合法" : "非法") << endl;

    // 2. 使用带参构造函数重载创建图书对象
    Book book2("002", "9787115585386", "C++ Primer", "Stanley B.Lippman", "人民邮电出版社", 128.0, 838, true);
    cout << "\n--- 带参构造的图书 ---" << endl;
    book2.show();
    cout << "ISBN合法性校验：" << (book2.checkIsbnValid() ? "合法" : "非法") << endl;

    // 3. 测试set/get接口（封装性）
    Book book3;
    book3.setBarcode("003");
    book3.setTitle("数据结构与算法分析");
    book3.setAuthor("Mark Allen Weiss");
    book3.setPublisher("机械工业出版社");
    book3.setPrice(69.0);
    book3.setPages(544);
    book3.setIsbn("9787111215080");
    book3.setAvailable(true);
    cout << "\n--- 通过setter设置的图书 ---" << endl;
    book3.show();

    // ========== 二、实验二：组合关系 + 依赖关系 ==========
    cout << "\n========== 实验二：组合关系与依赖关系 ==========" << endl;

    // ----- 组合关系演示：Library "has a" Book[] + Student[] -----
    Library lib("西南科技大学逸夫图书馆");
    lib.addBook(book1);
    lib.addBook(book2);
    lib.addBook(book3);
    // 再添加两本图书
    lib.addBook(Book("004", "9787302599709", "算法导论", "Thomas H.Cormen", "清华大学出版社", 128.0, 1312, true));
    lib.addBook(Book("005", "9787115428028", "深入理解计算机系统", "Randal E.Bryant", "人民邮电出版社", 139.0, 813, true));

    Student stu1("5120256566", "江彦辉", "计算机科学与技术学院", 5);
    Student stu2("20230010", "李四", "计算机学院", 3);
    lib.addStudent(stu1);
    lib.addStudent(stu2);

    lib.showInfo();
    lib.showAllBooks();

    // ----- 依赖关系演示：Student.borrowBook(Book&) 通过引用使用Book -----
    cout << "\n----- 依赖关系：借书操作 -----" << endl;
    lib.borrowBook("5120256566", "001");  // 江彦辉借《活着》
    lib.borrowBook("5120256566", "002");  // 江彦辉借《C++ Primer》
    lib.borrowBook("20230010", "003");    // 李四借《数据结构》
    lib.borrowBook("20230010", "001");    // 尝试借已借出的书——失败

    cout << "\n----- 借阅后状态 -----" << endl;
    lib.showAllBooks();
    lib.showAllStudents();

    cout << "\n----- 依赖关系：还书操作 -----" << endl;
    lib.returnBook("5120256566", "001");  // 江彦辉归还《活着》

    cout << "\n----- 归还后状态 -----" << endl;
    lib.showAllStudents();

    cout << "\n===== 程序结束 =====" << endl;
    return 0;
}
