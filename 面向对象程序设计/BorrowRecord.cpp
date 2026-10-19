#include "BorrowRecord.h"
#include "TimeUtil.h"
#include <iomanip>

using namespace std;

BorrowRecord::BorrowRecord()
    : barcode(""), title(""), typeName(""), borrowTime(0), dueTime(0),
      returnTime(0), returned(false), fineRate(0.0), fine(0.0) {
}

BorrowRecord::BorrowRecord(const string& bc, const string& t,
                           const string& type, int lendDays, double rate)
    : barcode(bc), title(t), typeName(type),
      returnTime(0), returned(false), fineRate(rate > 0 ? rate : 0.0), fine(0.0) {
    borrowTime = TimeUtil::now();
    dueTime = TimeUtil::addDays(borrowTime, lendDays);
}

bool BorrowRecord::isOverdue() const {
    time_t check = returned ? returnTime : TimeUtil::now();
    return TimeUtil::daysBetween(check, dueTime) > 0;
}

int BorrowRecord::overdueDays() const {
    time_t check = returned ? returnTime : TimeUtil::now();
    return TimeUtil::daysBetween(check, dueTime);
}

double BorrowRecord::computeFine() const {
    int days = overdueDays();
    if (days <= 0) return 0.0;
    return days * fineRate;
}

double BorrowRecord::doReturn() {
    if (returned) return 0.0;
    returnTime = TimeUtil::now();
    fine = computeFine();
    returned = true;
    return fine;
}

string BorrowRecord::getBarcode() const { return barcode; }
string BorrowRecord::getTitle() const { return title; }
string BorrowRecord::getTypeName() const { return typeName; }
bool   BorrowRecord::isReturned() const { return returned; }
double BorrowRecord::getFine() const { return fine; }
double BorrowRecord::getFineRate() const { return fineRate; }
time_t BorrowRecord::getBorrowTime() const { return borrowTime; }
time_t BorrowRecord::getDueTime() const { return dueTime; }
time_t BorrowRecord::getReturnTime() const { return returnTime; }

void BorrowRecord::print(ostream& os) const {
    os << "条码：" << barcode
        << " | 名称：" << title
        << " | 类型：" << typeName
        << " | 借出：" << TimeUtil::toString(borrowTime)
        << " | 应还：" << TimeUtil::toString(dueTime)
        << " | ";
    if (returned) {
        os << "已归还(" << TimeUtil::toString(returnTime) << ")";
        if (fine > 0.0)
            os << " | 逾期" << overdueDays() << "天 | 罚金："
            << fixed << setprecision(2) << fine << "元";
    }
    else if (isOverdue()) {
        os << "未归还 | 已逾期" << overdueDays() << "天 | 应付罚金："
            << fixed << setprecision(2) << computeFine() << "元";
    }
    else {
        os << "未归还";
    }
}

void BorrowRecord::show() const {
    print(cout);
    cout << endl;
}
