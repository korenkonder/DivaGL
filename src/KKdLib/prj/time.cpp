/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "time.hpp"
#include <time.h>

namespace prj {
    prj::time prj::time::get_default() {
        static prj::time start_of_2005 = prj::strptime("2005-01-01 00:00:00");
        return start_of_2005;
    }

    prj::time strptime(std::string& str) {
        int32_t year;
        int32_t month;
        int32_t day;
        int32_t hour;
        int32_t min;
        int32_t sec;
        if (sscanf_s(str.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == 6) {
            struct tm time;
            time.tm_isdst = -1;
            time.tm_year = year - 1900;
            time.tm_mon = month - 1;
            time.tm_mday = day;
            time.tm_hour = hour;
            time.tm_min = min;
            time.tm_sec = sec;
            return prj::time(_mkgmtime(&time));
        }
        return {};
    }

    prj::time strptime(std::string&& str) {
        return strptime(str);
    }
}
