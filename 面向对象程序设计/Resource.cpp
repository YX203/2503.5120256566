#include "Resource.h"
#include <iomanip>

using namespace std;

Resource::Resource()
    : barcode(""), title(""), publisher(""), price(0.0), stock(0), totalCount(0) {
}

Resource::Resource(const string& bc, const string& t,
                   const string& p, double pr, int count)
    : barcode(bc), title(t), publisher(p), price(pr > 0 ? pr : 0.0),
      stock(count > 0 ? count : 0), totalCount(count > 0 ? count : 0) {
    // 合法性校验：条码/名称/出版社为空时给出默认占位值，避免出现空对象
    if (barcode.empty()) barcode = "UNKNOWN";
    if (title.empty())   title = "未命名资源";
    if (publisher.empty()) publisher = "未知出版社";
}

void Resource::print(ostream& os) const {
    // 金额统一保留两位小数；输出结束后把浮点格式恢复成默认值，
    // 避免"格式化状态"顺着流泄漏到后面其他输出里（iostream 的状态是粘的）
    os << "[" << getTypeName() << "] "
        << "条码：" << barcode
        << " | 名称：" << title
        << " | 出版社：" << publisher
        << " | 价格：" << fixed << setprecision(2) << price << "元"
        << " | 可借：" << getStockDesc()
        << " | 借期：" << getLendDays() << "天"
        << " | 日罚金：" << fixed << setprecision(2) << getFinePerDay() << "元";
    os << defaultfloat << setprecision(6);
}

void Resource::show() const {
    print(cout);
    cout << endl;
}

bool Resource::borrowOne() {
    if (stock <= 0) return false;
    --stock;
    return true;
}

bool Resource::returnOne() {
    if (stock < totalCount) {
        ++stock;
    }
    return true;
}

// 默认显示成 "3/5本"，派生类（电子资源）可以重写这一行为
string Resource::getStockDesc() const {
    return to_string(stock) + "/" + to_string(totalCount) + "本";
}

string Resource::getBarcode() const { return barcode; }
string Resource::getTitle() const { return title; }
string Resource::getPublisher() const { return publisher; }
double Resource::getPrice() const { return price; }
int    Resource::getStock() const { return stock; }
int    Resource::getTotalCount() const { return totalCount; }
bool   Resource::getAvailable() const { return stock > 0; }

bool Resource::setBarcode(const string& bc) {
    if (bc.empty()) {
        cout << "设置失败：条码不能为空。" << endl;
        return false;
    }
    barcode = bc;
    return true;
}

bool Resource::setTitle(const string& t) {
    if (t.empty()) {
        cout << "设置失败：名称不能为空。" << endl;
        return false;
    }
    title = t;
    return true;
}

bool Resource::setPublisher(const string& p) {
    if (p.empty()) {
        cout << "设置失败：出版社不能为空。" << endl;
        return false;
    }
    publisher = p;
    return true;
}

bool Resource::setPrice(double pr) {
    if (pr < 0.0) {
        cout << "设置失败：价格不能为负数。" << endl;
        return false;
    }
    price = pr;
    return true;
}

bool Resource::setStock(int n) {
    if (n < 0) {
        cout << "设置失败：可借数量不能为负数。" << endl;
        return false;
    }
    stock = n;
    if (totalCount < n) totalCount = n;
    return true;
}

bool Resource::addStock(int n) {
    if (n <= 0) {
        cout << "增加失败：数量必须为正整数。" << endl;
        return false;
    }
    stock += n;
    totalCount += n;
    return true;
}

bool Resource::reduceStock(int n) {
    if (n <= 0) {
        cout << "减少失败：数量必须为正整数。" << endl;
        return false;
    }
    if (n > stock) {
        cout << "减少失败：可借数量只有 " << stock << " 本，不能减少 " << n << " 本。" << endl;
        return false;
    }
    if (n > totalCount) n = totalCount;
    stock -= n;
    totalCount -= n;
    return true;
}

bool Resource::operator==(const Resource& other) const {
    return barcode == other.barcode;
}

bool Resource::operator<(const Resource& other) const {
    return price < other.price;
}

ostream& operator<<(ostream& os, const Resource& r) {
    r.print(os);   // 虚函数：实际调用派生类的 print
    return os;
}
