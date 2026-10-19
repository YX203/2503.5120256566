#pragma once
#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <string>
#include <iostream>
#include <ctime>

// ============================================================
// BorrowRecord：一条借阅记录
//   实验二 —— 它是 Reader 的成员变量（对象成员），体现组合关系：
//             一个借阅人"包含"若干条借阅记录。
//   职责：保存借出日期、应还日期、归还日期，
//         以及借出时该资源对应的日罚金标准，
//         据此计算逾期天数与罚款。
// ============================================================
class BorrowRecord {
private:
    std::string barcode;     // 资源条码
    std::string title;       // 资源名称（冗余保存，便于查询时显示）
    std::string typeName;    // 资源类型（图书/杂志/电子资源）
    time_t      borrowTime;  // 借出日期
    time_t      dueTime;     // 应还日期
    time_t      returnTime;  // 实际归还日期
    bool        returned;    // 是否已归还
    double      fineRate;    // 该资源的日罚金标准（借出时由虚函数取得）
    double      fine;        // 实际缴纳/应缴的罚金

public:
    BorrowRecord();
    BorrowRecord(const std::string& bc, const std::string& t,
                 const std::string& type, int lendDays, double rate);

    bool   isOverdue() const;        // 是否逾期
    int    overdueDays() const;      // 逾期天数
    double computeFine() const;      // 按日罚金计算应缴罚金

    double doReturn();               // 办理归还，登记日期并结算罚金

    std::string getBarcode() const;
    std::string getTitle() const;
    std::string getTypeName() const;
    bool        isReturned() const;
    double      getFine() const;
    double      getFineRate() const;
    time_t      getBorrowTime() const;
    time_t      getDueTime() const;
    time_t      getReturnTime() const;

    void print(std::ostream& os) const;
    void show() const;
};

#endif
