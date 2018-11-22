//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_COMMON_H
#define CAPSFACTORY_COMMON_H


#include <string>

#define TAB_VALUE  "  "
#define TB TAB_VALUE
#define TBTB TB TB
#define TBTBTB TB TB TB


#define RETURN_CODEFORMAT(Tab,TemplateStr, ...) \
char code[500000];\
snprintf(code, sizeof(code), TemplateStr, __VA_ARGS__);\
return Common::add_tab(code, Tab)

class Common {
public:
    static std::string add_tab(const char *str, const char *tab);
    static std::string camel_case(const char *str);
    static std::string head_upcase(const char *str);
    static std::string to_upper(const char* s);
};


#endif //CAPSFACTORY_COMMON_H
