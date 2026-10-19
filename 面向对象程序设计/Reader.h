#pragma once
#ifndef READER_H
#define READER_H

#include <string>
#include <iostream>
#include "BorrowRecord.h"

class Resource;   // 前向声明，真正的依赖在 .cpp 中使用

const int MAX_RECORD = 50;   // 每个借阅人最多保存的借阅记录条数

// ============================================================
// Reader：借阅人抽象基类
//
//   实验三（继承与派生）：派生类 Student（学生）、Teacher（教师）
//   实验四（多态与虚函数）：含纯虚函数 getReaderType()，
//         Library 用 Reader* 指针数组统一管理不同身份借阅人，
//         通过基类指针调用虚函数，输出各自不同的信息。
//   实验二（组合与依赖）：
//        - 组合：records[] 是 BorrowRecord 对象数组，是"整体—部分"关系
//        - 依赖：borrowResource(Resource& res) 参数为另一个类的对象引用，
//               借阅会同时改变 reader 和 resource 双方状态
// ============================================================
class Reader {
protected:
    std::string  id;            // 学号 / 工号
    std::string  name;          // 姓名
    std::string  department;    // 院系
    int          maxBorrow;     // 最大可借数量
    BorrowRecord records[MAX_RECORD];  // 借阅记录（对象数组成员 => 组合）
    int          recordCount;   // 已产生的记录条数（含已归还的）

public:
    Reader();
    Reader(const std::string& rid, const std::string& rname,
           const std::string& dept, int max);

    virtual ~Reader() {}

    // ---------------- 纯虚函数：不同身份借阅人规则不同 ----------------
    virtual std::string getReaderType() const = 0;
    virtual int         getDefaultMaxBorrow() const = 0;

    // ---------------- 虚函数 ----------------
    virtual void print(std::ostream& os) const;   // 输出借阅人信息
    void show() const;

    // ---------------- 借阅人的核心业务（实验二的依赖关系） ----------------
    bool borrowResource(Resource& res);   // 参数传引用：可直接改变资源状态
    bool returnResource(Resource& res);   // 归还并结算逾期罚金

    // ---------------- 统计 ----------------
    int    getBorrowedCount() const;      // 当前未归还的册数
    int    getOverdueCount() const;       // 当前逾期的册数
    double getUnpaidFine() const;         // 当前未还部分产生的罚金
    double getTotalFine() const;          // 历史累计罚金（含已归还时缴纳的）

    // ---------------- 输出 ----------------
    void showRecords() const;             // 输出该借阅人的全部借阅记录

    // ---------------- 获取 / 修改 ----------------
    std::string getId() const;
    std::string getName() const;
    std::string getDepartment() const;
    int         getMaxBorrow() const;
    int         getRecordCount() const;

    bool setName(const std::string& rname);
    bool setDepartment(const std::string& dept);
    bool setMaxBorrow(int max);

    friend std::ostream& operator<<(std::ostream& os, const Reader& r);
};

#endif
