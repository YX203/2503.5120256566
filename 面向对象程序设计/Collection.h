#pragma once
#ifndef COLLECTION_H
#define COLLECTION_H

#include <vector>
#include <algorithm>
#include <cstddef>
#include <utility>
#include <type_traits>

// ============================================================
// PtrCollection<T>：对象指针容器类模板（实验五）
//
//   为什么要有这个类？
//   ------------------------------------------------------------
//   实验五的名称是"模板和vector容器的使用"，两件事都要落地：
//   (1) 模板：把"管理一堆对象"这件事抽象成类模板，
//       T 就是被管理的元素的类型（本项目中 T = Resource / Reader，
//       都是抽象基类）。一组结构相同、只是类型不同的类，
//       用一个模板就全部覆盖，这是"类模板是一组类的抽象"的直观体现。
//   (2) vector：内部用 std::vector<T*> 顺序存放对象指针，
//       容量按需自动增长，替代实验三、实验四里写死的
//       Resource* resources[100] 这种定长数组（弊端：容量写死、
//       删除元素要手工搬移、无法知道真实元素个数）。
//
//   与实验二的关系（组合关系没有被淡化）：
//       Library "拥有" 一批资源与一批借阅人 —— has-a 关系。
//       以前这个"拥有"是用数组成员表现的，现在改用 PtrCollection
//       成员，组合关系的语义完全一样，只是部分类的实现方式升级了。
//       容器负责"持有并释放"，Library 只负责"管理与业务"，
//       这正好又是实验一讲过的单一职责原则。
//
//   与实验四的关系（多态没有被淡化）：
//       vector 里存的是 T*（基类指针），遍历时通过基类指针调用
//       虚函数，Book / Magazine / EBook 各自输出自己的内容。
//       这正是实验五思考题"基类和派生类能否放在一个 vector 下"
//       的标准答案：可以，条件是 vector 的元素类型为基类指针。
//
//   与实验一的关系（三法则没有被淡化）：
//       本类持有 new 出来的堆对象，是"资源管理者"，
//       所以拷贝构造 / 赋值运算符必须禁止（否则两个容器会 delete
//       同一块内存），析构函数负责统一回收 —— 一删全删，语义一致。
// ============================================================
template <class T>
class PtrCollection {
public:
    // 迭代器类型：直接复用 vector 的迭代器，供 begin()/end() 使用
    typedef typename std::vector<T*>::iterator       iterator;
    typedef typename std::vector<T*>::const_iterator const_iterator;

private:
    std::vector<T*> items;   // 真正的数据结构：vector 里放的是基类指针

    // 禁止拷贝与赋值（资源管理者不允许"两个主人管同一批堆对象"）
    PtrCollection(const PtrCollection&);
    PtrCollection& operator=(const PtrCollection&);

public:
    PtrCollection() {}
    ~PtrCollection() { clearAll(); }   // 析构时统一 delete，避免内存泄漏

    // ---------------- 增加元素：push_back ----------------
    // 把已经 new 好的对象交给容器保管（所有权转移）
    bool add(T* p) {
        if (p == nullptr) return false;
        items.push_back(p);       // vector::push_back
        return true;
    }

    // ---------------- 增加元素：emplace_back + 变参模板 ----------------
    // 直接在容器尾部原地构造一个派生类对象，减少一次临时对象的开销。
    //   Derived：要创建的具体派生类（Book / Magazine / EBook / Student ...）
    //   Args   ：该派生类构造函数所需的全部参数（类型和个数都不限制）
    // std::forward 把参数原样转发给 Derived 的构造函数（完美转发）。
    // 这里同时用到了：类模板 + 成员函数模板 + 可变参数模板 + vector::emplace_back。
    template <class Derived, class... Args>
    Derived* emplaceNew(Args&&... args) {
        static_assert(std::is_base_of<T, Derived>::value,
                      "Derived 必须是 T 的派生类，否则无法放进这个容器");
        Derived* p = new Derived(std::forward<Args>(args)...);
        items.emplace_back(p);    // vector::emplace_back
        return p;
    }

    // ---------------- 删除元素：erase ----------------
    // 按下标删除：先 delete 掉堆对象，再把指针从 vector 中 erase 掉
    bool removeAt(std::size_t index) {
        if (index >= items.size()) return false;
        iterator it = items.begin() + static_cast<std::ptrdiff_t>(index);
        delete *it;               // 释放派生类对象（基类虚析构保证正确）
        items.erase(it);          // vector::erase，后面的元素自动前移
        return true;
    }

    // ---------------- 按条件删除：erase + 谓词模板 ----------------
    // 返回真正删掉的元素个数。pred 可以是函数、函数对象或 lambda。
    template <class Pred>
    int removeIf(Pred pred) {
        int removed = 0;
        iterator it = items.begin();
        while (it != items.end()) {
            if (pred(*it)) {
                delete *it;
                it = items.erase(it);   // erase 返回下一个有效迭代器
                ++removed;
            }
            else {
                ++it;
            }
        }
        return removed;
    }

    // ---------------- 清空：clear ----------------
    // 演示实验五要求的"最后，清空 vector 回收相应的资源"
    void clearAll() {
        for (iterator it = items.begin(); it != items.end(); ++it) {
            delete *it;               // 逐个释放堆对象
            *it = nullptr;
        }
        items.clear();                // vector::clear，size 归 0，容量保留
    }

    // ---------------- 容量与下标访问 ----------------
    std::size_t size() const { return items.size(); }         // vector::size
    bool        empty() const { return items.empty(); }
    std::size_t capacity() const { return items.capacity(); } // vector::capacity
    T* at(std::size_t i) const { return items.at(i); }        // 带边界检查
    T* operator[](std::size_t i) const { return items[i]; }

    // ---------------- 迭代器 ----------------
    iterator       begin() { return items.begin(); }          // vector::begin
    iterator       end() { return items.end(); }              // vector::end
    const_iterator begin() const { return items.begin(); }
    const_iterator end() const { return items.end(); }

    // ---------------- 遍历：让每个元素发挥各自的功能 ----------------
    // fn 由调用方给出（lambda / 函数对象），容器本身不关心元素怎么用。
    // 因为元素是基类指针，fn 里通过基类指针调用虚函数即可产生多态行为。
    template <class Func>
    void forEach(Func fn) const {
        for (const_iterator it = items.begin(); it != items.end(); ++it) {
            fn(*it);
        }
    }

    // ---------------- 按条件查找：返回下标，找不到返回 -1 ----------------
    template <class Pred>
    int findIndexIf(Pred pred) const {
        for (std::size_t i = 0; i < items.size(); ++i) {
            if (pred(items[i])) return static_cast<int>(i);
        }
        return -1;
    }

    // ---------------- 排序：把比较规则交给调用方 ----------------
    // 说明：元素的静态类型是基类指针，不能直接对指针比大小
    //       （那比较的是地址），必须解引用后用重载的 operator< 比较。
    template <class Compare>
    void sortBy(Compare cmp) {
        std::sort(items.begin(), items.end(), cmp);   // 用 vector 的迭代器区间排序
    }
};

#endif
