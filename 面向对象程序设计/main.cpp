#include "Book.h"
#include <iostream>

int main() {
    Book b1;
    b1.show();

    Book b2("1001", "978-7-111-12345-6", "活着", "余华", "人民出版社", true);
    b2.show();

    b2.setAvailable(false);
    b2.show();

    return 0;
}