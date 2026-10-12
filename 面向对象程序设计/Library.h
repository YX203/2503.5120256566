#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "Book.h"
#include "Student.h"

const int MAX_BOOKS = 100;
const int MAX_STUDENTS = 50;

class Library {
private:
    std::string name;
    Book books[MAX_BOOKS];
    int bookCount;
    Student students[MAX_STUDENTS];
    int studentCount;

public:
    Library();
    Library(std::string libName);

    bool addBook(const Book& b);
    bool removeBook(std::string barcode);
    Book* findBook(std::string barcode);
    void showAllBooks() const;

    bool addStudent(const Student& s);
    Student* findStudent(std::string id);
    void showAllStudents() const;

    bool borrowBook(std::string stuId, std::string barcode);
    bool returnBook(std::string stuId, std::string barcode);

    void showInfo() const;

    std::string getName() const;
    int getBookCount() const;
    int getStudentCount() const;
};

#endif