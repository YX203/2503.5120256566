#include "EBook.h"

using namespace std;

EBook::EBook()
    : Resource("E001", "C++ Primer 电子版", "人民邮电出版社", 50.0, 1),
      fileSize(15.5), downloadLink("http://example.com/cpp_primer.pdf") {
}

EBook::EBook(const string& bc, const string& t, const string& p,
             double pr, double size, const string& link, int count)
    : Resource(bc, t, p, pr, count),
      fileSize(size > 0 ? size : 0.0), downloadLink(link) {
    if (downloadLink.empty()) downloadLink = "无";
}

string EBook::getTypeName() const { return "电子资源"; }
int    EBook::getLendDays() const { return 3; }      // 电子资源按授权期 3 天
double EBook::getFinePerDay() const { return 1.0; }  // 逾期每天 1.0 元

void EBook::print(ostream& os) const {
    Resource::print(os);
    os << " | 大小：" << fileSize << "MB"
        << " | 下载链接：" << downloadLink
        << " | （电子资源不占馆藏副本，可多人同时借阅）";
}

// 电子资源可同时借阅，不扣减副本数量
bool EBook::borrowOne() {
    return true;
}

bool EBook::returnOne() {
    return true;
}

// 电子资源的"可借数量"不按副本计，直接显示"不限份数"
string EBook::getStockDesc() const {
    return "不限份数";
}

bool EBook::setFileSize(double size) {
    if (size <= 0.0) {
        cout << "设置失败：文件大小必须为正数。" << endl;
        return false;
    }
    fileSize = size;
    return true;
}

bool EBook::setDownloadLink(const string& link) {
    if (link.empty()) {
        cout << "设置失败：下载链接不能为空。" << endl;
        return false;
    }
    downloadLink = link;
    return true;
}

double      EBook::getFileSize() const { return fileSize; }
string      EBook::getDownloadLink() const { return downloadLink; }
