#pragma once
#ifndef BOOK_H
#define BOOK_H

#include "Resource.h"

// 普通图书类：派生自 Resource
class Book : public Resource {
private:
    std::string isbn;
    std::string author;
    int pages;

    static bool isValidIsbn(const std::string& isbn);

public:
    Book();
    Book(std::string bc, std::string ib, std::string t, std::string a,
        std::string p, double pr, int pg, bool av);

    // 补上这两个 setter
    void setAuthor(std::string a);
    void setPages(int pg);

    bool setIsbn(std::string ib);
    std::string getIsbn() const;
    std::string getAuthor() const;
    int getPages() const;
    bool checkIsbnValid() const;

    // 重写基类虚函数
    void show() const override;
};

#endif