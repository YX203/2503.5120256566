#include "EBook.h"
#include <iostream>

using namespace std;

EBook::EBook() : Resource("E001", "C++ Primer 电子版", "人民邮电出版社", 50.0, true),
fileSize(15.5), downloadLink("http://example.com/cpp_primer.pdf") {
}

EBook::EBook(string bc, string t, string p, double pr, bool av, double size, string link)
    : Resource(bc, t, p, pr, av), fileSize(size), downloadLink(link) {
}

double EBook::getFileSize() const { return fileSize; }
string EBook::getDownloadLink() const { return downloadLink; }

void EBook::show() const {
    Resource::show();
    cout << "大小：" << fileSize << "MB | 下载链接：" << downloadLink << endl;
}