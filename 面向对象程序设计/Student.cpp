#include "Student.h"

using namespace std;

Student::Student()
    : Reader("20230001", "张三", "计算机学院", 5),
      major("计算机科学与技术"), grade(2) {
}

Student::Student(const string& sid, const string& sname,
                 const string& dept, int max,
                 const string& majorName, int gradeNo)
    : Reader(sid, sname, dept, max > 0 ? max : 5),
      major(majorName.empty() ? "未填写" : majorName) {
    // 合法性校验：年级只能是 1~5，超出范围则修正为 1
    if (gradeNo < 1 || gradeNo > 5) {
        cout << "警告：年级 " << gradeNo << " 不合法（应在 1~5 之间），已修正为 1。" << endl;
        grade = 1;
    }
    else {
        grade = gradeNo;
    }
}

string Student::getReaderType() const { return "学生"; }
int    Student::getDefaultMaxBorrow() const { return 5; }

void Student::print(ostream& os) const {
    Reader::print(os);   // 先输出基类通用信息（体现复用）
    os << " | 专业：" << major << " | 年级：" << grade << "年级";
}

bool Student::setMajor(const string& m) {
    if (m.empty()) {
        cout << "设置失败：专业不能为空。" << endl;
        return false;
    }
    major = m;
    return true;
}

bool Student::setGrade(int g) {
    if (g < 1 || g > 5) {
        cout << "设置失败：年级必须在 1~5 之间。" << endl;
        return false;
    }
    grade = g;
    return true;
}

string Student::getMajor() const { return major; }
int    Student::getGrade() const { return grade; }
