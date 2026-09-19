#include "Book.h"
#include <iostream>

Book::Book() {
    barcode = "000000";
    isbn = "000-0-0000-0000-0";
    title = "none";
    author = "unknown";
    publisher = "unknown";
    available = true;
}

Book::Book(string bc, string ib, string t, string a, string p, bool av) {
    barcode = bc;
    isbn = ib;
    title = t;
    author = a;
    publisher = p;
    available = av;
}

void Book::input() {
    cin >> barcode >> isbn >> title >> author >> publisher >> available;
}

void Book::show() {
    cout << barcode << "  " << isbn << "  " << title << "  "
        << author << "  " << publisher << "  "
        << (available ? "yes" : "no") << endl;
}

void Book::setAvailable(bool av) {
    available = av;
}

bool Book::getAvailable() {
    return available;
}