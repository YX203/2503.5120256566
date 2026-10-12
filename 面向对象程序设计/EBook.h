#pragma once
#ifndef EBOOK_H
#define EBOOK_H

#include "Resource.h"

// 电子资源类：派生自 Resource
class EBook : public Resource {
private:
    double fileSize;        // 大小 (MB)
    std::string downloadLink; // 下载链接

public:
    EBook();
    EBook(std::string bc, std::string t, std::string p, double pr, bool av, double size, std::string link);

    double getFileSize() const;
    std::string getDownloadLink() const;

    void show() const override;
};

#endif