// ============================================================
// 图书馆借阅管理系统 —— 主程序
//
// 题目来源：《面向对象程序设计（C++）实验指导书》实验一 ~ 实验四
//           基础题目三：图书馆借阅管理系统
//
// 本版本覆盖的知识点：
//   实验一  类与对象：类的设计、访问控制、构造/重载构造、数据合法性校验
//   实验二  组合与依赖：Library 组合 Resource / Reader；Reader 依赖 Resource
//   实验三  继承与派生：Resource -> Book / Magazine / EBook
//                       Reader   -> Student / Teacher
//   实验四  多态与虚函数：纯虚函数、抽象类、虚析构、
//                       基类指针数组 + 虚函数调用、操作符重载
//
// 所有功能都有完整的键盘输入与屏幕输出。
// ============================================================
#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "Student.h"
#include "Teacher.h"
#include "Library.h"
#include "TimeUtil.h"

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;

// ---------------- 输入工具：保证输入非法时不会死循环 ----------------
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            clearInput();
            return v;
        }
        cout << "  输入无效，请输入一个整数。" << endl;
        clearInput();
    }
}

double readDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v) {
            clearInput();
            return v;
        }
        cout << "  输入无效，请输入一个数字。" << endl;
        clearInput();
    }
}

string readString(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s) {
            clearInput();
            if (!s.empty()) return s;
        }
        cout << "  输入不能为空，请重新输入。" << endl;
        clearInput();
    }
}

// ---------------- 菜单 ----------------
void showMenu() {
    cout << "\n================== 图书馆借阅管理系统 ==================" << endl;
    cout << "  1. 显示全部馆藏资源" << endl;
    cout << "  2. 添加新资源（普通图书 / 杂志 / 电子资源）" << endl;
    cout << "  3. 增加已有资源的数量" << endl;
    cout << "  4. 减少已有资源的数量" << endl;
    cout << "  5. 删除某一资源" << endl;
    cout << "  6. 查询某一资源的详细信息" << endl;
    cout << "  7. 按价格排序后显示馆藏" << endl;
    cout << "  8. 添加借阅人（学生 / 教师）" << endl;
    cout << "  9. 显示全部借阅人" << endl;
    cout << " 10. 借阅资源" << endl;
    cout << " 11. 归还资源（自动计算逾期罚款）" << endl;
    cout << " 12. 查看逾期情况" << endl;
    cout << " 13. 查看全部借阅记录" << endl;
    cout << " 14. 查看图书馆统计信息" << endl;
    cout << " 15. 模拟时间推进（用于演示逾期罚款）" << endl;
    cout << "  0. 退出系统" << endl;
    cout << "========================================================" << endl;
}

// ---------------- 各功能实现 ----------------

// 2. 添加新资源
void addResourceUI(Library& lib) {
    cout << "\n请选择资源类型：" << endl;
    cout << "  1. 普通图书   2. 杂志   3. 电子资源" << endl;
    int type = readInt("类型编号: ");

    string bc    = readString("条码号: ");
    string title = readString("名称: ");
    string pub   = readString("出版社/出品方: ");
    double price = readDouble("价格(元): ");
    int    count = readInt("本次入库数量(册): ");
    if (count <= 0) {
        cout << "数量必须大于 0，添加失败。" << endl;
        return;
    }

    Resource* res = nullptr;   // 基类指针接收派生类对象 —— 多态的起点
    if (type == 1) {
        string isbn  = readString("ISBN(13位数字): ");
        string author = readString("作者: ");
        int    pages = readInt("页数: ");
        res = new Book(bc, isbn, title, author, pub, price, pages, count);
    }
    else if (type == 2) {
        string issue  = readString("期号(如 2024年第5期): ");
        string editor = readString("主编: ");
        res = new Magazine(bc, title, pub, price, issue, editor, count);
    }
    else if (type == 3) {
        double size = readDouble("文件大小(MB): ");
        string link = readString("下载链接: ");
        res = new EBook(bc, title, pub, price, size, link, count);
    }
    else {
        cout << "未知的类型，添加失败。" << endl;
        return;
    }

    if (lib.addResource(res)) {
        cout << "添加成功！类型：" << res->getTypeName()
            << "，借期 " << res->getLendDays() << " 天。" << endl;
    }
    else {
        delete res;   // 添加失败要自己回收，避免内存泄漏
    }
}

// 8. 添加借阅人
void addReaderUI(Library& lib) {
    cout << "\n请选择借阅人身份：" << endl;
    cout << "  1. 学生   2. 教师" << endl;
    int type = readInt("身份编号: ");

    string id   = readString(type == 1 ? "学号: " : "工号: ");
    string name = readString("姓名: ");
    string dept = readString("院系: ");
    int    max  = readInt("最大借阅数量: ");

    Reader* r = nullptr;
    if (type == 1) {
        string major = readString("专业: ");
        int    grade = readInt("年级(1-5): ");
        r = new Student(id, name, dept, max, major, grade);
    }
    else if (type == 2) {
        string title   = readString("职称(教授/副教授/讲师): ");
        string college = readString("所属学院: ");
        r = new Teacher(id, name, dept, max, title, college);
    }
    else {
        cout << "未知的身份类型，添加失败。" << endl;
        return;
    }

    if (lib.addReader(r)) {
        cout << "添加成功！身份：" << r->getReaderType()
            << "，最大借阅 " << r->getMaxBorrow() << " 本。" << endl;
    }
    else {
        delete r;
    }
}

// 预先载入的演示数据
void initDemoData(Library& lib) {
    cout << "\n正在载入演示数据..." << endl;

    // 三种派生类对象，全部交给基类指针管理（实验三 new + 实验四 多态）
    lib.addResource(new Book("B001", "9787115585386", "C++ Primer",
                             "Stanley B.Lippman", "人民邮电出版社", 128.0, 838, 3));
    lib.addResource(new Magazine("M001", "读者", "读者出版传媒", 10.0,
                                 "2024年第5期", "编辑部", 5));
    lib.addResource(new EBook("E001", "算法导论 电子版", "机械工业出版社", 50.0,
                              20.5, "http://example.com/algo.pdf"));

    // 两种派生类借阅人
    lib.addReader(new Student("5120256566", "江彦辉", "计算机科学与技术学院", 5,
                              "计算机科学与技术", 2));
    lib.addReader(new Teacher("T001", "刘畅", "计算机科学与技术学院", 10,
                              "副教授", "计算机科学与技术学院"));

    cout << "演示数据载入完成：3 项资源、2 位借阅人。" << endl;
}

int main() {
    // 图书馆对象用 new 创建在堆上（实验三：new / delete 的使用）
    Library* lib = new Library("西南科技大学逸夫图书馆");

    initDemoData(*lib);

    int choice = -1;
    while (true) {
        showMenu();
        choice = readInt("请输入你的选择(0-15): ");
        if (choice == 0) {
            cout << "感谢使用，系统正在退出..." << endl;
            break;
        }

        switch (choice) {
        case 1:
            lib->showAllResources();
            break;

        case 2:
            addResourceUI(*lib);
            break;

        case 3: {
            string bc = readString("要增加数量的资源条码: ");
            int n = readInt("增加册数: ");
            lib->increaseStock(bc, n);
            break;
        }
        case 4: {
            string bc = readString("要减少数量的资源条码: ");
            int n = readInt("减少册数: ");
            lib->decreaseStock(bc, n);
            break;
        }
        case 5: {
            string bc = readString("要删除的资源条码: ");
            lib->removeResource(bc);
            break;
        }
        case 6: {
            string bc = readString("要查询的资源条码: ");
            Resource* res = lib->findResource(bc);
            if (res == nullptr) {
                cout << "查询失败：未找到条码为 " << bc << " 的资源。" << endl;
            }
            else {
                // 这里用到 Resource 重载的 operator<<
                cout << "===== 资源详情 =====" << endl;
                cout << *res << endl;
            }
            break;
        }
        case 7:
            lib->sortResourcesByPrice();
            lib->showAllResources();
            break;

        case 8:
            addReaderUI(*lib);
            break;

        case 9:
            lib->showAllReaders();
            break;

        case 10: {
            string id = readString("借阅人编号(学号/工号): ");
            string bc = readString("资源条码: ");
            lib->borrowResource(id, bc);
            break;
        }
        case 11: {
            string id = readString("借阅人编号(学号/工号): ");
            string bc = readString("资源条码: ");
            lib->returnResource(id, bc);
            break;
        }
        case 12:
            lib->showOverdueList();
            break;

        case 13:
            lib->showAllRecords();
            break;

        case 14:
            lib->showInfo();
            break;

        case 15: {
            int days = readInt("模拟时间推进多少天: ");
            TimeUtil::advance(days);
            cout << "模拟时间已推进 " << days << " 天（累计 "
                << TimeUtil::getOffsetDays() << " 天），当前日期："
                << TimeUtil::toString(TimeUtil::now()) << endl;
            break;
        }
        default:
            cout << "无效的选择，请输入 0 ~ 15 之间的数字。" << endl;
            break;
        }
    }

    // 释放图书馆对象；其析构函数会 delete 掉所有资源和借阅人对象
    delete lib;
    lib = nullptr;

    return 0;
}
