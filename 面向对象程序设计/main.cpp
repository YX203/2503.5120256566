#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "Student.h"
#include "Library.h"
#include <iostream>

using namespace std;

int main() {
    cout << "========== 实验一：类与对象（基础类设计、构造重载、封装、ISBN校验） ==========" << endl;

    // 1. 默认构造
    Book book1;
    cout << "--- 1. 默认构造的图书 ---" << endl;
    book1.show();
    cout << "ISBN合法性校验：" << (book1.checkIsbnValid() ? "合法" : "非法") << endl;

    // 2. 带参构造重载
    Book book2("B001", "9787115585386", "C++ Primer", "Stanley B.Lippman",
        "人民邮电出版社", 128.0, 838, true);
    cout << "\n--- 2. 带参构造的图书 ---" << endl;
    book2.show();
    cout << "ISBN合法性校验：" << (book2.checkIsbnValid() ? "合法" : "非法") << endl;

    // 3. setter/getter 封装测试
    Book book3;
    book3.setBarcode("B002");
    book3.setTitle("数据结构与算法分析");
    book3.setAuthor("Mark Allen Weiss");
    book3.setPublisher("机械工业出版社");
    book3.setPrice(69.0);
    book3.setPages(544);
    book3.setIsbn("9787111215080");
    book3.setAvailable(true);
    cout << "\n--- 3. 通过setter设置的图书 ---" << endl;
    book3.show();

    // 4. 非法 ISBN 拦截测试
    cout << "\n--- 4. 测试非法ISBN设置 (预期拦截) ---" << endl;
    book3.setIsbn("123");

    cout << "\n\n========== 实验二 & 实验三：组合、依赖、继承与派生 ==========" << endl;

    // 创建图书馆（组合类，使用 new 分配在堆区）
    Library* lib = new Library("西南科技大学逸夫图书馆");

    // 添加学生 (组合关系: Library has-a Student)
    Student stu1("5120256566", "江彦辉", "计算机科学与技术学院", 5);
    Student stu2("20230010", "李四", "计算机学院", 3);
    lib->addStudent(stu1);
    lib->addStudent(stu2);

    // 【实验三核心】利用多态，统一添加不同类型的资源 (使用 new 动态创建)
    cout << "\n----- 向图书馆添加不同类型的资源 (继承与多态) -----" << endl;
    lib->addResource(new Book("B001", "9787115585386", "C++ Primer", "Stanley B.Lippman", "人民邮电出版社", 128.0, 838, true));
    lib->addResource(new Book("B002", "9787020040101", "活着", "余华", "人民文学出版社", 39.0, 191, true));
    lib->addResource(new Magazine("M001", "读者", "读者出版传媒", 10.0, true, "2024年第1期", "编辑部"));
    lib->addResource(new EBook("E001", "算法导论 电子版", "机械工业出版社", 50.0, true, 20.5, "http://example.com/algo.pdf"));

    lib->showInfo();
    lib->showAllResources();

    // 【实验二核心】依赖关系测试：学生借阅不同类型的资源
    cout << "\n----- 依赖关系：学生借阅 (Student 依赖 Resource) -----" << endl;
    lib->borrowResource("5120256566", "B001"); // 借普通图书
    lib->borrowResource("5120256566", "M001"); // 借杂志
    lib->borrowResource("20230010", "E001");   // 借电子资源
    lib->borrowResource("20230010", "B001");   // 尝试借已借出的书——失败

    cout << "\n----- 借阅后状态 -----" << endl;
    lib->showAllResources();
    lib->showAllStudents();

    // 删除资源测试
    cout << "\n----- 删除未借出资源测试 -----" << endl;
    lib->removeResource("B002");
    lib->showAllResources();

    // 归还测试
    cout << "\n----- 依赖关系：归还操作 -----" << endl;
    lib->returnResource("5120256566", "B001");
    lib->showAllStudents();

    // 释放堆区内存
    delete lib;
    lib = nullptr;

    cout << "\n===== 程序结束 =====" << endl;
    return 0;
}