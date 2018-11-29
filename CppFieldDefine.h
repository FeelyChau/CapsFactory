//
// Created by 周飞宇 on 2018/11/17.
//

#ifndef CAPSFACTORY_FIELDDEFINE_H
#define CAPSFACTORY_FIELDDEFINE_H

#include <string>
#include "cJSON.h"
#include "BaseAll.h"

using namespace std;

const char * const FieldTypeStr[] = {"string", "int32_t", "uint32_t", "int64_t", "uint64_t", "float", "double", "user_define"};

class CppFieldDefine: public BaseFieldDefine {
public:
    CppFieldDefine() = default;
    virtual ~CppFieldDefine() override {};

    virtual const string create_get_function(const string &tab) override;

    virtual const string create_set_function(const string &tab) override;

    virtual const string create_serialize_function(const string &tab) override;

    virtual const string create_deserialize_function(const string &tab) override;

    virtual const string create_class_member(const string &tab) override;

    const string create_to_json_function(const string &tab) override;
};


#endif //CAPSFACTORY_FIELDDEFINE_H
