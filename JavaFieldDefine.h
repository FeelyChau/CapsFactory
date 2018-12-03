//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_JAVAFIELDDEFINE_H
#define CAPSFACTORY_JAVAFIELDDEFINE_H
#include "BaseAll.h"

const char * const FieldJavaTypeStr[] = {"String", "int", "int", "long", "long", "float", "double", "user_define"};
const char * const FieldJavaReadFuncStr[] = {"readString", "readInt", "readInt", "readLong", "readLong", "readFloat", "readDouble", "readCaps"};
const char * const FieldJavaWriteFuncStr[] = {"write", "write", "write", "write", "write", "write", "write", "write"};


class JavaFieldDefine: public BaseFieldDefine{
public:
    ~JavaFieldDefine() override;

    const string create_get_function(const string &tab, CodeType ct) override;

    const string create_to_json_function(const string &tab, CodeType ct) override;

    const string create_set_function(const string &tab, CodeType ct) override;

    const string create_serialize_function(const string &tab, CodeType ct) override;

    const string create_deserialize_function(const string &tab, CodeType ct) override;

    const string create_class_member(const string &tab, CodeType ct) override;
};


#endif //CAPSFACTORY_JAVAFIELDDEFINE_H
