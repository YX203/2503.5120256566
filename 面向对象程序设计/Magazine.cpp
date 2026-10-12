#include "Magazine.h"
#include <iostream>

using namespace std;

Magazine::Magazine() : Resource("M001", "读者", "读者出版传媒", 10.0, true),
issue("2024年第1期"), editor("编辑部") {
}

Magazine::Magazine(string bc, string t, string p, double pr, bool av, string iss, string ed)
    : Resource(bc, t, p, pr, av), issue(iss), editor(ed) {
}

string Magazine::getIssue() const { return issue; }
string Magazine::getEditor() const { return editor; }

void Magazine::show() const {
    Resource::show();
    cout << "期号：" << issue << " | 主编：" << editor << endl;
}