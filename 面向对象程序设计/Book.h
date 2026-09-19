#pragma once
#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    string barcode;
    string isbn;
    string title;
    string author;
    string publisher;
    bool available;

public:
    Book();
    Book(string bc, string ib, string t, string a, string p, bool av);

    void input();
    void show();
    void setAvailable(bool av);
    bool getAvailable();
};

#endif