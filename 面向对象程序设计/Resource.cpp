#include "Resource.h"
#include <iostream>

using namespace std;

Resource::Resource()
    : barcode(""), title(""), publisher(""), price(0.0), available(true) {
}

Resource::Resource(string bc, string t, string p, double pr, bool av)
    : barcode(bc), title(t), publisher(p), price(pr), available(av) {
}

void Resource::show() const {
    cout << "条码：" << barcode << " | 名称：" << title
        << " | 出版社：" << publisher << " | 价格：" << price
        << " | 在馆：" << (available ? "是" : "否") << " | ";
}

string Resource::getBarcode() const { return barcode; }
string Resource::getTitle() const { return title; }
string Resource::getPublisher() const { return publisher; }
double Resource::getPrice() const { return price; }
bool Resource::getAvailable() const { return available; }

void Resource::setBarcode(string bc) { barcode = bc; }
void Resource::setTitle(string t) { title = t; }
void Resource::setPublisher(string p) { publisher = p; }
void Resource::setPrice(double pr) { price = pr; }
void Resource::setAvailable(bool av) { available = av; }