#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

// 图书资源基类
class Resource {
protected:
    std::string barcode;    // 条码
    std::string title;      // 书名/资源名
    std::string publisher;  // 出版社/出品方
    double price;           // 价格
    bool available;         // 在馆状态

public:
    Resource();
    Resource(std::string bc, std::string t, std::string p, double pr, bool av);

    // 虚析构函数：确保通过基类指针 delete 派生类对象时，派生类析构函数能被调用
    virtual ~Resource() {}

    // 虚函数：在派生类中重写，实现各自的显示方式
    virtual void show() const;

    // getters
    std::string getBarcode() const;
    std::string getTitle() const;
    std::string getPublisher() const;
    double getPrice() const;
    bool getAvailable() const;

    // setters
    void setBarcode(std::string bc);
    void setTitle(std::string t);
    void setPublisher(std::string p);
    void setPrice(double pr);
    void setAvailable(bool av);
};

#endif
