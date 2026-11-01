#include "Storage.h"
#include "Library.h"
#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "Student.h"
#include "Teacher.h"
#include "BorrowRecord.h"
#include "TimeUtil.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

namespace {

const char* RES_FILE = "resources.txt";   // 馆藏资源
const char* RDR_FILE = "readers.txt";     // 借阅人
const char* REC_FILE = "records.txt";     // 借阅记录
const char* RPT_FILE = "report.txt";      // 可读报表

// 把 "a|b|c" 拆成 {"a","b","c"}；空字段也保留，方便按下标取值
vector<string> split(const string& line) {
    vector<string> f;
    string cur;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '|') { f.push_back(cur); cur.clear(); }
        else cur += line[i];
    }
    f.push_back(cur);
    return f;
}

string fld(const vector<string>& f, size_t i) { return i < f.size() ? f[i] : string(); }
int    fldInt(const vector<string>& f, size_t i) { return atoi(fld(f, i).c_str()); }
double fldDouble(const vector<string>& f, size_t i) { return atof(fld(f, i).c_str()); }
time_t fldTime(const vector<string>& f, size_t i) { return static_cast<time_t>(atoll(fld(f, i).c_str())); }

}   // namespace

// ============================================================
// 保存：ofstream 写文件
// ============================================================
bool Storage::saveAll(const Library& lib) {
    int nRes = 0, nRdr = 0, nRec = 0;

    // 1) 馆藏资源：每行由对象自己的 saveTo() 生成（虚函数，各派生类字段不同）
    ofstream outRes(RES_FILE);
    if (!outRes) {
        cout << "保存失败：无法创建 " << RES_FILE << "（请检查当前目录的写权限）。" << endl;
        return false;
    }
    lib.forEachResource([&](Resource* r) { r->saveTo(outRes); ++nRes; });
    outRes.close();          // 用完及时 close()

    // 2) 借阅人
    ofstream outRdr(RDR_FILE);
    if (!outRdr) {
        cout << "保存失败：无法创建 " << RDR_FILE << "。" << endl;
        return false;
    }
    lib.forEachReader([&](Reader* r) { r->saveTo(outRdr); ++nRdr; });
    outRdr.close();

    // 3) 借阅记录：记录本身不含"是谁借的"，所以行首补上借阅人编号
    ofstream outRec(REC_FILE);
    if (!outRec) {
        cout << "保存失败：无法创建 " << REC_FILE << "。" << endl;
        return false;
    }
    lib.forEachReader([&](Reader* r) {
        for (const BorrowRecord& b : r->getRecords()) {
            outRec << r->getId() << '|' << b.getBarcode() << '|' << b.getTitle()
                << '|' << b.getTypeName()
                << '|' << static_cast<long long>(b.getBorrowTime())
                << '|' << static_cast<long long>(b.getDueTime())
                << '|' << static_cast<long long>(b.getReturnTime())
                << '|' << (b.isReturned() ? 1 : 0)
                << '|' << fixed << setprecision(2) << b.getFineRate()
                << '|' << b.getFine() << defaultfloat << '\n';
            ++nRec;
        }
    });
    outRec.close();

    cout << "保存完成（ofstream）：" << RES_FILE << " " << nRes << " 行、"
        << RDR_FILE << " " << nRdr << " 行、" << REC_FILE << " " << nRec << " 行。"
        << endl;
    return true;
}

// ============================================================
// 读取：ifstream 读文件，按行首的类型标记 new 出对应的派生类对象
// ============================================================
bool Storage::loadAll(Library& lib) {
    ifstream inRes(RES_FILE);
    if (!inRes) {
        cout << "读取失败：当前目录下找不到 " << RES_FILE
            << "（首次运行属正常，可先保存一次数据）。" << endl;
        return false;
    }
    // 先清空，避免重复加载（对象由容器负责释放，不会泄漏）
    if (lib.getResourceCount() > 0 || lib.getReaderCount() > 0) lib.clearAllData();

    string line;
    int nRes = 0, nRdr = 0, nRec = 0;

    // 1) 馆藏资源
    while (getline(inRes, line)) {
        if (line.empty()) continue;
        vector<string> f = split(line);
        const string type = fld(f, 0);
        Resource* r = nullptr;
        if (type == "普通图书")
            r = new Book(fld(f, 1), fld(f, 7), fld(f, 2), fld(f, 8), fld(f, 3),
                fldDouble(f, 4), fldInt(f, 9), fldInt(f, 6));
        else if (type == "杂志")
            r = new Magazine(fld(f, 1), fld(f, 2), fld(f, 3), fldDouble(f, 4),
                fld(f, 7), fld(f, 8), fldInt(f, 6));
        else if (type == "电子资源")
            r = new EBook(fld(f, 1), fld(f, 2), fld(f, 3), fldDouble(f, 4),
                fldDouble(f, 7), fld(f, 8), fldInt(f, 6));
        if (r == nullptr) continue;

        r->setStock(fldInt(f, 5));   // 构造时 stock = 总数，这里改回实际可借数
        lib.addResource(r);
        ++nRes;
    }
    inRes.close();

    // 2) 借阅人
    ifstream inRdr(RDR_FILE);
    while (getline(inRdr, line)) {
        if (line.empty()) continue;
        vector<string> f = split(line);
        const string type = fld(f, 0);
        Reader* rd = nullptr;
        if (type == "学生")
            rd = new Student(fld(f, 1), fld(f, 2), fld(f, 3), fldInt(f, 4),
                fld(f, 5), fldInt(f, 6));
        else if (type == "教师")
            rd = new Teacher(fld(f, 1), fld(f, 2), fld(f, 3), fldInt(f, 4),
                fld(f, 5), fld(f, 6));
        if (rd == nullptr) continue;
        lib.addReader(rd);
        ++nRdr;
    }
    inRdr.close();

    // 3) 借阅记录：按借阅人编号找到人，再补一条记录进去
    ifstream inRec(REC_FILE);
    while (getline(inRec, line)) {
        if (line.empty()) continue;
        vector<string> f = split(line);
        Reader* rd = lib.findReader(fld(f, 0));
        if (rd == nullptr) continue;
        rd->addRecord(BorrowRecord(fld(f, 1), fld(f, 2), fld(f, 3),
            fldTime(f, 4), fldTime(f, 5), fldTime(f, 6),
            fldInt(f, 7) != 0, fldDouble(f, 8), fldDouble(f, 9)));
        ++nRec;
    }
    inRec.close();

    cout << "读取完成（ifstream）：资源 " << nRes << " 项、借阅人 " << nRdr
        << " 人、借阅记录 " << nRec << " 条。" << endl;
    return true;
}

// ============================================================
// 导出一份给人看的报表
// ============================================================
bool Storage::exportReport(const Library& lib) {
    ofstream out(RPT_FILE);
    if (!out) {
        cout << "导出失败：无法创建 " << RPT_FILE << "。" << endl;
        return false;
    }
    out << "============ 图书馆借阅管理系统 数据报表 ============" << endl;
    out << "生成日期：" << TimeUtil::toString(TimeUtil::now()) << endl;

    out << "\n一、馆藏资源（" << lib.getResourceCount() << " 项）" << endl;
    lib.forEachResource([&out](Resource* r) {
        ostringstream oss;      // 先写进字符串流，再整行写文件，格式状态互不干扰
        oss << *r;
        out << "  " << oss.str() << endl;
    });

    out << "\n二、借阅人（" << lib.getReaderCount() << " 人）" << endl;
    lib.forEachReader([&out](Reader* r) {
        ostringstream oss;
        oss << *r;
        out << "  " << oss.str() << endl;
        for (const BorrowRecord& b : r->getRecords()) {
            ostringstream one;
            b.print(one);
            out << "      * " << one.str() << endl;
        }
    });

    out << "\n三、统计" << endl;
    out << "  馆藏种类：" << lib.getResourceCount() << " 项" << endl;
    out << "  借阅人数：" << lib.getReaderCount() << " 人" << endl;
    out.close();

    cout << "报表已导出到 " << RPT_FILE << "（可用记事本直接打开）。" << endl;
    return true;
}

// ============================================================
// 把数据文件的内容读回屏幕（验证存盘结果）
// ============================================================
void Storage::showDataFiles() {
    const char* files[3] = { RES_FILE, RDR_FILE, REC_FILE };
    for (int k = 0; k < 3; ++k) {
        ifstream in(files[k]);
        cout << "\n===== " << files[k] << " =====" << endl;
        if (!in) {
            cout << "  （文件不存在）" << endl;
            continue;
        }
        string line;
        int n = 0;
        while (getline(in, line)) {
            ++n;
            if (n <= 10) cout << "  " << line << endl;
        }
        if (n > 10) cout << "  ……（共 " << n << " 行，只显示前 10 行）" << endl;
        in.close();
    }
}
