#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Resource.h"
#include "Student.h"

const int MAX_RESOURCES = 100;
const int MAX_STUDENTS = 50;

// 图书馆类（组合类）：包含资源指针数组和学生对象数组
class Library {
private:
    std::string name;
    Resource* resources[MAX_RESOURCES]; // 基类指针数组，支持多态
    int resourceCount;
    Student students[MAX_STUDENTS];
    int studentCount;

public:
    Library();
    Library(std::string libName);
    ~Library(); // 析构函数释放资源内存

    bool addResource(Resource* res);
    bool removeResource(std::string barcode);
    Resource* findResource(std::string barcode);
    void showAllResources() const;

    bool addStudent(const Student& s);
    Student* findStudent(std::string id);
    void showAllStudents() const;

    bool borrowResource(std::string stuId, std::string barcode);
    bool returnResource(std::string stuId, std::string barcode);

    void showInfo() const;

    std::string getName() const;
    int getResourceCount() const;
    int getStudentCount() const;
};

#endif