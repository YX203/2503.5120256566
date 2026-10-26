#pragma once
#ifndef VECTORDEMO_H
#define VECTORDEMO_H

// ============================================================
// VectorDemo：实验五"模板和 vector 容器的使用"的交互演示模块
//
//   三个演示各有侧重：
//   1) demoValueVector     —— vector 里放"对象"：BorrowRecord / Book 这类
//                             生命周期跟随主体的部件，直接按值存放最省心。
//   2) demoPointerVector   —— vector 里放"对象指针"：馆藏资源有继承层次，
//                             只有存基类指针才能把 Book / Magazine / EBook
//                             放进同一个 vector，并保持多态。
//   3) showTypeStatistics  —— 用容器迭代器遍历 + 基类指针调虚函数做统计。
//
//   这三段合起来，正好回答实验五的两个思考题。
// ============================================================

class Library;

namespace VectorDemo {

    // 演示 vector 放对象：push_back / emplace_back / size / capacity /
    // begin / end / 迭代器遍历 / erase / clear，全程键盘输入 + 屏幕输出
    void demoValueVector(const Library& lib);

    // 演示 vector 放对象指针（基类指针）：统一管理不同派生类对象，多态照常生效
    void demoPointerVector(const Library& lib);

    // 按资源类型统计（迭代器遍历 + 虚函数），并给出容量与元素个数的对照
    void showTypeStatistics(const Library& lib);

}   // namespace VectorDemo

#endif
