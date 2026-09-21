#include "Book.h"
#include "Student.h"
#include <iostream>
using namespace std;

int main() {
    // 1. 初始化图书并验证ISBN
    Book book1;
    cout << "===== 初始图书信息 =====" << endl;
    book1.show();
    cout << "ISBN是否合法：" << (book1.checkIsbnValid() ? "是" : "否") << endl << endl;

    // 2. 初始化学生
    Student stu1("20230010", "李四", "计算机学院", 3);
    cout << "===== 初始学生信息 =====" << endl;
    stu1.showInfo();
    cout << endl;

    // 3. 演示借书（依赖关系核心交互）
    cout << "===== 借书操作 =====" << endl;
    stu1.borrowBook(book1);
    cout << endl;

    // 4. 查看借书后双方状态
    cout << "===== 借书后状态 =====" << endl;
    book1.show();
    stu1.showInfo();
    cout << endl;

    // 5. 测试重复借书（失败场景）
    cout << "===== 重复借书测试 =====" << endl;
    stu1.borrowBook(book1);
    cout << endl;

    // 6. 演示还书
    cout << "===== 还书操作 =====" << endl;
    stu1.returnBook(book1);
    cout << endl;

    // 7. 查看还书后状态
    cout << "===== 还书后状态 =====" << endl;
    book1.show();
    stu1.showInfo();

    return 0;
}