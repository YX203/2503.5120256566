#include "Reader.h"
#include "Resource.h"
#include "TimeUtil.h"
#include <iomanip>

using namespace std;

Reader::Reader()
    : id("00000000"), name("无名"), department("未填写"),
      maxBorrow(5), recordCount(0) {
}

Reader::Reader(const string& rid, const string& rname,
               const string& dept, int max)
    : id(rid), name(rname), department(dept), recordCount(0) {
    // 合法性校验：最大借阅数量不能为负，也不能超过记录数组能容纳的上限
    if (max < 0) max = 0;
    if (max > MAX_RECORD) max = MAX_RECORD;
    maxBorrow = max;

    if (id.empty())   id = "00000000";
    if (name.empty()) name = "无名";
    if (department.empty()) department = "未填写";
}

void Reader::print(ostream& os) const {
    os << "[" << getReaderType() << "] "
        << "编号：" << id
        << " | 姓名：" << name
        << " | 院系：" << department
        << " | 已借/上限：" << getBorrowedCount() << "/" << maxBorrow
        << " | 逾期：" << getOverdueCount() << "册";
    double fine = getUnpaidFine();
    if (fine > 0.0) {
        os << " | 待缴罚金：" << fixed << setprecision(2) << fine << "元";
    }
    os.unsetf(ios::fixed);
}

void Reader::show() const {
    print(cout);
    cout << endl;
}

// 借阅：这是"依赖关系"的具体体现 —— 参数是另一个类对象的引用
bool Reader::borrowResource(Resource& res) {
    // 1. 判断资源状态
    if (!res.getAvailable()) {
        cout << "借阅失败：《" << res.getTitle() << "》暂无可借副本。" << endl;
        return false;
    }
    // 2. 判断借阅人状态
    if (getBorrowedCount() >= maxBorrow) {
        cout << "借阅失败：" << name << " 已达最大借阅数量(" << maxBorrow << "本)。" << endl;
        return false;
    }
    if (recordCount >= MAX_RECORD) {
        cout << "借阅失败：借阅记录已满，请先归还部分资源。" << endl;
        return false;
    }
    // 3. 不允许同一本纸质资源重复借阅（电子资源不占副本，允许）
    if (res.getTypeName() != "电子资源") {
        for (int i = 0; i < recordCount; ++i) {
            if (!records[i].isReturned() && records[i].getBarcode() == res.getBarcode()) {
                cout << "借阅失败：《" << res.getTitle() << "》已在您手中，尚未归还。" << endl;
                return false;
            }
        }
    }

    // 4. 双方状态同时改变（通过引用的地址传参）
    res.borrowOne();
    records[recordCount] = BorrowRecord(res.getBarcode(), res.getTitle(),
                                        res.getTypeName(), res.getLendDays(),
                                        res.getFinePerDay());
    ++recordCount;

    cout << "借阅成功：" << name << " 借到《" << res.getTitle() << "》"
        << "（" << res.getTypeName() << "），借期 " << res.getLendDays()
        << " 天，应还日期：" << TimeUtil::toString(records[recordCount - 1].getDueTime()) << endl;
    return true;
}

// 归还：计算逾期天数与罚金，同时恢复资源状态
bool Reader::returnResource(Resource& res) {
    int idx = -1;
    for (int i = 0; i < recordCount; ++i) {
        if (!records[i].isReturned() && records[i].getBarcode() == res.getBarcode()) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "归还失败：" << name << " 没有《" << res.getTitle() << "》的未还记录。" << endl;
        return false;
    }

    double rate = res.getFinePerDay();
    int    days = records[idx].overdueDays();
    double fine = records[idx].doReturn();
    res.returnOne();

    cout << "归还成功：" << name << " 归还了《" << res.getTitle() << "》。" << endl;
    if (fine > 0.0) {
        cout << "  注意：已逾期 " << days << " 天，按每天 " << rate
            << " 元计算，应缴罚金 " << fixed << setprecision(2) << fine << " 元。"
            << endl;
        cout.unsetf(ios::fixed);
    }
    else {
        cout << "  未逾期，无需缴纳罚金。" << endl;
    }
    return true;
}

int Reader::getBorrowedCount() const {
    int n = 0;
    for (int i = 0; i < recordCount; ++i) {
        if (!records[i].isReturned()) ++n;
    }
    return n;
}

int Reader::getOverdueCount() const {
    int n = 0;
    for (int i = 0; i < recordCount; ++i) {
        if (!records[i].isReturned() && records[i].isOverdue()) ++n;
    }
    return n;
}

double Reader::getUnpaidFine() const {
    double sum = 0.0;
    for (int i = 0; i < recordCount; ++i) {
        if (!records[i].isReturned()) {
            sum += records[i].computeFine();
        }
    }
    return sum;
}

double Reader::getTotalFine() const {
    double sum = 0.0;
    for (int i = 0; i < recordCount; ++i) {
        sum += records[i].getFine();
    }
    return sum;
}

void Reader::showRecords() const {
    cout << "-- " << name << "（" << id << "）的借阅记录，共 "
        << recordCount << " 条 --" << endl;
    if (recordCount == 0) {
        cout << "  （暂无记录）" << endl;
        return;
    }
    for (int i = 0; i < recordCount; ++i) {
        cout << "  " << (i + 1) << ". ";
        records[i].show();
    }
}

string Reader::getId() const { return id; }
string Reader::getName() const { return name; }
string Reader::getDepartment() const { return department; }
int    Reader::getMaxBorrow() const { return maxBorrow; }
int    Reader::getRecordCount() const { return recordCount; }

bool Reader::setName(const string& rname) {
    if (rname.empty()) {
        cout << "设置失败：姓名不能为空。" << endl;
        return false;
    }
    name = rname;
    return true;
}

bool Reader::setDepartment(const string& dept) {
    if (dept.empty()) {
        cout << "设置失败：院系不能为空。" << endl;
        return false;
    }
    department = dept;
    return true;
}

bool Reader::setMaxBorrow(int max) {
    if (max < 0 || max > MAX_RECORD) {
        cout << "设置失败：最大借阅数量必须在 0 ~ " << MAX_RECORD << " 之间。" << endl;
        return false;
    }
    maxBorrow = max;
    return true;
}

ostream& operator<<(ostream& os, const Reader& r) {
    r.print(os);
    return os;
}
