//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_COMMON_H
#define CAPSFACTORY_COMMON_H


#include <string>

#define TAB_VALUE  "  "
#define TB TAB_VALUE
#define TB2 TB TB
#define TB3 TB TB TB
#define TB4 TB TB TB TB


#define RETURN_CODEFORMAT(Tab, TemplateStr, ...) \
{\
char code[500000];\
snprintf(code, sizeof(code), TemplateStr, __VA_ARGS__);\
return Common::add_tab(code, Tab);\
}

#define CODEFORMAT(StrRst, Tab, TemplateStr, ...) \
{\
char code[500000];\
snprintf(code, sizeof(code), TemplateStr, __VA_ARGS__);\
StrRst = Common::add_tab(code, Tab);\
}
class Common {
public:
    static std::string add_tab(const char *str, const char *tab);
    static std::string camel_case(const char *str);
    static std::string head_upcase(const char *str);
    static std::string to_upper(const char* s);
    static bool write_file(const std::string &filename, const std::string &content);
    static const std::string read_file(const char * filename);
};


#endif //CAPSFACTORY_COMMON_H
