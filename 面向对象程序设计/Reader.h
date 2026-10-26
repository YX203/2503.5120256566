#pragma once
#ifndef READER_H
#define READER_H

#include <string>
#include <iostream>
#include <vector>
#include "BorrowRecord.h"

class Resource;   // 前向声明，真正的依赖在 .cpp 中使用

const int MAX_BORROW_LIMIT = 100;   // 单个借阅人最大可借数量的上限（合法性校验用）

// ============================================================
// Reader：借阅人抽象基类
//
//   实验三（继承与派生）：派生类 Student（学生）、Teacher（教师）
//   实验四（多态与虚函数）：含纯虚函数 getReaderType()，
//         Library 用 Reader* 的容器统一管理不同身份借阅人，
//         通过基类指针调用虚函数，输出各自不同的信息。
//   实验二（组合与依赖）：
//        - 组合：records 是 BorrowRecord 的集合，是"整体—部分"关系
//        - 依赖：borrowResource(Resource& res) 参数为另一个类的对象引用，
//               借阅会同时改变 reader 和 resource 双方状态
//
//   实验五（模板与 vector）—— 这里刻意演示"vector 里放对象"：
//        以前：BorrowRecord records[MAX_RECORD];  数组，最多 50 条，写死
//        现在：std::vector<BorrowRecord> records;  按需增长，capacity 自动扩展
//        为什么不放指针？借阅记录是借阅人"身体的一部分"，生命周期完全
//        跟随借阅人，没有多态需求，也不需要在别处共享 —— 直接放对象
//        最省心（无 new/delete、无泄漏风险、内存连续、遍历更快）。
//        这正是实验五思考题一：vector 里放对象和放对象指针的差别。
// ============================================================
class Reader {
protected:
    std::string  id;            // 学号 / 工号
    std::string  name;          // 姓名
    std::string  department;    // 院系
    int          maxBorrow;     // 最大可借数量
    std::vector<BorrowRecord> records;   // 借阅记录（对象容器 => 组合 + 实验五）

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

    // ---------------- 实验五：vector 的增 / 删 / 清空 ----------------
    int  purgeReturned();   // 删除所有"已归还"的记录（erase-remove 惯用法）
    void clearRecords();    // 清空全部记录（vector::clear）
    int  getRecordCapacity() const;   // 供演示 vector::capacity

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
