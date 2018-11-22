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


std::string Common::camel_case(const char *str)
{
    std::string rst;
    if (*str != '\0')
    {
        rst.push_back(*str);
        str++;
    }
    while(*str != '\0')
    {
        if (*str == '_' && *(str + 1) >= 'a' && *(str + 1) <= 'z')
        {
            str++;
            rst.push_back((char)toupper(*str));
        }
        else
            rst.push_back(*str);
        str++;
    }
    return rst;
}

std::string Common::head_upcase(const char *str) {

    std::string rst;
    if (*str != '\0')
    {
        if (*(str + 1) >= 'a' && *(str + 1) <= 'z')
        {
            rst.push_back((char)std::toupper(*str));
        }
        else if (*str == '_' && *(str + 1) >= 'a' && *(str + 1) <= 'z')
        {
            rst.push_back((char)std::toupper(*str + 1));
            str++;
        }
        str++;
    }
    while(*str != '\0')
    {
        if (*str == '_' && *(str + 1) >= 'a' && *(str + 1) <= 'z')
        {
            str++;
            rst.push_back((char)toupper(*str));
        }
        else
            rst.push_back(*str);
        str++;
    }
    return rst;
}

std::string Common::to_upper(const char *s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
