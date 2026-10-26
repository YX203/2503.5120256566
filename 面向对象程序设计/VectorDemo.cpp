#include "VectorDemo.h"
#include "Library.h"
#include "Book.h"
#include "Resource.h"

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

// ---------------- 模块内部的输入助手 ----------------
// （不依赖 main.cpp 里的输入函数，保证本模块可以单独编译、
//   也顺便说明：模块之间靠接口耦合，而不是靠全局函数）
namespace {

void dropLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int askInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) { dropLine(); return v; }
        if (cin.eof()) throw std::runtime_error("检测到输入结束");
        cout << "  输入无效，请输入一个整数。" << endl;
        dropLine();
    }
}

double askDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v) { dropLine(); return v; }
        if (cin.eof()) throw std::runtime_error("检测到输入结束");
        cout << "  输入无效，请输入一个数字。" << endl;
        dropLine();
    }
}

string askString(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s) {
            dropLine();
            if (!s.empty()) return s;
        }
        if (cin.eof()) throw std::runtime_error("检测到输入结束");
        cout << "  输入不能为空，请重新输入。" << endl;
        dropLine();
    }
}

}   // namespace

// ============================================================
// 1) vector 里放对象
// ============================================================
void VectorDemo::demoValueVector(const Library& lib) {
    cout << "\n========== 演示一：vector 里放【对象】（值语义）==========" << endl;
    cout << "说明：下面用 std::vector<Book> 模拟一个书架，Book 对象直接存在容器里，"
            "不需要 new/delete。" << endl;

    int n = askInt("要放入书架的图书数量(1-20): ");
    if (n < 1) n = 1;
    if (n > 20) n = 20;

    vector<Book> shelf;   // 元素类型是"对象"，不是指针
    cout << "\n初始状态：size = " << shelf.size()
        << "，capacity = " << shelf.capacity() << "（空容器一般从 0 开始）" << endl;

    for (int i = 1; i <= n; ++i) {
        cout << "\n--- 第 " << i << " 本 ---" << endl;
        string title = askString("书名: ");
        double price = askDouble("价格(元): ");

        // 条码与 ISBN 在这里只是占位，演示重点在容器的增删
        string barcode = "SHELF" + to_string(100 + i);
        string isbn = "9780000000001";

        if (i % 2 == 1) {
            // push_back：先构造一个临时 Book 对象，再拷贝/移动到容器尾部
            shelf.push_back(Book(barcode, isbn, title, "演示作者",
                                 "演示出版社", price, 300, 1));
            cout << "  用 push_back 放入（先产生临时对象，再拷贝进容器）" << endl;
        }
        else {
            // emplace_back：把构造参数直接转发到容器尾部，原地构造，少一次拷贝
            shelf.emplace_back(barcode, isbn, title, "演示作者",
                               "演示出版社", price, 300, 1);
            cout << "  用 emplace_back 放入（在容器尾部原地构造）" << endl;
        }
        cout << "  当前 size = " << shelf.size()
            << "，capacity = " << shelf.capacity() << endl;
    }

    // 迭代器遍历：begin() / end()
    cout << "\n===== 用迭代器遍历书架 =====" << endl;
    int k = 0;
    for (vector<Book>::iterator it = shelf.begin(); it != shelf.end(); ++it) {
        cout << "[" << setw(2) << (++k) << "] ";
        // 转成基类引用再输出，走 Resource 重载的 operator<<，内部调用虚函数 print
        cout << static_cast<const Resource&>(*it) << endl;
    }

    // 范围 for（本质也是迭代器，写法更简洁）
    double sum = 0.0;
    for (const Book& b : shelf) sum += b.getPrice();
    cout << "合计 " << shelf.size() << " 册，总价 " << fixed << setprecision(2)
        << sum << " 元" << endl;
    cout << defaultfloat << setprecision(6);   // 恢复默认浮点格式

    // erase：按迭代器删除一个元素
    if (!shelf.empty()) {
        int pos = askInt("要删除第几本(输入序号，0 表示不删除): ");
        if (pos >= 1 && pos <= static_cast<int>(shelf.size())) {
            // begin() + pos - 1 就是第 pos 个元素的迭代器位置
            shelf.erase(shelf.begin() + (pos - 1));
            cout << "已用 erase 删除第 " << pos << " 本，剩余 size = "
                << shelf.size() << "，capacity = " << shelf.capacity()
                << "（capacity 一般不会因为删除而变小）" << endl;
        }
        else {
            cout << "未删除任何元素。" << endl;
        }
    }

    // clear：清空
    cout << "\n最后用 clear 清空容器：";
    shelf.clear();
    cout << "size = " << shelf.size() << "，capacity = " << shelf.capacity()
        << "（元素已析构，但申请到的内存通常还在，便于复用）" << endl;

    cout << "\n对比：本图书馆的馆藏用的是【对象指针】容器，当前有 "
        << lib.getResourceCount() << " 项资源、vector 容量 "
        << lib.getResourceCapacity() << " 个指针槽位。" << endl;
    cout << "为什么馆藏不能也写成 vector<Resource>？因为 Resource 是抽象类，"
            "不能实例化；而且按值存放派生类对象会被\"切片\"，多态就失效了。" << endl;
}

// ============================================================
// 2) vector 里放对象指针（基类指针）
// ============================================================
void VectorDemo::demoPointerVector(const Library& lib) {
    cout << "\n========== 演示二：vector 里放【基类指针】（多态）==========" << endl;

    // 从馆藏里把基类指针收集到一个临时 vector，看看"一个容器装多种派生类"
    vector<Resource*> brief;
    lib.forEachResource([&brief](Resource* r) { brief.push_back(r); });

    cout << "从馆藏收集到的资源指针：" << brief.size() << " 个" << endl;
    if (brief.empty()) {
        cout << "  （馆藏为空，先添加几项资源再看效果）" << endl;
        return;
    }

    // 迭代器遍历，通过基类指针调用纯虚函数 getTypeName()
    cout << "\n迭代器遍历，按实际类型输出（虚函数生效）：" << endl;
    for (vector<Resource*>::const_iterator it = brief.begin(); it != brief.end(); ++it) {
        cout << "  - 实际类型：" << (*it)->getTypeName()
            << " | 名称：" << (*it)->getTitle()
            << " | 借期：" << (*it)->getLendDays() << " 天"
            << " | 日罚金：" << (*it)->getFinePerDay() << " 元" << endl;
    }

    cout << "\n顺序统计（不用下标，只用迭代器）：" << endl;
    size_t totalCopies = 0;
    for (vector<Resource*>::const_iterator it = brief.begin(); it != brief.end(); ++it) {
        totalCopies += static_cast<size_t>((*it)->getTotalCount());
    }
    cout << "  馆藏总册数：" << totalCopies << " 册" << endl;

    cout << "\n【思考题二小结】基类与派生类可以放进同一个 vector，条件是：\n"
            "  元素类型写成基类指针（或基类智能指针），例如 vector<Resource*>；\n"
            "  遍历时通过基类指针调用虚函数，各派生类的行为自然不同；\n"
            "  销毁时需逐个 delete（本项目统一交给 PtrCollection::clearAll 负责），\n"
            "  且基类必须有虚析构函数，否则派生类那部分内存不会被释放。" << endl;
}

// ============================================================
// 3) 迭代器 + 虚函数做统计
// ============================================================
void VectorDemo::showTypeStatistics(const Library& lib) {
    cout << "\n========== 按资源类型统计（迭代器 + 虚函数）==========" << endl;

    int bookKinds = 0, magKinds = 0, ebookKinds = 0;
    int bookCopies = 0, magCopies = 0, ebookCopies = 0;
    double totalValue = 0.0;
    int availableCopies = 0;

    // forEach 内部就是 vector 迭代器遍历；这里用 lambda 累加
    lib.forEachResource([&](Resource* r) {
        const string type = r->getTypeName();   // 虚函数：按实际类型返回
        if (type == "普通图书") {
            ++bookKinds;
            bookCopies += r->getTotalCount();
        }
        else if (type == "杂志") {
            ++magKinds;
            magCopies += r->getTotalCount();
        }
        else if (type == "电子资源") {
            ++ebookKinds;
            ebookCopies += r->getTotalCount();
        }
        totalValue += r->getPrice() * r->getTotalCount();
        availableCopies += r->getStock();
    });

    if (lib.getResourceCount() == 0) {
        cout << "  （馆藏为空，先添加资源再统计）" << endl;
        return;
    }

    // 注意：中文占多个字节，用 setw 对齐会失准，这里直接分行输出
    cout << "  普通图书：" << bookKinds << " 种，" << bookCopies << " 册" << endl;
    cout << "  杂志    ：" << magKinds << " 种，" << magCopies << " 册" << endl;
    cout << "  电子资源：" << ebookKinds << " 种，" << ebookCopies << " 册" << endl;
    cout << "  ------------------------------------------" << endl;
    cout << "馆藏种类合计：" << lib.getResourceCount() << " 项"
        << "，册数合计：" << (bookCopies + magCopies + ebookCopies) << " 册"
        << "（当前可借 " << availableCopies << " 册）" << endl;
    cout << "馆藏总价值：" << fixed << setprecision(2) << totalValue << " 元" << endl;
    cout << defaultfloat << setprecision(6);   // 恢复默认浮点格式

    cout << "容器容量对照：资源 vector size = " << lib.getResourceCount()
        << "，capacity = " << lib.getResourceCapacity()
        << "；借阅人 vector size = " << lib.getReaderCount()
        << "，capacity = " << lib.getReaderCapacity() << endl;
}
