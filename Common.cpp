//
// Created by 周飞宇 on 2018/11/20.
//

#include "Common.h"


std::string Common::add_tab(const char *str, const char *tab)
{
    std::string rst, src(str);
    auto iter = src.rbegin();
    int count = 0;
    while (*iter == '\n')
    {
        count++;
        iter++;
    }
    char *save;
    char *s = strtok_r(const_cast<char*>(src.c_str()), "\n", &save);
    while(s != nullptr){
        rst += tab;
        rst += s;
        s = strtok_r(nullptr, "\n", &save);
        if (s != nullptr)
            rst += "\n";
    }
    while(count != 0)
    {
        rst +="\n";
        count--;
    }

    return rst;
}