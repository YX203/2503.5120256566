#pragma once
#ifndef BOOK_H
#define BOOK_H
#include <string>
using namespace std;

// 图书类：实验一的单个类设计，实验二中作为被依赖/被组合的"部分类"
class Book {
private:
    string barcode;    // 图书条码
    string isbn;       // ISBN号
    string title;      // 书名
    string author;     // 作者
    string publisher;  // 出版社
    double price;      // 价格
    int pages;         // 页数
    bool available;    // 在馆状态（true=可借，false=已借出）
public:
    // 构造函数重载（实验一要求：默认构造 + 带参构造）
    Book();
    Book(string bc, string ib, string t, string a, string p, double pr, int pg, bool av);

    // 基本操作：输入、输出
    void input();
    void show();

    // 属性修改、获取接口（访问控制体现封装性）
    void setBarcode(string bc);
    string getBarcode();
    void setIsbn(string ib);
    string getIsbn();
    void setTitle(string t);
    string getTitle();
    void setAuthor(string a);
    string getAuthor();
    void setPublisher(string p);
    string getPublisher();
    void setPrice(double pr);
    double getPrice();
    void setPages(int pg);
    int getPages();
    void setAvailable(bool av);
    bool getAvailable();

    // 扩展功能：验证ISBN合法性（13位纯数字）
    bool checkIsbnValid();
};
#endif
