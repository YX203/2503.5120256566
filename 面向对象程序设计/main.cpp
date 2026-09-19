#include "Book.h"
#include <iostream>

int main() {
    Book b1;
    b1.show();

    Book b2("1001", "978-7-111-12345-6", "C++Programming", "ZhangSan", "MachinePress", true);
    b2.show();

    b2.setAvailable(false);
    b2.show();

    return 0;
}