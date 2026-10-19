#include "Teacher.h"

using namespace std;

Teacher::Teacher()
    : Reader("T0001", "李老师", "计算机学院", 10),
      title("讲师"), college("计算机学院") {
}

Teacher::Teacher(const string& tid, const string& tname,
                 const string& dept, int max,
                 const string& titleName, const string& collegeName)
    : Reader(tid, tname, dept, max > 0 ? max : 10),
      title(titleName.empty() ? "讲师" : titleName),
      college(collegeName.empty() ? dept : collegeName) {
}

string Teacher::getReaderType() const { return "教师"; }
int    Teacher::getDefaultMaxBorrow() const { return 10; }

void Teacher::print(ostream& os) const {
    Reader::print(os);
    os << " | 职称：" << title << " | 所属学院：" << college;
}

bool Teacher::setTitle(const string& t) {
    if (t.empty()) {
        cout << "设置失败：职称不能为空。" << endl;
        return false;
    }
    title = t;
    return true;
}

bool Teacher::setCollege(const string& c) {
    if (c.empty()) {
        cout << "设置失败：学院不能为空。" << endl;
        return false;
    }
    college = c;
    return true;
}

string Teacher::getTitle() const { return title; }
string Teacher::getCollege() const { return college; }
