// ============================================================
// 图书馆借阅管理系统 —— 主程序
//
// 题目来源：《面向对象程序设计（C++）实验指导书》
//           基础题目三：图书馆借阅管理系统
//
// 本版本覆盖的知识点（实验一 ~ 实验五，逐次递增，前面的要求全部保留）：
//   实验一  类与对象：类的设计、访问控制、构造/重载构造、数据合法性校验、
//                     单一职责、资源管理类的三大函数
//   实验二  组合与依赖：Library 组合资源容器与借阅人容器；
//                       Reader 组合 vector<BorrowRecord>；
//                       Reader::borrowResource(Resource&) 依赖关系
//   实验三  继承与派生：Resource -> Book / Magazine / EBook
//                       Reader   -> Student / Teacher
//   实验四  多态与虚函数：纯虚函数、抽象类、虚析构、
//                       基类指针容器 + 虚函数调用、操作符重载
//   实验五  模板与 vector：PtrCollection<T> 类模板（内部 std::vector<T*>）、
//                       push_back / emplace_back / size / capacity /
//                       begin / end / erase / clear，全面取代定长数组
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
#include "VectorDemo.h"

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <stdexcept>

using namespace std;

// ---------------- 输入工具：保证输入非法时不会死循环 ----------------

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 函数模板：类型不同、逻辑相同的一类函数只写一次（实验五：理解模板的概念）
// readValue<int> / readValue<double> / readValue<string> 由编译器按需生成
template <class T>
T readValue(const string& prompt) {
    T v{};
    while (true) {
        cout << prompt;
        if (cin >> v) {
            clearInput();
            return v;
        }
        // 输入流已结束（Ctrl+Z / 重定向文件读完）时不能死循环，
        // 抛异常交给 main 统一处理，保证图书馆对象仍能被正常析构
        if (cin.eof()) throw runtime_error("检测到输入结束");
        cout << "  输入无效，请重新输入。" << endl;
        clearInput();
    }
}

// 三个"薄壳"函数，只是让调用处更可读，内部共用同一个函数模板
int    readInt(const string& prompt) { return readValue<int>(prompt); }
double readDouble(const string& prompt) { return readValue<double>(prompt); }
string readString(const string& prompt) { return readValue<string>(prompt); }

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
    cout << " ---------- 实验五：模板与 vector 容器 ----------" << endl;
    cout << " 16. 演示 vector 放对象：增 / 删 / 清空 / 容量变化" << endl;
    cout << " 17. 演示基类指针 vector：一个容器装多种资源（多态）" << endl;
    cout << " 18. 按资源类型统计（迭代器 + 虚函数）" << endl;
    cout << " 19. 清理某借阅人已归还的记录（vector::erase 区间删除）" << endl;
    cout << " 20. 注销借阅人（vector::erase 按位置删除）" << endl;
    cout << " 21. 清空全部数据（vector::clear 回收内存）" << endl;
    cout << "  0. 退出系统" << endl;
    cout << "========================================================" << endl;
}

// ---------------- 各功能实现 ----------------

// 2. 添加新资源
void addResourceUI(Library& lib) {
    cout << "\n请选择资源类型：" << endl;
    cout << "  1. 普通图书   2. 杂志   3. 电子资源" << endl;
    int type = readInt("类型编号: ");

    string bc = readString("条码号: ");
    string title = readString("名称: ");
    string pub = readString("出版社/出品方: ");
    double price = readDouble("价格(元): ");
    int    count = readInt("本次入库数量(册): ");
    if (count <= 0) {
        cout << "数量必须大于 0，添加失败。" << endl;
        return;
    }

    // 基类指针接收派生类对象 —— 多态的起点；
    // 对象由容器统一持有（PtrCollection 里 vector<T*> 负责保存与释放）
    Resource* res = nullptr;
    if (type == 1) {
        string isbn = readString("ISBN(13位数字): ");
        string author = readString("作者: ");
        int    pages = readInt("页数: ");
        // 普通创建方式：new 出对象，再交给容器（对应 vector::push_back）
        res = new Book(bc, isbn, title, author, pub, price, pages, count);
    }
    else if (type == 2) {
        string issue = readString("期号(如 2024年第5期): ");
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
            << "，借期 " << res->getLendDays() << " 天。"
            << "当前馆藏 " << lib.getResourceCount() << " 项（vector 容量 "
            << lib.getResourceCapacity() << "）。" << endl;
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

    string id = readString(type == 1 ? "学号: " : "工号: ");
    string name = readString("姓名: ");
    string dept = readString("院系: ");
    int    max = readInt("最大借阅数量: ");

    Reader* r = nullptr;
    if (type == 1) {
        string major = readString("专业: ");
        int    grade = readInt("年级(1-5): ");
        r = new Student(id, name, dept, max, major, grade);
    }
    else if (type == 2) {
        string title = readString("职称(教授/副教授/讲师): ");
        string college = readString("所属学院: ");
        r = new Teacher(id, name, dept, max, title, college);
    }
    else {
        cout << "未知的身份类型，添加失败。" << endl;
        return;
    }

    if (lib.addReader(r)) {
        cout << "添加成功！身份：" << r->getReaderType()
            << "，最大借阅 " << r->getMaxBorrow() << " 本。"
            << "当前借阅人 " << lib.getReaderCount() << " 人（vector 容量 "
            << lib.getReaderCapacity() << "）。" << endl;
    }
    else {
        delete r;
    }
}

// 预先载入的演示数据
void initDemoData(Library& lib) {
    cout << "\n正在载入演示数据..." << endl;

    // 资源一：常规写法 —— 先 new 出派生类对象，再由容器 push_back 接管
    lib.addResource(new Book("B001", "9787115585386", "C++ Primer",
                             "Stanley B.Lippman", "人民邮电出版社", 128.0, 838, 3));

    // 资源二：实验五写法 —— 容器模板 emplaceNew 直接在 vector 尾部构造对象，
    // 参数原样转发给 Magazine 的构造函数，不产生临时对象
    lib.createResource<Magazine>("M001", "读者", "读者出版传媒", 10.0,
                                 "2024年第5期", "编辑部", 5);

    // 资源三：电子资源（演示虚函数重写：不限份数、借期 3 天）
    lib.createResource<EBook>("E001", "算法导论 电子版", "机械工业出版社", 50.0,
                              20.5, "http://example.com/algo.pdf");

    // 两种派生类借阅人，同样交给基类指针容器统一管理
    lib.addReader(new Student("5120256566", "江彦辉", "计算机科学与技术学院", 5,
                              "计算机科学与技术", 2));
    lib.createReader<Teacher>("T001", "刘畅", "计算机科学与技术学院", 10,
                              "副教授", "计算机科学与技术学院");

    cout << "演示数据载入完成：" << lib.getResourceCount() << " 项资源、"
        << lib.getReaderCount() << " 位借阅人。" << endl;
}

// ---------------- 菜单主循环 ----------------
// 单独抽成一个函数：出问题时（比如输入流结束）异常能被 main 统一捕获，
// 保证图书馆对象最后仍然会被正常 delete。
void runMenu(Library& lib) {
    int choice = -1;
    while (true) {
        showMenu();
        choice = readInt("请输入你的选择(0-21): ");
        if (choice == 0) {
            cout << "感谢使用，系统正在退出..." << endl;
            break;
        }

        switch (choice) {
        case 1:
            lib.showAllResources();
            break;

        case 2:
            addResourceUI(lib);
            break;

        case 3: {
            string bc = readString("要增加数量的资源条码: ");
            int n = readInt("增加册数: ");
            lib.increaseStock(bc, n);
            break;
        }
        case 4: {
            string bc = readString("要减少数量的资源条码: ");
            int n = readInt("减少册数: ");
            lib.decreaseStock(bc, n);
            break;
        }
        case 5: {
            string bc = readString("要删除的资源条码: ");
            lib.removeResource(bc);
            break;
        }
        case 6: {
            string bc = readString("要查询的资源条码: ");
            Resource* res = lib.findResource(bc);
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
            lib.sortResourcesByPrice();
            lib.showAllResources();
            break;

        case 8:
            addReaderUI(lib);
            break;

        case 9:
            lib.showAllReaders();
            break;

        case 10: {
            string id = readString("借阅人编号(学号/工号): ");
            string bc = readString("资源条码: ");
            lib.borrowResource(id, bc);
            break;
        }
        case 11: {
            string id = readString("借阅人编号(学号/工号): ");
            string bc = readString("资源条码: ");
            lib.returnResource(id, bc);
            break;
        }
        case 12:
            lib.showOverdueList();
            break;

        case 13:
            lib.showAllRecords();
            break;

        case 14:
            lib.showInfo();
            break;

        case 15: {
            int days = readInt("模拟时间推进多少天: ");
            TimeUtil::advance(days);
            cout << "模拟时间已推进 " << days << " 天（累计 "
                << TimeUtil::getOffsetDays() << " 天），当前日期："
                << TimeUtil::toString(TimeUtil::now()) << endl;
            break;
        }

        // ---------------- 实验五相关 ----------------
        case 16:
            VectorDemo::demoValueVector(lib);
            break;

        case 17:
            VectorDemo::demoPointerVector(lib);
            break;

        case 18:
            VectorDemo::showTypeStatistics(lib);
            break;

        case 19: {
            string id = readString("要清理记录的借阅人编号: ");
            Reader* r = lib.findReader(id);
            if (r == nullptr) {
                cout << "操作失败：未找到编号为 " << id << " 的借阅人。" << endl;
            }
            else {
                cout << "清理前：" << r->getRecordCount() << " 条记录，vector 容量 "
                    << r->getRecordCapacity() << endl;
                r->purgeReturned();
            }
            break;
        }
        case 20: {
            string id = readString("要注销的借阅人编号: ");
            lib.removeReader(id);
            break;
        }
        case 21: {
            int ok = readInt("确认清空全部馆藏与借阅人数据？(1=确认，其他=取消): ");
            if (ok == 1) {
                lib.clearAllData();
                lib.showInfo();
            }
            else {
                cout << "已取消。" << endl;
            }
            break;
        }
        default:
            cout << "无效的选择，请输入 0 ~ 21 之间的数字。" << endl;
            break;
        }
    }
}

int main() {
    // 图书馆对象用 new 创建在堆上（实验三：new / delete 的使用）
    Library* lib = new Library("西南科技大学逸夫图书馆");

    initDemoData(*lib);

    try {
        runMenu(*lib);
    }
    catch (const exception& e) {
        // 输入流结束等异常在这里统一收口，程序仍能优雅退出
        cout << "\n" << e.what() << "，系统退出。" << endl;
    }

    // 释放图书馆对象；其成员容器析构时会 delete 掉所有资源和借阅人对象
    delete lib;
    lib = nullptr;

    return 0;
}
