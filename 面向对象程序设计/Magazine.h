#pragma once
#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "Resource.h"

// 杂志类：派生自 Resource
class Magazine : public Resource {
private:
    std::string issue;   // 期号
    std::string editor;  // 主编

public:
    Magazine();
    Magazine(std::string bc, std::string t, std::string p, double pr, bool av, std::string iss, std::string ed);

    std::string getIssue() const;
    std::string getEditor() const;

    void show() const override;
};

#endif