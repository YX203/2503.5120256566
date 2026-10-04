#include "Book.h"
#include <iostream>
using namespace std;

// 默认构造函数：初始化为一本默认图书《活着》
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

// 带参构造函数：重载构造，用指定参数构造图书对象
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

// 从键盘输入图书信息
void Book::input() {
    cout << "请输入图书信息（条码 ISBN 书名 作者 出版社 价格 页数 是否可借(1/0)）：" << endl;
    cin >> barcode >> isbn >> title >> author >> publisher >> price >> pages >> available;
}

// 输出图书完整信息
void Book::show() {
    cout << "条码：" << barcode << " | ISBN：" << isbn
         << " | 书名：" << title << " | 作者：" << author
         << " | 出版社：" << publisher << " | 价格：" << price
         << " | 页数：" << pages << " | 在馆：" << (available ? "是" : "否") << endl;
}

// setter / getter（封装：通过公有接口访问私有成员）
void Book::setBarcode(string bc) { barcode = bc; }
string Book::getBarcode() { return barcode; }
void Book::setIsbn(string ib) { isbn = ib; }
string Book::getIsbn() { return isbn; }
void Book::setTitle(string t) { title = t; }
string Book::getTitle() { return title; }
void Book::setAuthor(string a) { author = a; }
string Book::getAuthor() { return author; }
void Book::setPublisher(string p) { publisher = p; }
string Book::getPublisher() { return publisher; }
void Book::setPrice(double pr) { price = pr; }
double Book::getPrice() { return price; }
void Book::setPages(int pg) { pages = pg; }
int Book::getPages() { return pages; }
void Book::setAvailable(bool av) { available = av; }
bool Book::getAvailable() { return available; }

// 简化版ISBN验证：13位纯数字
bool Book::checkIsbnValid() {
    if (isbn.length() != 13) return false;
    for (char c : isbn) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}
