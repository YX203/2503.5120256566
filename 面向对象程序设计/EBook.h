#pragma once
#ifndef EBOOK_H
#define EBOOK_H

#include "Resource.h"

// ============================================================
// EBook：电子资源类，公有继承自 Resource（实验三）
//   新增文件大小、下载链接。
//   实验四要点：电子资源可以被多人同时借阅（不占用馆藏副本），
//   因此重写基类的虚函数 borrowOne / returnOne，
//   这正是"同名函数、不同实现"的多态体现。
// ============================================================
class EBook : public Resource {
private:
    double      fileSize;      // 文件大小(MB)
    std::string downloadLink;  // 下载链接

public:
    EBook();
    EBook(const std::string& bc, const std::string& t, const std::string& p,
          double pr, double size, const std::string& link, int count = 1);

    // 重写基类纯虚函数（实验四：多态接口）
    std::string getTypeName() const override;
    int         getLendDays() const override;
    double      getFinePerDay() const override;

    // 重写基类虚函数：电子资源不限份数
    void print(std::ostream& os) const override;
    bool borrowOne() override;
    bool returnOne() override;
    std::string getStockDesc() const override;

    // 新增成员函数的设置与获取
    bool        setFileSize(double size);
    bool        setDownloadLink(const std::string& link);

    double      getFileSize() const;
    std::string getDownloadLink() const;
};

#endif
