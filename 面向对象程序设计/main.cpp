#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "Student.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// 清空输入流，防止死循环
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 打印系统主菜单
void showMenu() {
    cout << "\n========== 图书馆借阅管理系统 ==========" << endl;
    cout << "1. 添加资源 (普通图书/杂志/电子资源)" << endl;
    cout << "2. 删除资源 (按条码)" << endl;
    cout << "3. 显示所有资源" << endl;
    cout << "4. 添加学生" << endl;
    cout << "5. 显示所有学生" << endl;
    cout << "6. 借阅资源" << endl;
    cout << "7. 归还资源" << endl;
    cout << "0. 退出系统" << endl;
    cout << "========================================" << endl;
    cout << "请输入你的选择 (0-7): ";
}

int main() {
    // 创建图书馆（堆区分配）
    Library* lib = new Library("西南科技大学逸夫图书馆");

    // ================= 预先初始化基础数据 =================
    // 1. 预先添加 3 个资源 (体现实验三的多态，基类指针管理派生类对象)
    lib->addResource(new Book("B001", "9787115585386", "C++ Primer", "Stanley B.Lippman", "人民邮电出版社", 128.0, 838, true));
    lib->addResource(new Magazine("M001", "读者", "读者出版传媒", 10.0, true, "2024年第1期", "编辑部"));
    lib->addResource(new EBook("E001", "算法导论 电子版", "机械工业出版社", 50.0, true, 20.5, "http://example.com/algo.pdf"));

    // 2. 预先添加 2 个学生 (满足实验二组合关系)
    lib->addStudent(Student("5120256566", "江彦辉", "计算机科学与技术学院", 5));
    lib->addStudent(Student("20230010", "李四", "计算机学院", 3));
    // ======================================================

    int choice = -1;

    while (true) {
        showMenu();
        if (!(cin >> choice)) { // 如果输入不是数字
            cout << "输入无效，请输入数字！" << endl;
            clearInput();
            continue;
        }

        if (choice == 0) {
            cout << "感谢使用，系统正在退出..." << endl;
            break;
        }

        switch (choice) {
        case 1: { // 添加资源
            int type;
            cout << "\n选择资源类型：1. 普通图书  2. 杂志  3. 电子资源 : ";
            cin >> type;
            clearInput();

            string bc, title, pub;
            double price;
            bool av = true;

            cout << "请输入条码: "; cin >> bc;
            cout << "请输入名称: "; cin >> title;
            cout << "请输入出版社: "; cin >> pub;
            cout << "请输入价格: "; cin >> price;

            if (type == 1) {
                string isbn, author; int pages;
                cout << "请输入ISBN: "; cin >> isbn;
                cout << "请输入作者: "; cin >> author;
                cout << "请输入页数: "; cin >> pages;
                lib->addResource(new Book(bc, isbn, title, author, pub, price, pages, av));
            }
            else if (type == 2) {
                string issue, editor;
                cout << "请输入期号: "; cin >> issue;
                cout << "请输入主编: "; cin >> editor;
                lib->addResource(new Magazine(bc, title, pub, price, av, issue, editor));
            }
            else if (type == 3) {
                double size; string link;
                cout << "请输入文件大小(MB): "; cin >> size;
                cout << "请输入下载链接: "; cin >> link;
                lib->addResource(new EBook(bc, title, pub, price, av, size, link));
            }
            else {
                cout << "未知类型，添加失败。" << endl;
            }
            break;
        }
        case 2: { // 删除资源
            string bc;
            cout << "请输入要删除的资源条码: "; cin >> bc;
            lib->removeResource(bc);
            break;
        }
        case 3: // 显示所有资源
            lib->showAllResources();
            break;

        case 4: { // 添加学生
            string id, name, dept; int maxB;
            cout << "请输入学号: "; cin >> id;
            cout << "请输入姓名: "; cin >> name;
            cout << "请输入院系: "; cin >> dept;
            cout << "请输入最大借阅数量: "; cin >> maxB;
            lib->addStudent(Student(id, name, dept, maxB));
            cout << "学生添加成功！" << endl;
            break;
        }
        case 5: // 显示所有学生
            lib->showAllStudents();
            break;

        case 6: { // 借阅资源
            string id, bc;
            cout << "请输入学生学号: "; cin >> id;
            cout << "请输入资源条码: "; cin >> bc;
            lib->borrowResource(id, bc);
            break;
        }
        case 7: { // 归还资源
            string id, bc;
            cout << "请输入学生学号: "; cin >> id;
            cout << "请输入资源条码: "; cin >> bc;
            lib->returnResource(id, bc);
            break;
        }
        default:
            cout << "无效的选择，请重新输入。" << endl;
            break;
        }
    }

    // 释放堆区内存
    delete lib;
    lib = nullptr;

    return 0;
}