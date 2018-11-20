//
// Created by 周飞宇 on 2018/11/20.
//

#ifndef CAPSFACTORY_JAVAFIELDDEFINE_H
#define CAPSFACTORY_JAVAFIELDDEFINE_H
#include "BaseAll.h"

const char * const FieldJavaTypeStr[] = {"String", "Integer", "uintxxx", "long", "uint64xxx", "float", "double", "user_define"};


class JavaFieldDefine: public BaseFieldDefine{
public:
    ~JavaFieldDefine() override;

    const string create_get_function(const string &tab) override;

    const string create_set_function(const string &tab) override;

    const string create_serialize_function(const string &tab) override;

    const string create_deserialize_function(const string &tab) override;

    const string create_class_member(const string &tab) override;
};


#endif //CAPSFACTORY_JAVAFIELDDEFINE_H
