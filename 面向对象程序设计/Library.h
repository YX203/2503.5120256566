#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Resource.h"
#include "Reader.h"

const int MAX_RESOURCES = 100;   // 馆藏资源上限
const int MAX_READERS   = 100;   // 注册借阅人上限

// ============================================================
// Library：图书馆类 —— 组合类（实验二）
//   成员变量是"另一个类的对象指针数组"：
//       Resource* resources[]  —— 存放 Book / Magazine / EBook
//       Reader*   readers[]    —— 存放 Student / Teacher
//   一个图书馆"拥有"若干资源与若干借阅人，是整体—部分（has-a）关系。
//
//   实验三：用 new 创建派生类对象，用 delete 释放（析构函数统一回收）。
//   实验四：两个数组成员都声明为基类指针，通过基类指针调用虚函数，
//           不同派生类对象表现出不同行为 —— 多态的核心用法。
// ============================================================
class Library {
private:
    std::string name;
    Resource*   resources[MAX_RESOURCES];
    int         resourceCount;
    Reader*     readers[MAX_READERS];
    int         readerCount;

public:
    Library();
    Library(const std::string& libName);
    ~Library();   // 释放所有 new 出来的资源与借阅人对象

    // ---------------- 资源管理（实验三：增 / 删 / 增数量 / 减数量 / 查询） ----------------
    bool      addResource(Resource* res);                 // 增加新资源
    bool      removeResource(const std::string& barcode); // 删除资源
    Resource* findResource(const std::string& barcode);   // 按条码查找
    bool      increaseStock(const std::string& barcode, int n);  // 增加已有资源数量
    bool      decreaseStock(const std::string& barcode, int n);  // 减少已有资源数量

    void showAllResources() const;                        // 显示全部（多态调用）
    void showResourceDetail(const std::string& barcode);  // 显示某一条资源详情
    void sortResourcesByPrice();                          // 用 operator< 排序
    bool isSameResource(const std::string& bc1, const std::string& bc2); // 用 operator==

    // ---------------- 借阅人管理 ----------------
    bool    addReader(Reader* r);
    Reader* findReader(const std::string& id);
    void    showAllReaders() const;

    // ---------------- 借还业务（实验二：依赖关系） ----------------
    bool borrowResource(const std::string& readerId, const std::string& barcode);
    bool returnResource(const std::string& readerId, const std::string& barcode);

    // ---------------- 查询与统计 ----------------
    void showOverdueList();        // 显示当前全部逾期情况
    void showAllRecords();         // 显示所有借阅人的记录
    void showInfo() const;

    std::string getName() const;
    int         getResourceCount() const;
    int         getReaderCount() const;
};

#endif
