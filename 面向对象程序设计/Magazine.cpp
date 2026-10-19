#include "Magazine.h"

using namespace std;

Magazine::Magazine()
    : Resource("M001", "读者", "读者出版传媒", 10.0, 1),
      issue("2024年第1期"), editor("编辑部") {
}

Magazine::Magazine(const string& bc, const string& t, const string& p,
                   double pr, const string& iss, const string& ed, int count)
    : Resource(bc, t, p, pr, count), issue(iss), editor(ed) {
    if (issue.empty())  issue = "未注明期号";
    if (editor.empty()) editor = "未注明主编";
}

string Magazine::getTypeName() const { return "杂志"; }
int    Magazine::getLendDays() const { return 7; }      // 杂志借期 7 天
double Magazine::getFinePerDay() const { return 0.5; }  // 逾期每天 0.5 元

void Magazine::print(ostream& os) const {
    Resource::print(os);
    os << " | 期号：" << issue
        << " | 主编：" << editor;
}

bool Magazine::setIssue(const string& iss) {
    if (iss.empty()) {
        cout << "设置失败：期号不能为空。" << endl;
        return false;
    }
    issue = iss;
    return true;
}

bool Magazine::setEditor(const string& ed) {
    if (ed.empty()) {
        cout << "设置失败：主编不能为空。" << endl;
        return false;
    }
    editor = ed;
    return true;
}

string Magazine::getIssue() const { return issue; }
string Magazine::getEditor() const { return editor; }
