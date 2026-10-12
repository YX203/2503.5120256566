#include "Book.h"
#include <iostream>

using namespace std;

bool Book::isValidIsbn(const string& isbn) {
    if (isbn.length() != 13) return false;
    for (char c : isbn) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

Book::Book()
    : barcode("001"), isbn("9787020040101"), title("活着"), author("余华"),
    publisher("人民文学出版社"), price(39.0), pages(191), available(true) {
}

Book::Book(string bc, string ib, string t, string a, string p, double pr, int pg, bool av)
    : barcode(bc), title(t), author(a), publisher(p), price(pr), pages(pg), available(av) {
    if (!isValidIsbn(ib)) {
        cout << "警告：ISBN " << ib << " 非法，已重置为 0000000000000" << endl;
        isbn = "0000000000000";
    }
    else {
        isbn = ib;
    }
}

void Book::input() {
    string tempIsbn;
    int av;
    cout << "请输入图书信息（条码 ISBN 书名 作者 出版社 价格 页数 是否可借(1/0)）：" << endl;
    cin >> barcode >> tempIsbn >> title >> author >> publisher >> price >> pages >> av;
    available = (av == 1);
    if (!setIsbn(tempIsbn)) {
        cout << "ISBN 非法，保留原值：" << isbn << endl;
    }
}

void Book::show() const {
    cout << "条码：" << barcode << " | ISBN：" << isbn
        << " | 书名：" << title << " | 作者：" << author
        << " | 出版社：" << publisher << " | 价格：" << price
        << " | 页数：" << pages << " | 在馆：" << (available ? "是" : "否") << endl;
}

void Book::setBarcode(string bc) { barcode = bc; }
string Book::getBarcode() const { return barcode; }

bool Book::setIsbn(string ib) {
    if (!isValidIsbn(ib)) {
        cout << "ISBN 设置失败：必须是13位纯数字。" << endl;
        return false;
    }
    isbn = ib;
    return true;
}
string Book::getIsbn() const { return isbn; }

void Book::setTitle(string t) { title = t; }
string Book::getTitle() const { return title; }

void Book::setAuthor(string a) { author = a; }
string Book::getAuthor() const { return author; }

void Book::setPublisher(string p) { publisher = p; }
string Book::getPublisher() const { return publisher; }

void Book::setPrice(double pr) { price = pr; }
double Book::getPrice() const { return price; }

void Book::setPages(int pg) { pages = pg; }
int Book::getPages() const { return pages; }

void Book::setAvailable(bool av) { available = av; }
bool Book::getAvailable() const { return available; }

bool Book::checkIsbnValid() const {
    return isValidIsbn(isbn);
}