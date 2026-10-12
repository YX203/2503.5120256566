#pragma once
#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    std::string barcode;    // 图书条码
    std::string isbn;       // ISBN号
    std::string title;      // 书名
    std::string author;     // 作者
    std::string publisher;  // 出版社
    double price;           // 价格
    int pages;              // 页数
    bool available;         // 在馆状态（true=可借，false=已借出）

    static bool isValidIsbn(const std::string& isbn);

public:
    Book();
    Book(std::string bc, std::string ib, std::string t, std::string a,
        std::string p, double pr, int pg, bool av);

    void input();
    void show() const;

    void setBarcode(std::string bc);
    std::string getBarcode() const;

    bool setIsbn(std::string ib);
    std::string getIsbn() const;

    void setTitle(std::string t);
    std::string getTitle() const;

    void setAuthor(std::string a);
    std::string getAuthor() const;

    void setPublisher(std::string p);
    std::string getPublisher() const;

    void setPrice(double pr);
    double getPrice() const;

    void setPages(int pg);
    int getPages() const;

    void setAvailable(bool av);
    bool getAvailable() const;

    bool checkIsbnValid() const;
};

#endif