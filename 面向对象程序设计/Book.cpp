#include "Book.h"
#include <iostream>
using namespace std;

Book::Book() {
    barcode = "001";
    isbn = "9787020040101";
    title = "活着";
    author = "余华";
    publisher = "人民文学出版社";
    price = 39.0;
    pages = 191;
    available = true;
}

Book::Book(string bc, string ib, string t, string a, string p, double pr, int pg, bool av) {
    barcode = bc;
    isbn = ib;
    title = t;
    author = a;
    publisher = p;
    price = pr;
    pages = pg;
    available = av;
}

void Book::input() {
    cout << "请输入图书信息（条码 ISBN 书名 作者 出版社 价格 页数 是否可借(1/0)）：" << endl;
    cin >> barcode >> isbn >> title >> author >> publisher >> price >> pages >> available;
}

void Book::show() {
    cout << "条码：" << barcode << " | ISBN：" << isbn
        << " | 书名：" << title << " | 作者：" << author
        << " | 出版社：" << publisher << " | 价格：" << price
        << " | 页数：" << pages << " | 在馆：" << (available ? "是" : "否") << endl;
}

void Book::setBarcode(string bc) { barcode = bc; }
string Book::getBarcode() { return barcode; }

void Book::setIsbn(string ib) { isbn = ib; }
string Book::getIsbn() { return isbn; }

void Book::setTitle(string t) { title = t; }
string Book::getTitle() { return title; }

void Book::setAvailable(bool av) { available = av; }
bool Book::getAvailable() { return available; }

// 简化版ISBN验证：校验13位纯数字格式
bool Book::checkIsbnValid() {
    if (isbn.length() != 13) return false;
    for (char c : isbn) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}