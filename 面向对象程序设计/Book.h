#pragma once
#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    string barcode;    // 图书条码
    string isbn;       // ISBN号
    string title;      // 书名
    string author;     // 作者
    string publisher;  // 出版社
    double price;      // 价格
    int pages;         // 页数
    bool available;    // 在馆状态

public:
    // 构造函数重载
    Book();
    Book(string bc, string ib, string t, string a, string p, double pr, int pg, bool av);

    // 基本操作：输入、输出
    void input();
    void show();

    // 属性修改、获取接口
    void setBarcode(string bc);
    string getBarcode();
    void setIsbn(string ib);
    string getIsbn();
    void setTitle(string t);
    string getTitle();
    void setAvailable(bool av);
    bool getAvailable();

    // 扩展功能：验证ISBN合法性
    bool checkIsbnValid();
};

#endif