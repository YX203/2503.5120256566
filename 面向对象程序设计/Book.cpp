#include "Book.h"

using namespace std;

// ISBN 合法性校验：必须是 13 位纯数字
bool Book::isValidIsbn(const string& isbn) {
    if (isbn.length() != 13) return false;
    for (size_t i = 0; i < isbn.length(); ++i) {
        char c = isbn[i];
        if (c < '0' || c > '9') return false;
    }
    return true;
}

// 默认构造函数：调用基类构造函数完成基类部分的初始化
Book::Book()
    : Resource("B001", "活着", "人民文学出版社", 39.0, 1),
      isbn("9787020040101"), author("余华"), pages(191) {
}

// 重载构造函数：初始化列表里先调用基类构造函数，再初始化本类新增成员
Book::Book(const string& bc, const string& ib, const string& t,
           const string& a, const string& p, double pr, int pg, int count)
    : Resource(bc, t, p, pr, count), author(a), pages(pg > 0 ? pg : 0) {
    if (!isValidIsbn(ib)) {
        cout << "警告：ISBN " << ib << " 非法（必须是13位纯数字），已重置为 0000000000000" << endl;
        isbn = "0000000000000";
    }
    else {
        isbn = ib;
    }
}

string Book::getTypeName() const { return "普通图书"; }
int    Book::getLendDays() const { return 30; }      // 图书借期 30 天
double Book::getFinePerDay() const { return 0.2; }   // 逾期每天 0.2 元

void Book::print(ostream& os) const {
    Resource::print(os);   // 先输出基类通用信息
    os << " | ISBN：" << isbn
        << " | 作者：" << author
        << " | 页数：" << pages;
}

bool Book::setAuthor(const string& a) {
    if (a.empty()) {
        cout << "设置失败：作者不能为空。" << endl;
        return false;
    }
    author = a;
    return true;
}

bool Book::setPages(int pg) {
    if (pg <= 0) {
        cout << "设置失败：页数必须为正整数。" << endl;
        return false;
    }
    pages = pg;
    return true;
}

bool Book::setIsbn(const string& ib) {
    if (!isValidIsbn(ib)) {
        cout << "设置失败：ISBN 必须是13位纯数字。" << endl;
        return false;
    }
    isbn = ib;
    return true;
}

string Book::getIsbn() const { return isbn; }
string Book::getAuthor() const { return author; }
int    Book::getPages() const { return pages; }
bool   Book::checkIsbnValid() const { return isValidIsbn(isbn); }
