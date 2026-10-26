#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <cstddef>
#include "Collection.h"
#include "Resource.h"
#include "Reader.h"

// ============================================================
// Library：图书馆类 —— 组合类（实验二）
//
//   一个图书馆"拥有"若干资源与若干借阅人，是整体—部分（has-a）关系。
//
//   实验二的组合关系（本版本没有被淡化，只是"部分"换成了容器）：
//       以前：Resource* resources[100];  Reader* readers[100];
//       现在：PtrCollection<Resource> resources;  PtrCollection<Reader> readers;
//       仍然是"图书馆包含资源对象和借阅人对象"，仍然是对象成员，
//       组合关系的判定标准（has-a / 生命周期由整体决定）完全成立。
//
//   实验三：用 new 创建派生类对象，用 delete 释放；
//           本类的析构函数不再手工 delete，而是由成员容器
//           PtrCollection 的析构函数统一回收 —— 单一职责。
//
//   实验四：两个容器的元素类型都是基类指针（Resource* / Reader*），
//           遍历时通过基类指针调用虚函数，不同派生类对象表现不同行为。
//
//   实验五：容器改用 vector（PtrCollection 内部的 std::vector<T*>），
//           增加对象 push_back / emplace_back，删除对象 erase，
//           清空对象 clear，容量不再写死为 100。
// ============================================================
class Library {
private:
    std::string             name;
    PtrCollection<Resource> resources;   // 存放 Book / Magazine / EBook
    PtrCollection<Reader>   readers;     // 存放 Student / Teacher

public:
    Library();
    Library(const std::string& libName);

    // 持有堆对象所有权的类不允许拷贝（实验一：资源管理类要管好三大函数）
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    ~Library();   // 成员容器析构时会自动 delete 所有资源与借阅人对象

    // ---------------- 资源管理（实验三：增 / 删 / 增数量 / 减数量 / 查询） ----------------
    bool      addResource(Resource* res);                 // 增加新资源（接管所有权）
    bool      removeResource(const std::string& barcode); // 删除资源（vector::erase）
    Resource* findResource(const std::string& barcode);   // 按条码查找
    bool      increaseStock(const std::string& barcode, int n);  // 增加已有资源数量
    bool      decreaseStock(const std::string& barcode, int n);  // 减少已有资源数量

    // 实验五：在容器尾部直接构造派生类对象（模板 + vector::emplace_back）
    // 用法：lib.createResource<Book>("B002","9787...","书名","作者","出版社",59.8,320,2);
    template <class Derived, class... Args>
    Derived* createResource(Args&&... args) {
        return resources.emplaceNew<Derived>(std::forward<Args>(args)...);
    }

    void showAllResources() const;                        // 显示全部（多态调用）
    void showResourceDetail(const std::string& barcode);  // 显示某一条资源详情
    void sortResourcesByPrice();                          // 用 operator< + 迭代器排序
    bool isSameResource(const std::string& bc1, const std::string& bc2); // 用 operator==

    // ---------------- 借阅人管理 ----------------
    bool    addReader(Reader* r);
    bool    removeReader(const std::string& id);          // 实验五：vector::erase 删借阅人
    Reader* findReader(const std::string& id);
    void    showAllReaders() const;

    // 实验五：借用模板在容器尾部构造借阅人对象
    template <class Derived, class... Args>
    Derived* createReader(Args&&... args) {
        return readers.emplaceNew<Derived>(std::forward<Args>(args)...);
    }

    // ---------------- 借还业务（实验二：依赖关系） ----------------
    bool borrowResource(const std::string& readerId, const std::string& barcode);
    bool returnResource(const std::string& readerId, const std::string& barcode);

    // ---------------- 查询与统计 ----------------
    void showOverdueList();        // 显示当前全部逾期情况
    void showAllRecords();         // 显示所有借阅人的记录
    void showInfo() const;

    // ---------------- 实验五：遍历容器（把"怎么用元素"交给调用方） ----------------
    template <class Func>
    void forEachResource(Func fn) const { resources.forEach(fn); }

    template <class Func>
    void forEachReader(Func fn) const { readers.forEach(fn); }

    // 清空整馆数据（vector::clear 演示）
    void clearAllData();

    std::string getName() const;
    int         getResourceCount() const;
    int         getReaderCount() const;
    int         getResourceCapacity() const;   // 供演示 vector::capacity
    int         getReaderCapacity() const;
};

#endif
