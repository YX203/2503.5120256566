#include "Reader.h"
#include "Resource.h"
#include "TimeUtil.h"
#include <iomanip>
#include <algorithm>

using namespace std;

Reader::Reader()
    : id("00000000"), name("无名"), department("未填写"), maxBorrow(5) {
    // records 是 vector 对象，默认构造即为空，不需要手工初始化数组元素
}

Reader::Reader(const string& rid, const string& rname,
               const string& dept, int max)
    : id(rid), name(rname), department(dept) {
    // 合法性校验：最大借阅数量不能为负，也不能超过上限
    if (max < 0) max = 0;
    if (max > MAX_BORROW_LIMIT) max = MAX_BORROW_LIMIT;
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
    os << defaultfloat << setprecision(6);   // 恢复默认浮点格式，防止状态泄漏
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
    // 3. 不允许同一本纸质资源重复借阅（电子资源不占副本，允许）
    if (res.getTypeName() != "电子资源") {
        for (size_t i = 0; i < records.size(); ++i) {
            if (!records[i].isReturned() && records[i].getBarcode() == res.getBarcode()) {
                cout << "借阅失败：《" << res.getTitle() << "》已在您手中，尚未归还。" << endl;
                return false;
            }
        }
    }

    // 4. 双方状态同时改变（通过引用的地址传参）
    res.borrowOne();
    // 实验五：vector::emplace_back —— 直接在容器尾部构造一条记录，
    // 不需要先造一个临时 BorrowRecord 对象再 push_back 拷进去。
    records.emplace_back(res.getBarcode(), res.getTitle(),
                         res.getTypeName(), res.getLendDays(),
                         res.getFinePerDay());

    const BorrowRecord& rec = records.back();   // vector::back()，最新一条记录
    cout << "借阅成功：" << name << " 借到《" << res.getTitle() << "》"
        << "（" << res.getTypeName() << "），借期 " << res.getLendDays()
        << " 天，应还日期：" << TimeUtil::toString(rec.getDueTime()) << endl;
    cout << "  （该借阅人现有 " << records.size() << " 条记录，vector 容量 "
        << records.capacity() << "）" << endl;
    return true;
}

// 归还：计算逾期天数与罚金，同时恢复资源状态
bool Reader::returnResource(Resource& res) {
    int idx = -1;
    for (size_t i = 0; i < records.size(); ++i) {
        if (!records[i].isReturned() && records[i].getBarcode() == res.getBarcode()) {
            idx = static_cast<int>(i);
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
        cout << defaultfloat << setprecision(6);   // 恢复默认浮点格式
    }
    else {
        cout << "  未逾期，无需缴纳罚金。" << endl;
    }
    return true;
}

// ---------------- 实验五：vector 的删除与清空 ----------------

// 删除所有已归还的记录：std::remove_if 把"要删的元素"挤到尾部，
// 返回新的逻辑结尾，再调用 vector::erase(first, last) 一次性抹掉。
// 注意：这里删的是"值对象"，erase 会自动调用每个元素的析构函数，无需 delete。
int Reader::purgeReturned() {
    size_t before = records.size();
    vector<BorrowRecord>::iterator newEnd =
        remove_if(records.begin(), records.end(),
                  [](const BorrowRecord& r) { return r.isReturned(); });
    records.erase(newEnd, records.end());   // vector::erase(区间)
    int removed = static_cast<int>(before - records.size());
    cout << name << " 的记录：" << before << " 条 -> " << records.size()
        << " 条，本次清理了 " << removed << " 条已归还记录。" << endl;
    return removed;
}

void Reader::clearRecords() {
    size_t before = records.size();
    bool hasUnreturned = getBorrowedCount() > 0;
    records.clear();                        // vector::clear，size 归 0，容量保留
    cout << name << " 的 " << before << " 条借阅记录已全部清空。";
    if (hasUnreturned) cout << "（注意：仍有资源未归还，记录已删除）";
    cout << endl;
}

int Reader::getRecordCapacity() const {
    return static_cast<int>(records.capacity());
}

// ---------------- 统计 ----------------

int Reader::getBorrowedCount() const {
    int n = 0;
    // 范围 for：遍历 vector 最简洁的写法
    for (const BorrowRecord& r : records) {
        if (!r.isReturned()) ++n;
    }
    return n;
}

int Reader::getOverdueCount() const {
    int n = 0;
    for (const BorrowRecord& r : records) {
        if (!r.isReturned() && r.isOverdue()) ++n;
    }
    return n;
}

double Reader::getUnpaidFine() const {
    double sum = 0.0;
    for (const BorrowRecord& r : records) {
        if (!r.isReturned()) sum += r.computeFine();
    }
    return sum;
}

double Reader::getTotalFine() const {
    double sum = 0.0;
    for (const BorrowRecord& r : records) {
        sum += r.getFine();
    }
    return sum;
}

void Reader::showRecords() const {
    cout << "-- " << name << "（" << id << "）的借阅记录，共 "
        << records.size() << " 条 --" << endl;
    if (records.empty()) {
        cout << "  （暂无记录）" << endl;
        return;
    }
    for (size_t i = 0; i < records.size(); ++i) {
        cout << "  " << (i + 1) << ". ";
        records[i].show();
    }
}

string Reader::getId() const { return id; }
string Reader::getName() const { return name; }
string Reader::getDepartment() const { return department; }
int    Reader::getMaxBorrow() const { return maxBorrow; }
int    Reader::getRecordCount() const { return static_cast<int>(records.size()); }

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
    if (max < 0 || max > MAX_BORROW_LIMIT) {
        cout << "设置失败：最大借阅数量必须在 0 ~ " << MAX_BORROW_LIMIT << " 之间。" << endl;
        return false;
    }
    maxBorrow = max;
    return true;
}

ostream& operator<<(ostream& os, const Reader& r) {
    r.print(os);
    return os;
}
