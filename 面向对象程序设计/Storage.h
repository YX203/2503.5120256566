#pragma once
#ifndef STORAGE_H
#define STORAGE_H

// ============================================================
// Storage：文件的输入 / 输出模块（实验六）
//
//   cin / cout 处理键盘与屏幕，fstream 处理磁盘文件。
//   "对象的保存"就是把对象相应成员变量的值写到文件里；
//   "对象的读取"就是按文件里的数据重新构造出对象。
//
//   本模块负责三件事：
//     1) saveAll   —— 用 ofstream 把馆藏、借阅人、借阅记录写进三个文本文件；
//     2) loadAll   —— 用 ifstream 逐行读回，重新 new 出对应的派生类对象；
//     3) exportReport / showDataFiles —— 导出可读报表、把文件内容打印回屏幕。
//
//   三个数据文件都放在程序当前目录下，每行一条记录，字段用 '|' 分隔：
//     resources.txt  类型|条码|名称|出版社|价格|可借数|总数|子类字段...
//     readers.txt    身份|编号|姓名|院系|最大借阅|子类字段...
//     records.txt    借阅人编号|条码|名称|类型|借出|应还|归还|是否已还|日罚金|罚金
//
//   为什么存盘用虚函数、读盘用 switch？
//   存的时候对象已经在内存里了，让 saveTo() 这个虚函数各自写自己的字段即可；
//   读的时候手上只有一行文本，基类不知道要造 Book 还是 Magazine，
//   所以只能靠行首的"类型标记"来决定 new 哪一个派生类。
//   ——这就是实验六思考题"如何既保存基类对象又保存派生类对象"的答案。
// ============================================================

class Library;

namespace Storage {

    // 把整馆数据保存到三个文本文件，返回是否成功
    bool saveAll(const Library& lib);

    // 从三个文本文件恢复到 lib 中（会先清空原有数据），返回是否成功
    bool loadAll(Library& lib);

    // 导出一份可以直接用记事本看的报表（report.txt）
    bool exportReport(const Library& lib);

    // 把三个数据文件的内容原样打印到屏幕上（ifstream 逐行读回）
    void showDataFiles();

}   // namespace Storage

#endif
