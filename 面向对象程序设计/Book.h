#pragma once
#ifndef BOOK_H
#define BOOK_H

#include "Resource.h"

// ============================================================
// Book：普通图书类，公有继承自 Resource（实验三）
//   在基类基础上新增 ISBN、作者、页数，
//   并重写纯虚函数，给出"图书"这一类资源的借期与罚金规则（实验四）
// ============================================================
class Book : public Resource {
private:
    std::string isbn;    // 13 位 ISBN 号
    std::string author;  // 作者
    int         pages;   // 页数

    static bool isValidIsbn(const std::string& isbn);

public:
    Book();
    Book(const std::string& bc, const std::string& ib, const std::string& t,
         const std::string& a, const std::string& p, double pr,
         int pg, int count = 1);

    // 重写基类纯虚函数（实验四：多态接口）
    std::string getTypeName() const override;
    int         getLendDays() const override;
    double      getFinePerDay() const override;

    // 重写基类虚函数
    void print(std::ostream& os) const override;

    // 新增成员函数的设置与获取
    bool setAuthor(const std::string& a);
    bool setPages(int pg);
    bool setIsbn(const std::string& ib);

    std::string getIsbn() const;
    std::string getAuthor() const;
    int         getPages() const;
    bool        checkIsbnValid() const;
};

#endif
