// ============================================================
// 图书馆借阅管理系统 —— 主程序
//
// 题目来源：《面向对象程序设计（C++）实验指导书》
//           基础题目三：图书馆借阅管理系统
//
// 本版本覆盖的知识点（实验一 ~ 实验六，逐次递增，前面的要求全部保留）：
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
//   实验六  输入输出流：Storage 模块用 fstream（ofstream / ifstream）
//                       把对象的成员数据存成文本文件，重启后可以读回来；
//                       存盘走虚函数 saveTo()，读盘按类型标记 new 对应派生类
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
#include "Storage.h"

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

// 字符串单独用 getline 读一整行：书名、姓名、出版社里常有空格
// （比如 "C++ Primer"），用 cin >> 只会读到空格前那一段，
// 剩下的字符会串到下一次输入里去。
string readString(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) throw runtime_error("检测到输入结束");
    if (!line.empty() && line.back() == '\r') line.pop_back();   // 兼容 CRLF 文本
    return line;
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
    cout << " 16. 演示 vector 放对象：增 / 删 / 清空 / 容量变化" << endl;
    cout << " 17. 演示基类指针 vector：一个容器装多种资源（多态）" << endl;
    cout << " 18. 按资源类型统计（迭代器 + 虚函数）" << endl;
    cout << " 19. 清理某借阅人已归还的记录（vector::erase 区间删除）" << endl;
    cout << " 20. 注销借阅人（vector::erase 按位置删除）" << endl;
    cout << " 21. 清空全部数据（vector::clear 回收内存）" << endl;
    cout << " 22. 保存全部数据到文件（ofstream 写 resources/readers/records.txt）" << endl;
    cout << " 23. 从文件读取全部数据（ifstream 逐行还原对象）" << endl;
    cout << " 24. 导出可读报表 report.txt（ofstream）" << endl;
    cout << " 25. 查看数据文件内容（ifstream 读回屏幕）" << endl;
    cout << " 26. 修改资源信息（名称/出版社/价格/数量/类型专属字段）" << endl;
    cout << " 27. 修改借阅人信息（姓名/院系/上限/身份专属字段）" << endl;
    cout << " 28. 查询某一借阅人（详细信息 + 借阅记录）" << endl;
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

    // 合法性校验（实验一）：超出合理范围时按身份默认上限处理，并说明原因
    if (max < 1) {
        max = (type == 1 ? 5 : 10);
        cout << "  输入不合法，已按" << (type == 1 ? "学生" : "教师")
            << "默认上限 " << max << " 本处理。" << endl;
    }
    else if (max > MAX_BORROW_LIMIT) {
        max = MAX_BORROW_LIMIT;
        cout << "  超过系统上限，已按 " << MAX_BORROW_LIMIT << " 本处理。" << endl;
    }

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

// ---------------- 补充功能：修改资源 / 修改借阅人 / 查询借阅人 ----------------
// 对应指导书"项目答辩（设计报告2）· 题目3"的需求：
//   添加借阅人信息、修改借阅人信息、添加图书或资源信息、修改图书或资源信息、
//   查看图书信息、查询以上信息。
//
// 这两个"修改"同时用到实验四的知识点：手上只有基类指针 Resource* / Reader*，
// 却要改派生类自己的字段（ISBN、专业、职称……），于是用 dynamic_cast 向下转型；
// 转型不成功会返回 nullptr，正好可以安全地判断"它到底是哪一类"。

// 修改资源信息
void modifyResourceUI(Library& lib) {
    string bc = readString("要修改的资源条码（条码是主键，不可修改）: ");
    Resource* res = lib.findResource(bc);
    if (res == nullptr) {
        cout << "修改失败：未找到条码为 " << bc << " 的资源。" << endl;
        return;
    }

    cout << "\n修改前的信息：" << endl;
    cout << *res << endl;

    cout << "\n可修改的项目：" << endl;
    cout << "  1. 名称" << endl;
    cout << "  2. 出版社 / 出品方" << endl;
    cout << "  3. 价格" << endl;
    cout << "  4. 馆藏总数量（含外借中的副本）" << endl;
    cout << "  5. 该类型资源的专属信息（ISBN / 作者 / 期号 / 大小 等）" << endl;
    cout << "  6. 以上全部修改" << endl;
    cout << "  0. 取消" << endl;
    int sel = readInt("请选择: ");
    if (sel == 0) {
        cout << "已取消。" << endl;
        return;
    }
    if (sel < 1 || sel > 6) {
        cout << "无效的选择，修改取消。" << endl;
        return;
    }

    bool changed = false;

    // 基类字段：直接用基类的 set 函数，不需要知道是哪一类资源
    if (sel == 1 || sel == 6) {
        if (res->setTitle(readString("新名称: "))) changed = true;
    }
    if (sel == 2 || sel == 6) {
        if (res->setPublisher(readString("新出版社/出品方: "))) changed = true;
    }
    if (sel == 3 || sel == 6) {
        if (res->setPrice(readDouble("新价格(元): "))) changed = true;
    }
    if (sel == 4 || sel == 6) {
        // 改的是"馆藏总数量"：算出差值后走 addStock / reduceStock，
        // 这样 stock 与 totalCount 始终同步，也不会把外借中的副本减掉。
        int newTotal = readInt("新的馆藏总数量(册): ");
        if (newTotal < 1) {
            cout << "数量修改失败：馆藏总数量至少为 1 册。" << endl;
        }
        else {
            int  delta = newTotal - res->getTotalCount();
            bool ok = true;
            if (delta > 0)      ok = res->addStock(delta);
            else if (delta < 0) ok = res->reduceStock(-delta);
            if (ok && delta != 0) changed = true;
        }
    }
    if (sel == 5 || sel == 6) {
        // 派生类字段：必须先向下转型，转型失败说明类型不匹配
        if (Book* b = dynamic_cast<Book*>(res)) {
            if (b->setAuthor(readString("新作者: ")))        changed = true;
            if (b->setPages(readInt("新页数: ")))            changed = true;
            if (b->setIsbn(readString("新ISBN(13位数字): "))) changed = true;
        }
        else if (Magazine* m = dynamic_cast<Magazine*>(res)) {
            if (m->setIssue(readString("新期号: ")))  changed = true;
            if (m->setEditor(readString("新主编: "))) changed = true;
        }
        else if (EBook* e = dynamic_cast<EBook*>(res)) {
            if (e->setFileSize(readDouble("新文件大小(MB): ")))    changed = true;
            if (e->setDownloadLink(readString("新下载链接: ")))   changed = true;
        }
    }

    if (changed) {
        cout << "\n修改完成，当前信息：" << endl;
        cout << *res << endl;
    }
    else {
        cout << "\n没有任何字段被修改（输入值不合法，或改成了与原来相同的值）。" << endl;
    }
}

// 修改借阅人信息
void modifyReaderUI(Library& lib) {
    string id = readString("要修改的借阅人编号（学号/工号是主键，不可修改）: ");
    Reader* r = lib.findReader(id);
    if (r == nullptr) {
        cout << "修改失败：未找到编号为 " << id << " 的借阅人。" << endl;
        return;
    }

    cout << "\n修改前的信息：" << endl;
    cout << *r << endl;

    cout << "\n可修改的项目：" << endl;
    cout << "  1. 姓名" << endl;
    cout << "  2. 院系" << endl;
    cout << "  3. 最大借阅数量" << endl;
    cout << "  4. 身份专属信息（学生：专业/年级；教师：职称/学院）" << endl;
    cout << "  5. 以上全部修改" << endl;
    cout << "  0. 取消" << endl;
    int sel = readInt("请选择: ");
    if (sel == 0) {
        cout << "已取消。" << endl;
        return;
    }
    if (sel < 1 || sel > 5) {
        cout << "无效的选择，修改取消。" << endl;
        return;
    }

    bool changed = false;

    if (sel == 1 || sel == 5) {
        if (r->setName(readString("新姓名: "))) changed = true;
    }
    if (sel == 2 || sel == 5) {
        if (r->setDepartment(readString("新院系: "))) changed = true;
    }
    if (sel == 3 || sel == 5) {
        if (r->setMaxBorrow(readInt("新的最大借阅数量: "))) changed = true;
    }
    if (sel == 4 || sel == 5) {
        if (Student* s = dynamic_cast<Student*>(r)) {
            if (s->setMajor(readString("新专业: ")))      changed = true;
            if (s->setGrade(readInt("新年级(1-5): ")))    changed = true;
        }
        else if (Teacher* t = dynamic_cast<Teacher*>(r)) {
            if (t->setTitle(readString("新职称: ")))          changed = true;
            if (t->setCollege(readString("新所属学院: ")))    changed = true;
        }
    }

    if (changed) {
        cout << "\n修改完成，当前信息：" << endl;
        cout << *r << endl;
        cout << "（退出系统或选择保存时会写回 readers.txt，下次启动仍然有效）" << endl;
    }
    else {
        cout << "\n没有任何字段被修改（输入值不合法，或改成了与原来相同的值）。" << endl;
    }
}

// 查询某一借阅人：先按编号找，找不到再按姓名找
void queryReaderUI(Library& lib) {
    string key = readString("请输入借阅人编号(学号/工号)或姓名: ");

    Reader* r = lib.findReader(key);
    if (r == nullptr) {
        // 编号没找到，退一步按姓名匹配（遍历容器，只取第一个命中的）
        lib.forEachReader([&key, &r](Reader* one) {
            if (r == nullptr && one->getName() == key) r = one;
        });
    }
    if (r == nullptr) {
        cout << "查询失败：没有编号或姓名为 " << key << " 的借阅人。" << endl;
        return;
    }

    cout << "\n===== 借阅人详情 =====" << endl;
    cout << *r << endl;
    cout << "  身份：" << r->getReaderType()
        << " | 可借上限：" << r->getMaxBorrow()
        << " 册 | 已借：" << r->getBorrowedCount()
        << " 册 | 还可借：" << (r->getMaxBorrow() - r->getBorrowedCount()) << " 册" << endl;
    cout << "  逾期：" << r->getOverdueCount() << " 册 | 未缴罚金："
        << fixed << setprecision(2) << r->getUnpaidFine()
        << " 元 | 历史累计罚金：" << r->getTotalFine() << " 元"
        << defaultfloat << setprecision(6) << endl;
    r->showRecords();
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
        choice = readInt("请输入你的选择(0-28): ");
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
                cout << "已清空全部馆藏与借阅人数据（vector::clear 已回收内存）。" << endl;
                lib.showInfo();
            }
            else {
                cout << "已取消。" << endl;
            }
            break;
        }

        // ---------------- 实验六相关：文件输入输出流 ----------------
        case 22:
            Storage::saveAll(lib);
            break;

        case 23:
            Storage::loadAll(lib);
            break;

        case 24:
            Storage::exportReport(lib);
            break;

        case 25:
            Storage::showDataFiles();
            break;

        // ---------------- 补充功能：修改与查询 ----------------
        case 26:
            modifyResourceUI(lib);
            break;

        case 27:
            modifyReaderUI(lib);
            break;

        case 28:
            queryReaderUI(lib);
            break;

        default:
            cout << "无效的选择，请输入 0 ~ 28 之间的数字。" << endl;
            break;
        }
    }
}

int main() {
    // 图书馆对象用 new 创建在堆上（实验三：new / delete 的使用）
    Library* lib = new Library("西南科技大学逸夫图书馆");

    // 实验六：启动时先用 ifstream 把上次存盘的数据读回来；
    // 没有数据文件（或文件打不开）才载入演示数据。
    if (!Storage::loadAll(*lib)) {
        initDemoData(*lib);
    }

    try {
        runMenu(*lib);
    }
    catch (const exception& e) {
        // 输入流结束等异常在这里统一收口，程序仍能优雅退出
        cout << "\n" << e.what() << "，系统退出。" << endl;
    }

    // 实验六：关闭应用程序时用 ofstream 把（可能已经修改过的）数据存回去
    Storage::saveAll(*lib);

    // 释放图书馆对象；其成员容器析构时会 delete 掉所有资源和借阅人对象
    delete lib;
    lib = nullptr;

    return 0;
}
