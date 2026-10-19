#pragma once
#ifndef STUDENT_H
#define STUDENT_H

#include "Reader.h"

// ============================================================
// Student：学生借阅人，公有继承自 Reader（实验三）
//   实验四：重写纯虚函数，给出"学生"这一身份的规则与显示方式。
//   相比教师，学生最大借阅数量更少、罚金标准更高。
// ============================================================
class Student : public Reader {
private:
    std::string major;   // 专业（学生特有属性）
    int         grade;   // 年级

public:
    Student();
    Student(const std::string& sid, const std::string& sname,
            const std::string& dept, int max = 5,
            const std::string& majorName = "未填写", int gradeNo = 1);

    // 重写基类纯虚函数（实验四：多态接口）
    std::string getReaderType() const override;
    int         getDefaultMaxBorrow() const override;

    // 重写基类虚函数
    void print(std::ostream& os) const override;

    // 学生特有属性的设置与获取
    bool setMajor(const std::string& m);
    bool setGrade(int g);

    std::string getMajor() const;
    int         getGrade() const;
};

#endif
