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

Book::Book() : Resource("B001", "活着", "人民文学出版社", 39.0, true),
isbn("9787020040101"), author("余华"), pages(191) {
}

Book::Book(string bc, string ib, string t, string a, string p, double pr, int pg, bool av)
    : Resource(bc, t, p, pr, av), author(a), pages(pg) {
    if (!isValidIsbn(ib)) {
        cout << "警告：ISBN " << ib << " 非法，已重置为 0000000000000" << endl;
        isbn = "0000000000000";
    }
    else {
        isbn = ib;
    }
}

// 补上 setAuthor 和 setPages 的实现
void Book::setAuthor(string a) { author = a; }
void Book::setPages(int pg) { pages = pg; }

bool Book::setIsbn(string ib) {
    if (!isValidIsbn(ib)) {
        cout << "ISBN 设置失败：必须是13位纯数字。" << endl;
        return false;
    }
    isbn = ib;
    return true;
}
string Book::getIsbn() const { return isbn; }
string Book::getAuthor() const { return author; }
int Book::getPages() const { return pages; }
bool Book::checkIsbnValid() const { return isValidIsbn(isbn); }

void Book::show() const {
    Resource::show(); // 调用基类显示通用信息
    cout << "ISBN：" << isbn << " | 作者：" << author
        << " | 页数：" << pages << endl;
}