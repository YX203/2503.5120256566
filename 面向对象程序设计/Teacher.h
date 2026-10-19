#pragma once
#ifndef TEACHER_H
#define TEACHER_H

#include "Reader.h"

// ============================================================
// Teacher：教师借阅人，公有继承自 Reader（实验三）
//   实验四：与 Student 一起放进 Reader* 指针数组，
//   通过基类指针调用同名虚函数，输出结果各不相同 —— 这就是多态。
// ============================================================
class Teacher : public Reader {
private:
    std::string title;   // 职称（教授/副教授/讲师）
    std::string college; // 所属学院

public:
    Teacher();
    Teacher(const std::string& tid, const std::string& tname,
            const std::string& dept, int max = 10,
            const std::string& titleName = "讲师", const std::string& collegeName = "");

    // 重写基类纯虚函数（实验四：多态接口）
    std::string getReaderType() const override;
    int         getDefaultMaxBorrow() const override;

    // 重写基类虚函数
    void print(std::ostream& os) const override;

    // 教师特有属性的设置与获取
    bool setTitle(const std::string& t);
    bool setCollege(const std::string& c);

    std::string getTitle() const;
    std::string getCollege() const;
};

#endif
