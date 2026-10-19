#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <iostream>

// ============================================================
// Resource：图书资源抽象基类
//
//   实验三（继承与派生）：作为基类，被 Book / Magazine / EBook 公有继承；
//                        派生类构造函数负责调用基类构造函数。
//   实验四（多态与虚函数）：含纯虚函数 => Resource 是抽象类，
//                        不能实例化对象，只能通过基类指针/引用
//                        调用虚函数，从而产生多态行为（统一接口）。
//   实验一（类与对象）：体现封装 —— 数据成员为 protected，
//                        对外只暴露带合法性校验的公有成员函数。
// ============================================================
class Resource {
protected:
    std::string barcode;     // 条码号（唯一标识）
    std::string title;       // 书名 / 资源名
    std::string publisher;   // 出版社 / 出品方
    double      price;       // 价格（元）
    int         stock;       // 当前可借数量
    int         totalCount;  // 馆藏总数量

public:
    Resource();
    Resource(const std::string& bc, const std::string& t,
             const std::string& p, double pr, int count = 1);

    // 虚析构函数：通过基类指针 delete 派生类对象时，
    // 保证派生类的析构函数也会被调用（实验四要点）
    virtual ~Resource() {}

    // ---------------- 纯虚函数（Resource 因此是抽象类） ----------------
    virtual std::string getTypeName() const = 0;   // 资源类型名称
    virtual int         getLendDays() const = 0;   // 可借天数
    virtual double      getFinePerDay() const = 0; // 逾期每天罚金（元）

    // ---------------- 虚函数（有默认实现，派生类可重写） ----------------
    virtual void print(std::ostream& os) const;    // 把信息输出到指定流
    virtual bool borrowOne();                      // 借出一本：可借数量 -1
    virtual bool returnOne();                      // 归还一本：可借数量 +1
    virtual std::string getStockDesc() const;      // 可借数量的显示文字（派生类可重写）

    void show() const;                             // 输出到屏幕（内部调用 print）

    // ---------------- 获取（get） ----------------
    std::string getBarcode() const;
    std::string getTitle() const;
    std::string getPublisher() const;
    double      getPrice() const;
    int         getStock() const;
    int         getTotalCount() const;
    bool        getAvailable() const;              // 是否还有可借副本

    // ---------------- 修改（set），全部带合法性校验 ----------------
    bool setBarcode(const std::string& bc);        // 条码不能为空
    bool setTitle(const std::string& t);           // 名称不能为空
    bool setPublisher(const std::string& p);       // 出版社不能为空
    bool setPrice(double pr);                      // 价格不能为负
    bool setStock(int n);                          // 数量不能为负

    // ---------------- 库存增减（实验三：数量管理） ----------------
    bool addStock(int n);                          // 增加数量
    bool reduceStock(int n);                       // 减少数量

    // ---------------- 操作符重载（实验四扩展） ----------------
    // 按条码判断两个资源是否相同
    bool operator==(const Resource& other) const;
    // 按价格比较大小，可用于排序
    bool operator<(const Resource& other) const;
    // 输出流重载：cout << *res_ptr 即可输出资源信息
    friend std::ostream& operator<<(std::ostream& os, const Resource& r);
};

#endif
