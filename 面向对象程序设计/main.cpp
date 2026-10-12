#include "Book.h"
#include "Student.h"
#include "Library.h"
#include <iostream>

using namespace std;

int main() {
    // ========== 实验一：类与对象 ==========
    cout << "========== 实验一：类与对象 ==========" << endl;

    Book book1;
    cout << "--- 默认构造的图书 ---" << endl;
    book1.show();
    cout << "ISBN合法性校验：" << (book1.checkIsbnValid() ? "合法" : "非法") << endl;

    Book book2("002", "9787115585386", "C++ Primer", "Stanley B.Lippman",
        "人民邮电出版社", 128.0, 838, true);
    cout << "\n--- 带参构造的图书 ---" << endl;
    book2.show();
    cout << "ISBN合法性校验：" << (book2.checkIsbnValid() ? "合法" : "非法") << endl;

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

    cout << "\n测试非法ISBN设置：" << endl;
    book3.setIsbn("123");

    // ========== 实验二：组合关系 + 依赖关系 ==========
    cout << "\n========== 实验二：组合关系与依赖关系 ==========" << endl;

    Library lib("西南科技大学逸夫图书馆");
    lib.addBook(book1);
    lib.addBook(book2);
    lib.addBook(book3);
    lib.addBook(Book("004", "9787302599709", "算法导论", "Thomas H.Cormen",
        "清华大学出版社", 128.0, 1312, true));
    lib.addBook(Book("005", "9787115428028", "深入理解计算机系统", "Randal E.Bryant",
        "人民邮电出版社", 139.0, 813, true));

    Student stu1("5120256566", "江彦辉", "计算机科学与技术学院", 5);
    Student stu2("20230010", "李四", "计算机学院", 3);
    lib.addStudent(stu1);
    lib.addStudent(stu2);

    lib.showInfo();
    lib.showAllBooks();

    cout << "\n----- 依赖关系：借书操作 -----" << endl;
    lib.borrowBook("5120256566", "001");
    lib.borrowBook("5120256566", "002");
    lib.borrowBook("20230010", "003");
    lib.borrowBook("20230010", "001");

    cout << "\n----- 借阅后状态 -----" << endl;
    lib.showAllBooks();
    lib.showAllStudents();

    cout << "\n----- 删除未借出图书（安全演示） -----" << endl;
    lib.removeBook("005");
    lib.showAllBooks();

    cout << "\n----- 依赖关系：还书操作 -----" << endl;
    lib.returnBook("5120256566", "001");

    cout << "\n----- 归还后状态 -----" << endl;
    lib.showAllStudents();

    cout << "\n===== 程序结束 =====" << endl;
    return 0;
}