#include "TimeUtil.h"
#include <cstdio>

int TimeUtil::offsetDays = 0;

time_t TimeUtil::now() {
    return time(nullptr) + static_cast<time_t>(offsetDays) * 24 * 60 * 60;
}

void TimeUtil::advance(int days) {
    if (days > 0) offsetDays += days;
}

void TimeUtil::reset() {
    offsetDays = 0;
}

int TimeUtil::getOffsetDays() {
    return offsetDays;
}

std::string TimeUtil::toString(time_t t) {
    struct tm tmv;
#ifdef _MSC_VER
    localtime_s(&tmv, &t);
#else
    tmv = *localtime(&t);
#endif
    char buf[32];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday);
    return std::string(buf);
}

int TimeUtil::daysBetween(time_t later, time_t earlier) {
    double diff = difftime(later, earlier);
    if (diff <= 0.0) return 0;
    return static_cast<int>(diff / (24 * 60 * 60));
}

time_t TimeUtil::addDays(time_t base, int days) {
    return base + static_cast<time_t>(days) * 24 * 60 * 60;
}
