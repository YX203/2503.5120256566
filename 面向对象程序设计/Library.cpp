#include "Library.h"
#include <iostream>

using namespace std;

Library::Library() : name("西南科技大学图书馆"), resourceCount(0), studentCount(0) {
    for (int i = 0; i < MAX_RESOURCES; ++i) resources[i] = nullptr;
}

Library::Library(string libName) : name(libName), resourceCount(0), studentCount(0) {
    for (int i = 0; i < MAX_RESOURCES; ++i) resources[i] = nullptr;
}

// 析构函数：释放所有动态分配的 Resource 对象
Library::~Library() {
    for (int i = 0; i < resourceCount; ++i) {
        delete resources[i];
        resources[i] = nullptr;
    }
}

bool Library::addResource(Resource* res) {
    if (resourceCount >= MAX_RESOURCES) {
        cout << "添加失败：图书馆资源已达上限(" << MAX_RESOURCES << "项)" << endl;
        return false;
    }
    resources[resourceCount++] = res;
    return true;
}

bool Library::removeResource(string barcode) {
    int idx = -1;
    for (int i = 0; i < resourceCount; i++) {
        if (resources[i]->getBarcode() == barcode) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout << "删除失败：未找到条码为 " << barcode << " 的资源" << endl;
        return false;
    }
    if (!resources[idx]->getAvailable()) {
        cout << "删除失败：资源《" << resources[idx]->getTitle() << "》正被借出，无法删除" << endl;
        return false;
    }

    delete resources[idx]; // 释放内存
    for (int i = idx; i < resourceCount - 1; i++) {
        resources[i] = resources[i + 1];
    }
    resources[--resourceCount] = nullptr;
    cout << "删除成功：条码 " << barcode << " 的资源已从馆内移除" << endl;
    return true;
}

Resource* Library::findResource(string barcode) {
    for (int i = 0; i < resourceCount; i++) {
        if (resources[i]->getBarcode() == barcode) return resources[i];
    }
    return nullptr;
}

void Library::showAllResources() const {
    cout << "===== 【" << name << "】馆藏资源(共" << resourceCount << "项) =====" << endl;
    for (int i = 0; i < resourceCount; i++) {
        cout << "[" << (i + 1) << "] ";
        resources[i]->show(); // 多态调用
    }
}

bool Library::addStudent(const Student& s) {
    if (studentCount >= MAX_STUDENTS) return false;
    students[studentCount++] = s;
    return true;
}

Student* Library::findStudent(string id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].getId() == id) return &students[i];
    }
    return nullptr;
}

void Library::showAllStudents() const {
    cout << "===== 【" << name << "】注册学生(共" << studentCount << "人) =====" << endl;
    for (int i = 0; i < studentCount; i++) {
        cout << "[" << (i + 1) << "] ";
        students[i].showInfo();
        students[i].showBorrowedBooks();
    }
}

bool Library::borrowResource(string stuId, string barcode) {
    Student* s = findStudent(stuId);
    if (!s) { cout << "借阅失败：未找到学号 " << stuId << endl; return false; }
    Resource* r = findResource(barcode);
    if (!r) { cout << "借阅失败：未找到条码 " << barcode << endl; return false; }
    return s->borrowBook(*r);
}

bool Library::returnResource(string stuId, string barcode) {
    Student* s = findStudent(stuId);
    if (!s) { cout << "还书失败：未找到学号 " << stuId << endl; return false; }
    Resource* r = findResource(barcode);
    if (!r) { cout << "还书失败：未找到条码 " << barcode << endl; return false; }
    return s->returnBook(*r);
}

void Library::showInfo() const {
    cout << "===== 图书馆信息 =====" << endl;
    cout << "名称：" << name << "\n馆藏资源：" << resourceCount << " 项\n注册学生：" << studentCount << " 人" << endl;
}

string Library::getName() const { return name; }
int Library::getResourceCount() const { return resourceCount; }
int Library::getStudentCount() const { return studentCount; }