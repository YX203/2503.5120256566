#include "Library.h"
#include <iostream>
#include <iomanip>

using namespace std;

// 默认构造函数：数组指针全部置空（初值必须在构造时确定）
Library::Library() : name("西南科技大学图书馆"), resourceCount(0), readerCount(0) {
    for (int i = 0; i < MAX_RESOURCES; ++i) resources[i] = nullptr;
    for (int i = 0; i < MAX_READERS; ++i)   readers[i] = nullptr;
}

// 重载构造函数：体现构造函数对成员（含内嵌对象）的初始化
Library::Library(const string& libName)
    : name(libName), resourceCount(0), readerCount(0) {
    if (name.empty()) name = "未命名图书馆";
    for (int i = 0; i < MAX_RESOURCES; ++i) resources[i] = nullptr;
    for (int i = 0; i < MAX_READERS; ++i)   readers[i] = nullptr;
}

// 析构函数：回收所有 new 出来的对象
// 因为基类都是虚析构，delete 基类指针时会正确调用派生类析构函数
Library::~Library() {
    for (int i = 0; i < resourceCount; ++i) {
        delete resources[i];
        resources[i] = nullptr;
    }
    for (int i = 0; i < readerCount; ++i) {
        delete readers[i];
        readers[i] = nullptr;
    }
}

// ==================== 资源管理 ====================

bool Library::addResource(Resource* res) {
    if (res == nullptr) return false;
    if (resourceCount >= MAX_RESOURCES) {
        cout << "添加失败：馆藏资源已达上限(" << MAX_RESOURCES << "项)。" << endl;
        return false;
    }
    // 条码唯一性校验
    if (findResource(res->getBarcode()) != nullptr) {
        cout << "添加失败：条码 " << res->getBarcode() << " 已存在。" << endl;
        return false;
    }
    resources[resourceCount++] = res;
    return true;
}

bool Library::removeResource(const string& barcode) {
    int idx = -1;
    for (int i = 0; i < resourceCount; ++i) {
        if (resources[i]->getBarcode() == barcode) { idx = i; break; }
    }
    if (idx == -1) {
        cout << "删除失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    // 有副本在外借时不允许删除
    if (resources[idx]->getStock() != resources[idx]->getTotalCount()) {
        cout << "删除失败：《" << resources[idx]->getTitle()
            << "》尚有副本在外借，请先归还。" << endl;
        return false;
    }
    cout << "删除成功：" << resources[idx]->getTitle() << " 已从馆内移除。" << endl;
    delete resources[idx];      // 释放派生类对象
    for (int i = idx; i < resourceCount - 1; ++i) {
        resources[i] = resources[i + 1];
    }
    resources[--resourceCount] = nullptr;
    return true;
}

Resource* Library::findResource(const string& barcode) {
    for (int i = 0; i < resourceCount; ++i) {
        if (resources[i]->getBarcode() == barcode) return resources[i];
    }
    return nullptr;
}

bool Library::increaseStock(const string& barcode, int n) {
    Resource* r = findResource(barcode);
    if (!r) {
        cout << "增加失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    if (!r->addStock(n)) return false;
    cout << "增加成功：《" << r->getTitle() << "》现有 "
        << r->getStock() << "/" << r->getTotalCount() << " 本可借。" << endl;
    return true;
}

bool Library::decreaseStock(const string& barcode, int n) {
    Resource* r = findResource(barcode);
    if (!r) {
        cout << "减少失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    if (!r->reduceStock(n)) return false;
    cout << "减少成功：《" << r->getTitle() << "》现有 "
        << r->getStock() << "/" << r->getTotalCount() << " 本可借。" << endl;
    return true;
}

// 遍历基类指针数组 —— 实验四"多态"最典型的用法
void Library::showAllResources() const {
    cout << "===== 【" << name << "】馆藏资源（共 " << resourceCount << " 项）=====" << endl;
    if (resourceCount == 0) {
        cout << "  （馆藏为空）" << endl;
        return;
    }
    for (int i = 0; i < resourceCount; ++i) {
        cout << "[" << setw(2) << (i + 1) << "] ";
        // resources[i] 的静态类型是 Resource*，实际指向 Book / Magazine / EBook，
        // 这里通过基类指针调用虚函数 print，实际执行的是派生类的版本
        resources[i]->show();
    }
}

void Library::showResourceDetail(const string& barcode) {
    Resource* r = findResource(barcode);
    if (!r) {
        cout << "查询失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return;
    }
    cout << "===== 资源详情 =====" << endl;
    r->show();
}

// 按价格从小到大排序，这里用到了 Resource 重载的 operator<
void Library::sortResourcesByPrice() {
    for (int i = 0; i < resourceCount - 1; ++i) {
        for (int j = 0; j < resourceCount - 1 - i; ++j) {
            if (*resources[j + 1] < *resources[j]) {
                Resource* tmp = resources[j];
                resources[j] = resources[j + 1];
                resources[j + 1] = tmp;
            }
        }
    }
    cout << "已按价格从低到高排序。" << endl;
}

// 用 Resource 重载的 operator== 判断两条记录是否指向同一资源
bool Library::isSameResource(const string& bc1, const string& bc2) {
    Resource* a = findResource(bc1);
    Resource* b = findResource(bc2);
    if (!a || !b) {
        cout << "比较失败：条码不存在。" << endl;
        return false;
    }
    return (*a == *b);
}

// ==================== 借阅人管理 ====================

bool Library::addReader(Reader* r) {
    if (r == nullptr) return false;
    if (readerCount >= MAX_READERS) {
        cout << "添加失败：借阅人已达上限(" << MAX_READERS << "人)。" << endl;
        return false;
    }
    if (findReader(r->getId()) != nullptr) {
        cout << "添加失败：编号 " << r->getId() << " 已存在。" << endl;
        return false;
    }
    readers[readerCount++] = r;
    return true;
}

Reader* Library::findReader(const string& id) {
    for (int i = 0; i < readerCount; ++i) {
        if (readers[i]->getId() == id) return readers[i];
    }
    return nullptr;
}

void Library::showAllReaders() const {
    cout << "===== 【" << name << "】注册借阅人（共 " << readerCount << " 人）=====" << endl;
    if (readerCount == 0) {
        cout << "  （暂无借阅人）" << endl;
        return;
    }
    for (int i = 0; i < readerCount; ++i) {
        cout << "[" << setw(2) << (i + 1) << "] ";
        readers[i]->show();   // 同样是基类指针调用虚函数
    }
}

// ==================== 借还业务 ====================

bool Library::borrowResource(const string& readerId, const string& barcode) {
    Reader* r = findReader(readerId);
    if (!r) {
        cout << "借阅失败：未找到编号为 " << readerId << " 的借阅人。" << endl;
        return false;
    }
    Resource* res = findResource(barcode);
    if (!res) {
        cout << "借阅失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    // 依赖关系：把资源对象按引用交给借阅人处理，双方状态会一起改变
    return r->borrowResource(*res);
}

bool Library::returnResource(const string& readerId, const string& barcode) {
    Reader* r = findReader(readerId);
    if (!r) {
        cout << "归还失败：未找到编号为 " << readerId << " 的借阅人。" << endl;
        return false;
    }
    Resource* res = findResource(barcode);
    if (!res) {
        cout << "归还失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    return r->returnResource(*res);
}

// ==================== 查询与统计 ====================

void Library::showOverdueList() {
    cout << "===== 逾期情况汇总 =====" << endl;
    bool any = false;
    for (int i = 0; i < readerCount; ++i) {
        if (readers[i]->getOverdueCount() > 0) {
            any = true;
            cout << "[" << readers[i]->getReaderType() << "] "
                << readers[i]->getName() << "（" << readers[i]->getId() << "）"
                << " 逾期 " << readers[i]->getOverdueCount() << " 册"
                << "，应付罚金 " << fixed << setprecision(2)
                << readers[i]->getUnpaidFine() << " 元" << endl;
            readers[i]->showRecords();
            cout.unsetf(ios::fixed);
        }
    }
    if (!any) cout << "  当前没有任何逾期记录。" << endl;
}

void Library::showAllRecords() {
    cout << "===== 全部借阅记录 =====" << endl;
    if (readerCount == 0) {
        cout << "  （暂无借阅人）" << endl;
        return;
    }
    for (int i = 0; i < readerCount; ++i) {
        readers[i]->showRecords();
    }
}

void Library::showInfo() const {
    int totalCopies = 0, availableCopies = 0, borrowed = 0;
    for (int i = 0; i < resourceCount; ++i) {
        totalCopies += resources[i]->getTotalCount();
        availableCopies += resources[i]->getStock();
    }
    // 外借册数按借阅人的未还记录统计，电子资源等不限份数的资源也计入
    for (int i = 0; i < readerCount; ++i) {
        borrowed += readers[i]->getBorrowedCount();
    }

    cout << "===== 图书馆信息 =====" << endl;
    cout << "名称：" << name << endl;
    cout << "馆藏种类：" << resourceCount << " 项" << endl;
    cout << "馆藏总册数：" << totalCopies << " 册（可借 " << availableCopies << " 册）" << endl;
    cout << "注册借阅人：" << readerCount << " 人" << endl;
    cout << "当前外借：" << borrowed << " 册（次）" << endl;
}

string Library::getName() const { return name; }
int    Library::getResourceCount() const { return resourceCount; }
int    Library::getReaderCount() const { return readerCount; }
