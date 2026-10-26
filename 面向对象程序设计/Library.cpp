#include "Library.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// 默认构造函数：容器自动初始化为空（不需要再手工把数组元素置空）
Library::Library() : name("西南科技大学图书馆") {
}

// 重载构造函数：体现构造函数对成员的初始化
Library::Library(const string& libName) : name(libName) {
    if (name.empty()) name = "未命名图书馆";
}

// 析构函数：这里不需要写 delete 循环了。
// resources / readers 是本类的对象成员，会在本类析构后被自动析构，
// PtrCollection<T>::~PtrCollection() 里统一 delete 所有堆对象。
// 因为基类都是虚析构，delete 基类指针时会正确调用派生类析构函数。
Library::~Library() {
}

// ==================== 资源管理 ====================

bool Library::addResource(Resource* res) {
    if (res == nullptr) return false;
    // 条码唯一性校验（用基类接口 getBarcode，无需知道具体是哪类资源）
    if (findResource(res->getBarcode()) != nullptr) {
        cout << "添加失败：条码 " << res->getBarcode() << " 已存在。" << endl;
        return false;
    }
    resources.add(res);   // vector::push_back，容量按需增长，不再有"已达上限"
    return true;
}

bool Library::removeResource(const string& barcode) {
    // 用容器模板的按条件查找找到下标，再用 removeAt 走 vector::erase
    int idx = resources.findIndexIf(
        [&barcode](Resource* r) { return r->getBarcode() == barcode; });
    if (idx == -1) {
        cout << "删除失败：未找到条码为 " << barcode << " 的资源。" << endl;
        return false;
    }
    Resource* target = resources[idx];
    // 有副本在外借时不允许删除
    if (target->getStock() != target->getTotalCount()) {
        cout << "删除失败：《" << target->getTitle()
            << "》尚有副本在外借，请先归还。" << endl;
        return false;
    }
    cout << "删除成功：" << target->getTitle() << " 已从馆内移除。" << endl;
    resources.removeAt(static_cast<size_t>(idx));   // delete + vector::erase
    return true;
}

Resource* Library::findResource(const string& barcode) {
    int idx = resources.findIndexIf(
        [&barcode](Resource* r) { return r->getBarcode() == barcode; });
    return idx == -1 ? nullptr : resources[idx];
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

// 遍历基类指针容器 —— 实验四"多态"最典型的用法，实验五用 vector 迭代器实现
void Library::showAllResources() const {
    cout << "===== 【" << name << "】馆藏资源（共 " << resources.size()
        << " 项，vector 容量 " << resources.capacity() << "）=====" << endl;
    if (resources.empty()) {
        cout << "  （馆藏为空）" << endl;
        return;
    }
    int i = 0;
    // forEach 内部用 vector 迭代器遍历；lambda 里通过基类指针调用虚函数 show()
    resources.forEach([&i](Resource* r) {
        cout << "[" << setw(2) << (++i) << "] ";
        r->show();
    });
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

// 按价格从小到大排序：用 Resource 重载的 operator<，配合 vector 迭代器区间排序
// （实验四的操作符重载 + 实验五的容器迭代器，两个知识点在这里合流）
void Library::sortResourcesByPrice() {
    if (resources.size() < 2) {
        cout << "馆藏不足 2 项，无需排序。" << endl;
        return;
    }
    resources.sortBy([](Resource* a, Resource* b) { return *a < *b; });
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
    if (findReader(r->getId()) != nullptr) {
        cout << "添加失败：编号 " << r->getId() << " 已存在。" << endl;
        return false;
    }
    readers.add(r);   // vector::push_back
    return true;
}

// 实验五：删除借阅人 —— vector::erase 的另一个使用场景
bool Library::removeReader(const string& id) {
    int idx = readers.findIndexIf([&id](Reader* r) { return r->getId() == id; });
    if (idx == -1) {
        cout << "删除失败：未找到编号为 " << id << " 的借阅人。" << endl;
        return false;
    }
    Reader* target = readers[idx];
    if (target->getBorrowedCount() > 0) {
        cout << "删除失败：" << target->getName()
            << " 还有 " << target->getBorrowedCount()
            << " 册资源未归还，请先归还。" << endl;
        return false;
    }
    cout << "删除成功：借阅人 " << target->getName()
        << "（" << target->getId() << "）已注销。" << endl;
    readers.removeAt(static_cast<size_t>(idx));
    return true;
}

Reader* Library::findReader(const string& id) {
    int idx = readers.findIndexIf([&id](Reader* r) { return r->getId() == id; });
    return idx == -1 ? nullptr : readers[idx];
}

void Library::showAllReaders() const {
    cout << "===== 【" << name << "】注册借阅人（共 " << readers.size()
        << " 人，vector 容量 " << readers.capacity() << "）=====" << endl;
    if (readers.empty()) {
        cout << "  （暂无借阅人）" << endl;
        return;
    }
    int i = 0;
    readers.forEach([&i](Reader* r) {
        cout << "[" << setw(2) << (++i) << "] ";
        r->show();   // 同样是基类指针调用虚函数
    });
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
    readers.forEach([&any](Reader* r) {
        if (r->getOverdueCount() > 0) {
            any = true;
            cout << "[" << r->getReaderType() << "] "
                << r->getName() << "（" << r->getId() << "）"
                << " 逾期 " << r->getOverdueCount() << " 册"
                << "，应付罚金 " << fixed << setprecision(2)
                << r->getUnpaidFine() << " 元" << endl;
            r->showRecords();
            cout << defaultfloat << setprecision(6);   // 恢复默认浮点格式
        }
    });
    if (!any) cout << "  当前没有任何逾期记录。" << endl;
}

void Library::showAllRecords() {
    cout << "===== 全部借阅记录 =====" << endl;
    if (readers.empty()) {
        cout << "  （暂无借阅人）" << endl;
        return;
    }
    readers.forEach([](Reader* r) { r->showRecords(); });
}

void Library::showInfo() const {
    int totalCopies = 0, availableCopies = 0, borrowed = 0;
    resources.forEach([&](Resource* r) {
        totalCopies += r->getTotalCount();
        availableCopies += r->getStock();
    });
    // 外借册数按借阅人的未还记录统计，电子资源等不限份数的资源也计入
    readers.forEach([&borrowed](Reader* r) { borrowed += r->getBorrowedCount(); });

    cout << "===== 图书馆信息 =====" << endl;
    cout << "名称：" << name << endl;
    cout << "馆藏种类：" << resources.size() << " 项" << endl;
    cout << "馆藏总册数：" << totalCopies << " 册（可借 " << availableCopies << " 册）" << endl;
    cout << "注册借阅人：" << readers.size() << " 人" << endl;
    cout << "当前外借：" << borrowed << " 册（次）" << endl;
    cout << "容器容量：资源 vector " << resources.capacity()
        << " 个指针槽位，借阅人 vector " << readers.capacity() << " 个指针槽位" << endl;
}

void Library::clearAllData() {
    resources.clearAll();   // delete 全部资源 + vector::clear
    readers.clearAll();     // delete 全部借阅人 + vector::clear
    cout << "已清空全部馆藏与借阅人数据（vector::clear 已回收内存）。" << endl;
}

string Library::getName() const { return name; }
int    Library::getResourceCount() const { return static_cast<int>(resources.size()); }
int    Library::getReaderCount() const { return static_cast<int>(readers.size()); }
int    Library::getResourceCapacity() const { return static_cast<int>(resources.capacity()); }
int    Library::getReaderCapacity() const { return static_cast<int>(readers.capacity()); }
