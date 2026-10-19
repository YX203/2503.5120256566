#pragma once
#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "Resource.h"

// ============================================================
// Magazine：杂志类，公有继承自 Resource（实验三）
//   新增期号、主编，并给出"杂志"这一类资源的借期与罚金规则（实验四）
// ============================================================
class Magazine : public Resource {
private:
    std::string issue;   // 期号
    std::string editor;  // 主编

public:
    Magazine();
    Magazine(const std::string& bc, const std::string& t, const std::string& p,
             double pr, const std::string& iss, const std::string& ed,
             int count = 1);

    // 重写基类纯虚函数（实验四：多态接口）
    std::string getTypeName() const override;
    int         getLendDays() const override;
    double      getFinePerDay() const override;

    // 重写基类虚函数
    void print(std::ostream& os) const override;

    // 新增成员函数的设置与获取
    bool setIssue(const std::string& iss);
    bool setEditor(const std::string& ed);

    std::string getIssue() const;
    std::string getEditor() const;
};

#endif
