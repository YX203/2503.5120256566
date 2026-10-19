#pragma once
#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <string>
#include <ctime>

// ============================================================
// TimeUtil：时间工具类
//   借阅日期、应还日期、逾期天数的计算都集中在这里。
//   为了便于实验演示（不必真的等 30 天），提供"模拟时间推进"功能，
//   所有日期都以 now() 为基准，now() = 真实时间 + 模拟偏移天数。
// ============================================================
class TimeUtil {
private:
    static int offsetDays;          // 模拟时间已经推进的天数

public:
    static time_t now();            // 当前时间（含模拟偏移）

    static void advance(int days);  // 模拟时间向前推进 days 天
    static void reset();            // 模拟时间归零
    static int  getOffsetDays();

    // 把 time_t 转成 "2026-10-19" 形式的字符串
    static std::string toString(time_t t);

    // 计算 later 比 earlier 晚多少天（不足一天按 0 天算）
    static int daysBetween(time_t later, time_t earlier);

    // 在基准时间上加 days 天
    static time_t addDays(time_t base, int days);
};

#endif
